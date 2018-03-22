#ifndef RecoDataProducts_CrvCoincidenceCheckResult_hh
#define RecoDataProducts_CrvCoincidenceCheckResult_hh
//
// $Id: $
// $Author: ehrlich $
// $Date: 2014/08/07 01:33:41 $
//
// Contact person Ralf Ehrlich
//

#include "DataProducts/inc/CRSScintillatorBarIndex.hh"

#include <vector>

namespace mu2e 
{
  class CrvCoincidenceCheckResult
  {
    public:

    struct CoincidenceCombination
    {
      double                        _time[3];
      int                           _PEs[3];
      mu2e::CRSScintillatorBarIndex _counters[3];
      int                           _SiPMs[3];
      CoincidenceCombination() {}
    };

    CrvCoincidenceCheckResult() {}

    const bool CoincidenceFound() const 
    {
      return (!_coincidenceCombinations.empty());
    }

    const std::vector<CoincidenceCombination> &GetCoincidenceCombinations() const
    {
      return _coincidenceCombinations;
    }

    std::vector<CoincidenceCombination> &GetCoincidenceCombinations()
    {
      return _coincidenceCombinations;
    }

    private:

    std::vector<CoincidenceCombination> _coincidenceCombinations;
  };
}

#endif /* RecoDataProducts_CrvCoincidenceCheckResult_hh */
