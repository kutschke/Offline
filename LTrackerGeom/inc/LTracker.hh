#ifndef LTRACKER_HH
#define LTRACKER_HH
//
// Hold all geometry and identifier information about
// an LTracker.  In order to insulate this class from 
// knowledge of databases etc, this class must not know
// how to make itself.
//
// $Id: LTracker.hh,v 1.1 2009/09/30 22:57:47 kutschke Exp $
// $Author: kutschke $ 
// $Date: 2009/09/30 22:57:47 $
//
// Original author Rob Kutschke
//

#include <deque>
#include <vector>

#include "LTrackerGeom/inc/Device.hh"
#include "LTrackerGeom/inc/SectorId.hh"

#include "GeometryService/inc/Detector.hh"

namespace mu2e {

  class LTracker: public Detector{

  friend class LTrackerMaker;

  public:
    LTracker(){}
    ~LTracker(){};
    
    // Compiler generated copy and assignment constructors
    // should be OK.

    virtual std::string name() const { return "LTracker";}


    enum LTrackerDeviceId { undefined=-1, wedge, vane};
    
    double r0()   const { return _r0;}
    double z0()   const { return _z0;}
    double rOut() const { return _rOut;}

    double rInscribed() const { return _rInscribed;}

    double zHalfLength() const;

    double Tilt() const { return _tilt;}

    int nSides() const { return _devices.at(0).getSectors().size(); }

    double strawRadius() const{
      return getStraw(StrawId(wedge,0,0,0)).getDetail().outerRadius();
    }

    std::string const& fillMaterial() const { return _fillMaterial; }
    
    // Check for legal identifiers.
    bool isLegal(DeviceId d) const{
      return ( d>-1 && 
	       std::vector<Device>::size_type(d) <_devices.size() 
	       );
    };

    
    bool isLegal(const SectorId& sid) const{
      return (isLegal(sid._did) && 
	      sid._sector >-1   &&
	      std::vector<Sector>::size_type(sid._sector) < getDevice(sid._did).getSectors().size()
	      );
    }

    typedef std::vector<Sector>::size_type stypeLayer;
    bool isLegal(const LayerId& lid ) const{
      return ( isLegal(lid._sid) &&
	       lid._layer > -1   &&
	       std::vector<Layer>::size_type(lid._layer) < getSector(lid._sid).getLayers().size()
	       );
    }

    bool isLegal(const StrawId& sid) const{
      return ( isLegal(sid._lid) &&
	       sid._n > -1       &&
	       std::vector<Straw>::size_type(sid._n) < getLayer(sid._lid).getStraws().size()
	       );
    }

    // Accessors
    const std::vector<Device>& getDevices() const{ 
      return _devices;
    }
    
    const Device& getDevice ( DeviceId id) const{ 
      return _devices.at(id);
    }
    
    const Device& getSides() const { 
      return _devices.at(wedge);
    }

    const Device& getVanes() const { 
      return _devices.at(vane);
    }

    const Sector& getSector ( const SectorId& sid ) const{
      return _devices.at(sid.getDevice()).getSector(sid);
    }

    const Layer& getLayer ( const LayerId& lid ) const{
      return _devices.at(lid.getDevice()).getLayer(lid);
    }

    const Straw& getStraw ( const StrawId& sid ) const{
      return _devices.at(sid.getDevice()).getStraw(sid);
    }

    const Straw& getStraw ( int i ) const{
      return _allStraws.at(i);
    }

    const std::deque<Straw>& getAllStraws() const {return _allStraws;}

#ifndef __CINT__

    // Loop over all straws and call F.
    // F can be a class with an operator() or a free function.
    template <class F>
    inline void LTracker::forAllStraws ( F& f) const{
      for ( std::vector<Device>::const_iterator i=_devices.begin(), e=_devices.end();
	    i !=e; ++i){
	i->forAllStraws(f);
      }
    }

    template <class F>
    inline void LTracker::forAllLayers ( F& f) const{
      for ( std::vector<Device>::const_iterator i=_devices.begin(), e=_devices.end();
	    i !=e; ++i){
	i->forAllLayers(f);
      }
    }

    template <class F>
    inline void LTracker::forAllSectors ( F& f) const{
      for ( std::vector<Device>::const_iterator i=_devices.begin(), e=_devices.end();
	    i !=e; ++i){
	i->forAllSectors(f);
      }
    }
    
    template <class F>
    inline void LTracker::forAllDevices ( F& f) const{
      for ( std::vector<Device>::const_iterator i=_devices.begin(), e=_devices.end();
	    i !=e; ++i){
	f(*i);
      }
    }

#endif


protected:

    // Nominal values.  
    // _r0 = Nominal radius of the center of the sector.
    // _z0 = position of the center of the tracker relative to the origin
    //       of the Mu2e coordinate system.
    // _Inscribed = inscribed radius that is tangent to the inner part of the sector.
    // _tilt - rotation of each layer about the normal to its own plane.
    double _r0;
    double _z0;
    double _rInscribed;
    double _tilt;

    // Outer radius of a logical volume that will just contain the entire tracker.
    double _rOut;

    std::string _fillMaterial;

    // Detailed info about each type of straw.
    std::vector<StrawDetail> _strawDetail;

    // An LTracker is made of two devices, sides and vanes.
    std::vector<Device> _devices;

    // There will be pointers to the objects in this container.
    std::deque<Straw>  _allStraws;

    // Needed to complete the second phase of construction.
    void FillPointers1();
    void FillPointers2();
    

  };

} //namespace mu2e

#endif
