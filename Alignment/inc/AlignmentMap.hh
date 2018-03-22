#ifndef Alignment_AlignmentMap_HH
#define Alignment_AlignmentMap_HH
// AlignmentMap.hh
// This is the definition file for the AlignmentMap class.
// This class holds a list of associations between GeomHandle objects and
// AlignmentObj objects for use in the Mu2e Alignment service.

#include "Alignment/inc/AlignmentSequence.hh"
#include "ConfigTools/inc/SimpleConfig.hh"

#include <unordered_map>
#include <string>
#include <iostream>

namespace mu2e {
  class AlignmentMap {
  public:
    AlignmentMap();
    ~AlignmentMap(){}

    void make(const SimpleConfig&  _config );
    AlignmentSequence find(std::string& handle) ;

    void addAlignment( std::string& handle, AlignmentSequence& as );

  private:
    std::unordered_map<std::string, AlignmentSequence> _map;

  };
} // end of namespace mu2e
#endif // Alignment_AlignmentMap_HH
