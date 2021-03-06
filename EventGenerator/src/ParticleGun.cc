//
// Shoots a single particle gun and puts its output
// into a generated event.
//
// $Id: ParticleGun.cc,v 1.1 2009/09/30 22:57:47 kutschke Exp $
// $Author: kutschke $ 
// $Date: 2009/09/30 22:57:47 $
//
// Original author Rob Kutschke
// 

#include <iostream>

#include "EventGenerator/inc/ParticleGun.hh"
#include "Mu2eUtilities/inc/SimpleConfig.hh"

using namespace std;

using CLHEP::Hep3Vector;
using CLHEP::HepLorentzVector;

namespace mu2e {

  ParticleGun::ParticleGun( const SimpleConfig& config ):
    GeneratorBase(){
  }

  ParticleGun::~ParticleGun(){
  }


  void ParticleGun::generate( ToyGenParticleCollection& genParts ){

    int pdgid(1);
    Hep3Vector pos(0.,0.,0);
    HepLorentzVector mom(0.,0.,0.,0.);
    double time(0.);
    

    genParts.push_back( ToyGenParticle( pdgid, GenId::particleGun, pos, mom, time));

  }

}
