// class to resolve hit ambiguities by panel, assuming a reasonable track
// fit as input
//
// $Id: PanelAmbigResolver.cc,v 1.4 2012/08/31 22:39:00 brownd Exp $
// $Author: brownd $ 
// $Date: 2012/08/31 22:39:00 $
//
#include "KalmanTests/inc/PanelAmbigResolver.hh"
#include "KalmanTests/inc/PanelStateIterator.hh"
#include "KalmanTests/inc/KalFitResult.hh"
#include "KalmanTests/inc/TrkStrawHit.hh"
#include "BaBar/BaBar.hh"
#include "TrkBase/TrkT0.hh"
#include "KalmanTrack/KalRep.hh"
#include "KalmanTrack/KalSite.hh"
#include "KalmanTrack/KalHit.hh"
#include "TrkBase/TrkPoca.hh"
#include "difAlgebra/DifPoint.hh"
#include "difAlgebra/DifVector.hh"
#include <vector>
#include <algorithm>
#include <functional>
// art
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
///using namespace CLHEP; this conflicts with BaBar HepPoint FIXME!!

namespace mu2e {
  namespace PanelAmbig {
    // functor for sorting by panel.  Note that SectorId uniquely defines a panel
    struct panelcomp : public std::binary_function<TrkStrawHit*, TrkStrawHit*, bool> {
      bool operator()(TrkStrawHit* x, TrkStrawHit* y) { return x->straw().id().getSectorId() < y->straw().id().getSectorId(); }
    };

    // functor to sort panel results by chisquared
    struct resultcomp : public std::binary_function<PanelResult const&, PanelResult const&,bool> {
      bool operator()(PanelResult const& a, PanelResult const& b) { return a._chisq < b._chisq; }
    };

    typedef TSHV::iterator TSHI;
    typedef TSHV::const_iterator TSHCI;
    typedef std::vector<KalSite*>::const_iterator KSI;


    PanelAmbigResolver::PanelAmbigResolver(fhicl::ParameterSet const& pset, double ExtErr, int Iter,KalDiag* kdiag): 
      AmbigResolver(pset,ExtErr,Iter),
      _minsep(pset.get<double>("minChisqSep",6.0)),
      _inactivepenalty(pset.get<double>("inactivePenalty",16.0)),
      _penaltyres(pset.get<double>("PenaltyResolution",0.5)),
      _addtrkpos(pset.get<bool>("AddTrackPositionConstraint",true)),
      _diag(pset.get<int>("DiagLevel",0)),
      _kdiag(kdiag)
    {
      double nullerr = pset.get<double>("NullAmbigPenalty",0.05);
      _nullerr2 = nullerr*nullerr;
      std::vector<int> allowed = pset.get< std::vector<int> >("AllowedHitStates");
      for(std::vector<int>::iterator ial = allowed.begin();ial != allowed.end();++ial){
	if(*ial >= HitState::negambig && *ial < HitState::ignore){
	  _allowed.push_back(HitState(static_cast<HitState::TSHState>(*ial)));
	} else
	  throw cet::exception("RECO")<<"mu2e::PanelAmbigResolver: illegal state" << std::endl;
      }
      // if requested, setup diagnostics
      if(_diag > 0){
	art::ServiceHandle<art::TFileService> tfs;
	char title[40];
	snprintf(title,40,"padiag_%i",Iter);
	_padiag=tfs->make<TTree>(title,"Panel Ambiguity Resolution Diagnostics");
	_padiag->Branch("nhits",&_nrhits,"nhits/I");
	_padiag->Branch("nactive",&_nactive,"nactive/I");
	_padiag->Branch("nres",&_nres,"nres/I");
	_padiag->Branch("results",&_results);

	snprintf(title,40,"pudiag_%i",Iter);
	_pudiag=tfs->make<TTree>(title,"Panel U position Diagnostics");
	_pudiag->Branch("nhits",&_nuhits,"nhits/I");
	_pudiag->Branch("tupos",&_tupos,"tupos/F");
	_pudiag->Branch("tuerr",&_tuerr,"tuerr/F");
	_pudiag->Branch("uinfo",&_uinfo);
	if(_kdiag != 0){
	  _pudiag->Branch("mctupos",&_mctupos,"mctupos/F");
	  _pudiag->Branch("mcuinfo",&_mcuinfo);
	}
      }
    }

    PanelAmbigResolver::~PanelAmbigResolver() {}

    void PanelAmbigResolver::resolveTrk(KalFitResult& kfit, int Final) const {
      // initialize hit external errors
      initHitErrors(kfit);
      // optionally setup MC truth
      if(_diag > 0 && _kdiag != 0){
	_kdiag->kalDiag(kfit._krep,false);
      }
      // sort by panel
      std::sort(kfit._hits.begin(),kfit._hits.end(),panelcomp());
      // collect hits in the same panel
      TSHI ihit = kfit._hits.begin();
      while(ihit != kfit._hits.end()){ 
	SectorId pid = (*ihit)->straw().id().getSectorId();
	(*ihit)->setExtErr(AmbigResolver::_extErr);
	TSHI jhit = ihit;
	TSHV phits;
	while(jhit != kfit._hits.end() && (*jhit)->straw().id().getSectorId() == pid){
	  phits.push_back(*jhit++);
	}
	// resolve the panel hits
	resolvePanel(phits,kfit);
	ihit = jhit;
      }
    }

    void PanelAmbigResolver::resolvePanel(TSHV& phits,KalFitResult& kfit) const {
      // fill panel information
      PanelInfo pinfo;
      fillPanelInfo(phits,kfit._krep,pinfo);
      // loop over all ambiguity/activity states for this panel
      PanelStateIterator psi(phits,_allowed);
      PRV results;
      do {
	// for each state, fill the result of the 1-dimensional optimization
	PanelResult result(psi.current());
	fillResult(pinfo,kfit._krep->t0(),result);
	if(result._status == 0)results.push_back(result);
      } while(psi.increment());
      if(results.size() > 0){
	// sort the results to have lowest chisquard first
	std::sort(results.begin(),results.end(),resultcomp());
	// for now, set the hit state according to the best result.  In future, maybe we want to treat
	// cases with different ambiguities differently from inactive hits
	results[0]._state.setHitStates(phits);
	// if the chisq difference between patterns is negligible, inflate the errors of the
	// hit which changes
	size_t nhits = results[0]._state._nhits;
	size_t ires(1);
	while (ires < results.size() && results[ires]._chisq - results[0]._chisq < _minsep){
	  for(size_t ihit=0;ihit<nhits;++ihit){
	    if(results[ires]._state.hitState(ihit) != results[0]._state.hitState(ihit)){
	      phits[ihit]->setPenalty(_penaltyres);
	    }
	  }
	  ++ires;
	}
      }
      if( _diag > 1 ) {
	_nuhits = _nrhits = pinfo._uinfo.size();
	_nactive = 0;
	for(auto ishi : pinfo._uinfo) {
	  if(ishi._active)++_nactive;
	}
	_nres = results.size();
	_results = results;
	_padiag->Fill();
	//
	_tupos = pinfo._tupos;
	_tuerr = pinfo._tuerr;
	_uinfo = pinfo._uinfo;
	if(_kdiag != 0){
	  // fill MC truth for these hits and track
	  _mcuinfo.clear();
	  // use 1st hit for MC information
	  StrawDigiMC const& mcdigi = _kdiag->mcData()._mcdigis->at(pinfo._uinfo[0]._index);
	  StrawDigi::TDCChannel itdc = StrawDigi::zero;
	  if(!mcdigi.hasTDC(StrawDigi::zero))
	    itdc = StrawDigi::one;
	  CLHEP::Hep3Vector const& mctpos = mcdigi.stepPointMC(itdc)->position();
	  CLHEP::Hep3Vector udir(pinfo._udir._x, pinfo._udir._y, pinfo._udir._z);
	  _mctupos = udir.dot(mctpos);		
	  for(auto ishi : pinfo._uinfo) {
	    TSHMCUInfo mcinfo;
	    TrkStrawHitInfoMC const& tshmc = _kdiag->_tshinfomc[ishi._index];
	    mcinfo._ambig = tshmc._ambig;
	    mcinfo._dr = tshmc._dist;
	    _mcuinfo.push_back(mcinfo);
	  }
	}
	_pudiag->Fill();
      }
    }

    bool PanelAmbigResolver::fillPanelInfo(TSHV const& phits, const KalRep* krep, PanelInfo& pinfo) const {
      bool retval(false);
      // find the best trajectory we can local to these hits, but excluding their information ( if possible).
      const TrkSimpTraj* straj = findTraj(phits,krep);
      if(straj != 0){
	// find POCA to the first hit
	const TrkStrawHit* firsthit = phits[0];
	TrkPoca tpoca(*straj,firsthit->fltLen(),*firsthit->hitTraj(),firsthit->hitLen());
	if(tpoca.status().success()){
	  retval = true;
	  // find the position and direction information at the middle of this range.
	  DifPoint tpos;
	  DifVector tdir;
	  straj->getDFInfo2(tpoca.flt1(), tpos, tdir);
	  // straw information; cast to dif even though the derivatives are 0
	  CLHEP::Hep3Vector wdir = firsthit->hitTraj()->direction(firsthit->hitLen());
	  HepPoint wpos = firsthit->hitTraj()->position(firsthit->hitLen());
	  CLHEP::Hep3Vector wposv(wpos.x(),wpos.y(),wpos.z());
	  DifVector delta = DifVector(wposv) - tpos;
	  // compute the U direction (along the measurement, perp to the track and wire
	  // The sign is chosen to give positive and negative ambiguity per BaBar convention
	  DifVector dudir = cross(DifVector(wdir),tdir).normalize();
	  CLHEP::Hep3Vector udir(dudir.x.number(),dudir.y.number(),dudir.z.number());
	  // compute the track constraint on u, and it's error.  The dif algebra part propagates the error
	  DifNumber trku = dudir.dot(delta);
	  pinfo._udir = udir;
	  Hep3Vector tposv(tpos.x.number(),tpos.y.number(),tpos.z.number());
	  pinfo._tupos = udir.dot(tposv);
	  pinfo._tuerr = trku.error();
	  pinfo._tuwt = 1.0/(pinfo._tuerr*pinfo._tuerr);
	  // now, project the hits onto u, WRT the projected track position. 
	  for(TSHCI ihit = phits.begin();ihit != phits.end();++ihit){
	    pinfo._uinfo.push_back(TSHUInfo(*ihit,udir,tpos.hepPoint()));
	  }
	}
      } else {
	throw cet::exception("RECO")<<"mu2e::PanelAmbigResolver: no trajectory" << std::endl;
      }
      return retval;
    }

    void PanelAmbigResolver::fillResult(PanelInfo const& pinfo,TrkT0 const& t0, PanelResult& result) const {
      // initialize the sums
      double wsum(0.0);
      double uwsum(0.0);
      double vwsum(0.0);
      double uuwsum(0.0);
      double vvwsum(0.0);
      double uvwsum(0.0);
      double chi2penalty(0.0);
      // loop over the straw hit info and accumulate the sums used to compute chisquared
      size_t ntsh = pinfo._uinfo.size();
      // consistency check
      if(ntsh != result._state._nhits) 
	throw cet::exception("RECO")<<"mu2e::PanelAmbigResolver: inconsistent hits" << std::endl;
      unsigned nused(0);
      for(size_t itsh=0;itsh<ntsh;++itsh){
	// only accumulate if the state is active
	HitState const& tshs = result._state.hitState(itsh);
	TSHUInfo const& tshui = pinfo._uinfo[itsh];
	if(tshs.active()){
	  ++nused;
  // compare this state to the original, record any differences
	  if(tshs != HitState(tshui._ambig, tshui._active))
	    result._statechange |= (itsh << 1);	    
	  double w = tshui._uwt;
	  double r = tshui._dr;
	  double v = tshui._dv;
	  // sign for ambiguity
	  if(tshs._state == HitState::negambig){
	    r *= -1;
	    v *= -1;
	  } else if(tshs._state == HitState::noambig){
	    r = 0;
	    w = 1.0/(1.0/w + _nullerr2); // increase the error on 0 ambiguity hits
	  }
	  double u = tshui._upos + r;
	  wsum += w;
	  uwsum += u*w;
	  vwsum += v*w;
	  uuwsum += u*u*w;
	  vvwsum += v*v*w;
	  uvwsum += u*v*w;
	} else if(tshs._state == HitState::inactive) // penalize inactive hits
	  chi2penalty += _inactivepenalty;
      }
      if(nused > 0){
	// propogate t0 uncertainty.  This is a constraint centered at the
	// current value of t0, unit derivative
	double t0wt = 1.0/(t0._t0err*t0._t0err);
	vvwsum += t0wt; // t0 has unit derrivative
	// optionally add track position constraint if there are multiple hits.  It is like a hit, but with r=v=0
	// NB, since the track position is defined to be 0, it adds only to the weight
	if(_addtrkpos || nused == 1)wsum += pinfo._tuwt;
	// now compute the scalar, vector, and tensor terms for the 2nd-order chisquared expansion
	double alpha = uuwsum;
	CLHEP::HepVector beta(2);
	beta(1) = uwsum;
	beta(2) = uvwsum;
	HepSymMatrix gamma(2);
	gamma.fast(1,1) = wsum;
	gamma.fast(2,2) = vvwsum;
	gamma.fast(1,2) = vwsum;
	// invert and solve
	gamma.invert(result._status);
	if(result._status == 0){
	  result._dcov = gamma;
	  result._delta = gamma * beta;
	  result._chisq = alpha - gamma.similarity(beta) + chi2penalty;
	  // debug printout
#ifdef DEBUG
	  double g11 = gamma.fast(1,1);
	  double g22 = gamma.fast(2,2);
	  double g12 = gamma.fast(1,2);
	  double g21 = gamma.fast(2,1);
	  double b1 = beta(1);
	  double b2 = beta(2);
	  double d1 = result._delta(1);
	  double d2 = result._delta(2);
	  double sim = gamma.similarity(beta);
	  double test = alpha - sim;
	  double test2 = alpha -2*dot(beta,result._delta) + gamma.similarity(result._delta);
#endif
	  // add a penalty term (if any) for this particular pattern.  Still to be written, FIXME!!!
	  //  addPatternPenalty();
	}
      } else {
	result._status = -100;
      }
    }

  } // PanelAmbig namespace
} // mu2e namespace
