#ifndef EventGenerator_GeneratorBase_hh
#define EventGenerator_GeneratorBase_hh
//
// Base class to allow generic access to all of the event generator classes.
// Also provides access to the random number engine associated with the
// EventGenerator module.
//
// $Id: GeneratorBase.hh,v 1.7 2011/05/18 22:01:46 wb Exp $
// $Author: wb $
// $Date: 2011/05/18 22:01:46 $
//
// Original author Rob Kutschke
//

// Framework includes
#include "art/Framework/Services/Optional/RandomNumberGenerator.h"

// Mu2e includes
#include "ToyDP/inc/GenParticleCollection.hh"

// CLHEP includes
#include "CLHEP/Random/RandomEngine.h"

namespace mu2e {

  class GeneratorBase{

  public:
    GeneratorBase( ){
    }
    virtual ~GeneratorBase(){
    }

    virtual void generate( GenParticleCollection&  ) = 0;

  protected:

    // A helper function to access the random number engine associated with this module.
    static art::RandomNumberGenerator::base_engine_t&
    getEngine( const art::RandomNumberGenerator::label_t& engine_label = "" );

};

} // end namespace mu2e,

#endif /* EventGenerator_GeneratorBase_hh */

