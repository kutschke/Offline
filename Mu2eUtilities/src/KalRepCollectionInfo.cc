//
// Convenience class to access provenance information about a KalRepCollection.
//
// Contact person, Rob Kutschke
//

#include <exception>
#include <memory>
#include <typeinfo>

#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Provenance.h"
#include "canvas/Persistency/Provenance/ProductID.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/exception.h"

#include "Offline/Mu2eUtilities/inc/KalRepCollectionInfo.hh"


namespace mu2e {

  KalRepCollectionInfo::KalRepCollectionInfo( art::Handle<KalRepCollection> const& handle ):
    patRecType_(handle.provenance()->parameterSet().get<std::string>("module_type")),
    instance_(handle.provenance()->productInstanceName()){
  }

  KalRepCollectionInfo::KalRepCollectionInfo( art::ValidHandle<KalRepCollection> const& handle ):
    patRecType_(handle.provenance()->parameterSet().get<std::string>("module_type")),
    instance_(handle.provenance()->productInstanceName()){
  }


}
