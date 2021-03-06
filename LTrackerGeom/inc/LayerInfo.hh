#ifndef LAYERINFO_HH
#define LAYERINFO_HH
//
// Information about a Layer.  Used by LTrackerMaker
// to construct an LTracker.
//
//
// $Id: LayerInfo.hh,v 1.1 2009/09/30 22:57:47 kutschke Exp $
// $Author: kutschke $ 
// $Date: 2009/09/30 22:57:47 $
//
// Original author Rob Kutschke
//

namespace mu2e {

struct LayerInfo{

  // The straws with adjacent cathode pads are conductive.
  // The straws without adjacent cathode pads are non-conductive.
  enum Stype {conductive, nonconductive, undefined};

public:
  LayerInfo():
    _nStraws(-1),
    _strawType(undefined)
  {
  }
  LayerInfo( int nStraws,
	     Stype strawType
	     ):
    _nStraws(nStraws),
    _strawType(strawType){
  }
  
  ~LayerInfo  (){}
  
  // Compiler generated copy and assignment constructors
  // should be OK.
  int _nStraws;
  Stype _strawType;
  
};

}  //namespace mu2e

#endif
