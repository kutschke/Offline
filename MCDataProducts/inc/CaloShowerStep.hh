#ifndef MCDataProducts_CaloShowerStep_hh
#define MCDataProducts_CaloShowerStep_hh

//
// Compress information of the shower generated in the crystal / readout in the calorimeter
// A CaloShowerStep records the position, time and energy of the fraction of an electromagnetic shower 
// generated by a SimParticle in a small longitudinal slice of a crystal (simple, right). 
//
// The Particle Id refers to the type of particle leaving the energy deposit, not the Particle originating the shower
//
// Original author Bertrand Echenard
//

#include <iostream>
#include "MCDataProducts/inc/SimParticle.hh"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Matrix/SymMatrix.h"




namespace mu2e {


   class CaloShowerStep {


       public:
	 
	  CaloShowerStep(): volumeId_(-1),sim_(),nCompress_(0),time_(0),energyMC_(0),momentumIn_(0),posIn_(),pos_(),covPos_() {}
	  
	  CaloShowerStep(int volumeId, art::Ptr<SimParticle> const& sim, int nCompress, double time,  
	                 double energy, double momentumIn, const CLHEP::Hep3Vector& posIn, const CLHEP::Hep3Vector& pos, 
			 const CLHEP::HepSymMatrix& covPos): 
	    volumeId_(volumeId),
	    sim_(sim),
	    nCompress_(nCompress),
	    time_(time),
	    energyMC_(energy),
	    momentumIn_(momentumIn),
	    posIn_(posIn), 
	    pos_(pos), 
	    covPos_(covPos)	  
	    {}


	  int                            volumeId()    const {return volumeId_;}
	  const art::Ptr<SimParticle>&   simParticle() const {return sim_;}
	  int                            nCompress()   const {return nCompress_;}
	  double                         timeStepMC()  const {return time_;}
	  double                         energyMC()    const {return energyMC_;}
	  double                         momentumIn()  const {return momentumIn_;}
	  const CLHEP::Hep3Vector&       positionIn()  const {return posIn_;}	  
	  const CLHEP::Hep3Vector&       position()    const {return pos_;}	  
	  const CLHEP::HepSymMatrix&     covPosition() const {return covPos_;}	  

          void setSimParticle(const art::Ptr<SimParticle>& sim) {sim_ = sim;}                                                    

          void print(std::ostream& ost = std::cout) const {ost<<"Calo Shower content    volumeId = "<<volumeId_<<"  pid="<<sim_->pdgId()
	                                                      <<"  edep="<<energyMC_<<"  time="<<time_<<" position = "<<pos_<<std::endl;}
                                                              


       private:
       
	    int                     volumeId_;      // volume_id : either crystal_id or readout_id
	    art::Ptr<SimParticle>   sim_;           // pdg id code of particle leaving energy
	    int                     nCompress_;     // number of StepPointMC compressed inside this ShowerStep
	    double                  time_;          // time of first stepPointMC
	    double                  energyMC_;      // total energy deposited at MC level
	    double                  momentumIn_;    // momentum of incoming particle
	    CLHEP::Hep3Vector       posIn_;           // position in the crystal frame
	    CLHEP::Hep3Vector       pos_;           // position in the crystal frame
	    CLHEP::HepSymMatrix     covPos_;        // position covariance matrix
   };

} 

#endif
