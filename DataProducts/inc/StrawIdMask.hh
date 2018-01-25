#ifndef TrackerGeom_StrawIdMask_hh
#define TrackerGeom_StrawIdMask_hh
//
//  Specify a subset of StrawId fields, and provide a set of
//  accessors accordingly
//
#include "DataProducts/inc/StrawId2.hh"
#include <vector>
namespace mu2e {
  class StrawIdMask {
    // fields of StrawId
    public:
    enum field{station=0,plane,panel,layer,straw};
    // compute the mask associated with a given field
    static uint16_t fieldMask(field fval);
    // inform user which fields are being compared: this returns true if ALL
    // the bits for that field are present, regardless of how the object was initialized
    bool compare(field fval) const {
      return (fieldMask(fval) & _mask) == fieldMask(fval); }
    // specify which fields to compare on construction.  These will be 'ORed' together, so
    // selecting overlapping fields will result in the broader category.  Note that the fields
    // are specific: a StrawIdMask at field 'panel' will match Ids with the same panel regardless
    // of plane, straw, ...   To identify unique panels, specify both 'plane' and 'panel'
    // default constructor matches nothing
    StrawIdMask() : _mask(0) {}
    StrawIdMask(std::vector<field> const& fields) : StrawIdMask() {
      for(auto fval :fields ) _mask |= fieldMask(fval);
    }
    // comare StrawIds at the given field.
    bool lessthan (StrawId2 const& sid1, StrawId2 const& sid2) const {
      return (sid1.strawId2() & _mask) < (sid2.strawId2() & _mask); }
    bool equal (StrawId2 const& sid1, StrawId2 const& sid2) const {
      return (sid1.strawId2() & _mask) == (sid2.strawId2() & _mask); }
    bool notequal (StrawId2 const& sid1, StrawId2 const& sid2) const {
      return (! equal(sid1,sid2)); }
    // return a truncated StrawID.  This is returned by value
    StrawId2 maskStrawId(StrawId2 const& sid) const {
      return StrawId2(sid.strawId2() & _mask); }
    private:
    uint16_t _mask;
  };
  uint16_t StrawIdMask::fieldMask(StrawIdMask::field fval) {
    switch (fval) {
      case plane :
	return StrawId2::_planemsk;
      case panel :
	return StrawId2::_panelmsk;
      case straw :
	return StrawId2::_strawmsk;
      case station :
	return StrawId2::_stationmsk;
      case layer :
	return StrawId2::_layermsk;
      default:
	return 0;
    }
    return 0;
  }

}
#endif

