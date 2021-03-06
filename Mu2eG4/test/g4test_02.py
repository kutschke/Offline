# Configuration file for G4Test02
#
# $Id: g4test_02.py,v 1.1 2009/09/30 22:57:47 kutschke Exp $
# $Author: kutschke $
# $Date: 2009/09/30 22:57:47 $
#
# Original author Rob Kutschke
#
# Spacing is not signficant in this file.

# Define the default configuration for the framework.
import FWCore.ParameterSet.python.Config as mu2e

# Give this job a name.  
process = mu2e.Process("G4Test02")

# Maximum number of events to do.
process.maxEvents = mu2e.untracked.PSet(
    input = mu2e.untracked.int32(10)
)

# Load the standard message logger configuration.
# Threshold=Info. Limit of 5 per category; then exponential backoff.
process.load("Config/MessageLogger_cfi")

# Load the service that manages root files for histograms.
process.TFileService = mu2e.Service("TFileService",
                       fileName = mu2e.string("g4test_02.root"),
                       closeFileFast = mu2e.untracked.bool(False)
)

# Initialize the random number sequences.
# This just changes the seed for the global CLHEP random engine.
process.RandomNumberService = mu2e.Service("RandomNumberService",
                                           globalSeed=mu2e.untracked.int32(9877),
)                              

# Define the geometry.
process.GeometryService = mu2e.Service("GeometryService",
       inputfile=cms.untracked.string("Mu2eG4/test/geom_02.txt")
)

# Define and configure some modules to do work on each event.
# Modules are just defined for now, the are scheduled later.

# Start each new event with an empty event.
process.source = mu2e.Source("EmptySource")

# Make some generated tracks and add them to the event.
process.generate = mu2e.EDProducer(
    "EventGenerator",
    inputfile = mu2e.untracked.string("Mu2eG4/test/genconfig_02.txt")
)

# Run G4 and add its hits to the event.
process.g4run = mu2e.EDProducer(
    "G4"
    )

# Look at the hits from G4.
process.checkhits = mu2e.EDAnalyzer(
    "ReadBack",
    maxFullPrint = mu2e.untracked.int32(5)
)

# End of the section that defines and configures modules.


# Adjust configuration of message logger.
# Enable debug printout from the module instance "hitinspect".
# Print unlimited messages with category ToyHitInfo.
process.MessageLogger.cerr.threshold = mu2e.untracked.string('DEBUG')
process.MessageLogger.debugModules.append("hitinspect")
process.MessageLogger.categories.append("ToyHitInfo")
process.MessageLogger.categories.append("GEOM")

# Tell the system to execute all paths.
process.output = mu2e.EndPath(   process.generate*process.g4run*process.checkhits );

