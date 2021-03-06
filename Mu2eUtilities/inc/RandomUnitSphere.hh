#ifndef RANDOMUNITSHPERE_HH
#define RANDOMUNITSHPERE_HH

//
// Return Hep3Vector objects that are unit vectors uniformly
// distributed over the unit sphere.
// 
// $Id: RandomUnitSphere.hh,v 1.1 2009/09/30 22:57:47 kutschke Exp $
// $Author: kutschke $ 
// $Date: 2009/09/30 22:57:47 $
//
// Original author Rob Kutschke
//
// Allows for range limitations on cos(theta) and phi.
//
//

#include <cmath>
#include "CLHEP/Vector/ThreeVector.h"

namespace mu2e { 

  class RandomUnitSphere {

  public:
  
    RandomUnitSphere():
      _czmin(-1.),
      _czmax( 1.),
      _phimin(0.),
      _phimax(twoPi){
    }

    RandomUnitSphere( double czmin,
		      double czmax,
		      double phimin=0,
		      double phimax=twoPi):
      _czmin(czmin),
      _czmax(czmax),
      _phimin(phimin),
      _phimax(phimax){
    }

    ~RandomUnitSphere(){}

    CLHEP::Hep3Vector shoot() const;

    
    // Do I really want the setters?
    void setczmin(double czmin){
      _czmin=czmin;
    }
    
    void setczmax(double czmax){
      _czmax=czmax;
    }
    
    void setphimin(double phimin){
      _phimin=phimin;
    }
    
    void setphimax(double phimax){
      _phimax=phimax;
    }

    double czmin(){ return _czmin;}
    double czmax(){ return _czmax;}
    
    double phimin(){ return _phimin;}
    double phimax(){ return _phimax;}
    
    
  private:

    static const double twoPi = 2.*M_PI;

    double _czmin;
    double _czmax;
    double _phimin;
    double _phimax;
    
  };

}

#endif
