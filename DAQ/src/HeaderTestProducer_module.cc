// Produce fake Run, SubRun and Event header classes.
//

#include "mu2e-artdaq-core/Data/EventHeader.hh"
#include "mu2e-artdaq-core/Data/RunHeader.hh"
#include "mu2e-artdaq-core/Data/SubRunHeader.hh"

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/ParameterSet.h"

#include <ctime>
#include <iostream>
#include <memory>
#include "unistd.h"

namespace mu2e {

  class HeaderTestProducer : public art::EDProducer {

  public:

    HeaderTestProducer( fhicl::ParameterSet pset );
    void beginRun   ( art::Run&    run    ) override;
    void beginSubRun( art::SubRun& subrun ) override;
    void produce    ( art::Event&  event  ) override;

  private:

    // Starting value; will increment for each event.
    EWT _ewt = 123456;

  };

  HeaderTestProducer::HeaderTestProducer( fhicl::ParameterSet pset ):
    art::EDProducer{pset}{
    produces<EventHeader>();
    produces<SubRunHeader,art::InSubRun>();
    produces<RunHeader,art::InRun>();
  }

  void HeaderTestProducer::beginRun( art::Run& run ){
    auto rh = std::make_unique<RunHeader>( time(0), _ewt);
    std::cout << "\nMark: beginRun: " << run.id() << " " << *rh << std::endl;
    run.put( std::move(rh), art::fullRun() );
  }

  void HeaderTestProducer::beginSubRun( art::SubRun& subrun ){
    auto sh = std::make_unique<SubRunHeader>( time(0), _ewt);
    std::cout << "Mark: beginSubRun: " << subrun.id() << " " << *sh << std::endl;
    subrun.put( std::move(sh), art::fullSubRun() );
  }

  void HeaderTestProducer::produce( art::Event& event ){

    uint32_t mode   = (event.id().run()==1) ? 3 : 2;
    uint8_t  RFmTDC = (_ewt%2 == 0) ? 64 : 63;
    uint8_t  flags  = (event.id().run()==1) ? 4 : 1;


    auto eh = std::make_unique<EventHeader>( _ewt++, mode, RFmTDC, flags);
    std::cout << "Mark: event: " << event.id() << "  " << *eh << std::endl;

    event.put( std::move(eh) );

    // Sleep for 1 second; so that the timestamps in the Run and SubRun headers will change.
    sleep( 1);

  }

}

DEFINE_ART_MODULE(mu2e::HeaderTestProducer)
