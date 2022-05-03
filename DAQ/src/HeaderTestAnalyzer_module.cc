// Read back fake Run, SubRun and Event header objects made by HeaderTestAnalyzer
//

#include "mu2e-artdaq-core/Data/EventHeader.hh"
#include "mu2e-artdaq-core/Data/RunHeader.hh"
#include "mu2e-artdaq-core/Data/SubRunHeader.hh"

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/ParameterSet.h"

#include <ctime>
#include <iostream>
#include <memory>
#include "unistd.h"

namespace mu2e {

  class HeaderTestAnalyzer : public art::EDAnalyzer {

  public:

    HeaderTestAnalyzer( fhicl::ParameterSet pset );
    void beginRun   ( art::Run const&    run    ) override;
    void beginSubRun( art::SubRun const& subrun ) override;
    void analyze    ( art::Event const&  event  ) override;

  private:

  };

  HeaderTestAnalyzer::HeaderTestAnalyzer( fhicl::ParameterSet pset ):
    art::EDAnalyzer{pset}{
  }

  void HeaderTestAnalyzer::beginRun( art::Run const& run ){
    auto rh = run.getValidHandle<RunHeader>("generateHeaders");
    std::cout << "\nMark: beginRun: " << run.id() << " " << *rh << std::endl;
  }

  void HeaderTestAnalyzer::beginSubRun( art::SubRun const& subrun ){
    auto sh = subrun.getValidHandle<SubRunHeader>("generateHeaders");
    std::cout << "Mark: beginSubRun: " << subrun.id() << " " << *sh << std::endl;
  }

  void HeaderTestAnalyzer::analyze( art::Event const& event ){
    auto eh = event.getValidHandle<EventHeader>("generateHeaders");
    std::cout << "Mark: event: " << event.id() << "  " << *eh << std::endl;
  }

}

DEFINE_ART_MODULE(mu2e::HeaderTestAnalyzer)
