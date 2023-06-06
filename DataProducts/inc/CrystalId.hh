#ifndef DataProducts_CrystalId_hh
#define DataProducts_CrystalId_hh
//
// Offline identifier of one calorimeter crystal channel
// See also CaloId and CrystalRawId
//

#include "Offline/DataProducts/inc/CaloId.hh"

#include <iosfwd>

namespace mu2e {

  class CrystalId{

  public:

    using value_type=uint16_t;

    // No "explicit" for compatitibilty with existing code.
    CrystalId(value_type id):_id(id){}
    CrystalId(CaloId id ):_id(id.crystal()){}

    // Automatic conversion to value_type; needed for backwards compatibilty
    operator value_type() const { return _id;}

    // Not 100% sure if we need both this and the conversion to value_type.
    value_type id() const { return _id;}

    // Return the Ids of the two SiPMs on this crystal.
    CaloId   sipm0()    const { return CaloId(_id*2); }
    CaloId   sipm1()    const { return CaloId(_id*2 + 1); }

    value_type disk()     const { return sipm0().disk(); }
    bool       isValid()  const { return _id < CaloId::_nChannel; }
    bool       isCaphri() const { return sipm0().isCaphri(); }

  private:

    value_type _id;
  };

  std::ostream& operator<<(std::ostream& ost, const CrystalId &id );

};
#endif /* DataProducts_CrystalId_hh */
