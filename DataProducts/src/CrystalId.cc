#include "Offline/DataProducts/inc/CrystalId.hh"

#include <ostream>

namespace mu2e {

  std::ostream& operator<<(std::ostream& ost, const CrystalId &id ){
    ost << CrystalId::value_type(id);
    return ost;
  }

} // end namespace mu2e
