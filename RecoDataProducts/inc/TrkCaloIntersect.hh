//
// Container for the info of the iterception of a track with the calorimeter sections
//
// Original author B. Echenard
//


#ifndef RecoDataProducts_TrkCaloIntersect_hh
#define RecoDataProducts_TrkCaloIntersect_hh


// Mu2e includes:
#include "art/Persistency/Common/Ptr.h"


//tracker includes
#include "KalmanTrack/KalRep.hh"
#include "KalmanTests/inc/KalRepPtrCollection.hh"




namespace mu2e {


  class TrkCaloIntersect {

       public:

	   TrkCaloIntersect():_sectionId(-1),_trk(),_trkId(-1),_pathLengthEntrance(0.0),_pathLengthEntranceErr(0),_pathLengthExit(0.0) {}

	   TrkCaloIntersect(int section,  KalRepPtr& trk, int trkId, double entrance, double entranceErr, double exit):
	     _sectionId(section), _trk(trk), _trkId(trkId), _pathLengthEntrance(entrance),_pathLengthEntranceErr(entranceErr), _pathLengthExit(exit)
	   {}

	   ~TrkCaloIntersect(){}


	   int                sectionId()             const {return _sectionId;}
	   double             pathLengthEntrance()    const {return _pathLengthEntrance;}
	   double             pathLengthExit()        const {return _pathLengthExit;}
	   double             pathLenghtEntranceErr() const {return _pathLengthEntranceErr;}
	   KalRepPtr const&   trk()                   const {return _trk; }
	   int                trkId()                 const {return _trkId; }


       private:

	   int         _sectionId;           
	   KalRepPtr   _trk;
	   int         _trkId;
	   double      _pathLengthEntrance;
	   double      _pathLengthEntranceErr;
	   double      _pathLengthExit;

   };


} 

#endif 


/*
Copied a few useful trk accessors

	  double            time()                  const {return _trk->arrivalTime(_pathLengthEntrance);}
	  double            t0Err()                 const {return _trk->t0().t0Err();}
          double            t0()                    const {return _trk->t0().t0();}
	  double            fitConsistency()        const {return _trk->chisqConsistency().consistency();}
	  HepPoint          entrancePosition()      const {return _trk->position(_pathLengthEntrance);}
	  BbrPointErr       entrancePositionErr()   const {return _trk->positionErr(_pathLengthEntrance);}
	  HepPoint          exitPosition()          const {return _trk->position(_pathLengthExit);}
	  BbrPointErr       exitPositionErr()       const {return _trk->positionErr(_pathLengthExit);}
	  Hep3Vector        momentum()              const {return _trk->momentum(_pathLengthEntrance);}
	  BbrVectorErr      momentumErr()           const {return _trk->momentumErr(_pathLengthEntrance);}
	  Hep3Vector        t0Momentum()            const {return _trk->momentum(_trk->firstHit()->kalHit()->hitOnTrack()->fltLen());}
	  BbrVectorErr      t0MomentumErr()         const {return _trk->momentumErr(_trk->firstHit()->kalHit()->hitOnTrack()->fltLen());}
*/
