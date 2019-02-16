import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(
     'file:/uscms_data/d2/spanier/mc/yyh/set1.root', 
     #'file:/uscms_data/d2/spanier/mc/jjz/ztojj1.root', 
     #'file:/uscms_data/d2/spanier/mc/jjh/set2.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set3.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set4.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set5.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set6.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set7.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set8.root',
     #'file:/uscms_data/d2/spanier/mc/jjh/set9.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set10.root',
     #'file:/uscms_data/d2/spanier/mc/jjh/set11.root',
     #'file:/uscms_data/d2/spanier/mc/jjh/set12.root',
     #'file:/uscms_data/d2/spanier/mc/jjh/set13.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set14.root',      
     #'file:/uscms_data/d2/spanier/mc/jjh/set15.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set16.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set17.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set18.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set19.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set20.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set21.root',     
     #'file:/uscms_data/d2/spanier/mc/jjh/set22.root',                        
     )
  )

process.demo = cms.EDAnalyzer('GensimAnalyzer',
#rootFileName                 = cms.untracked.string("GENEventTree_lhcb_fix.root")
#    rootFileName                 = cms.untracked.string("GENEventTree_tests.root")
#    rootFileName                 = cms.untracked.string("GENEventTree_xl21_fix.root")
    rootFileName                 = cms.untracked.string("GENEventTree_HJJ_Tot.root")
)


process.p = cms.Path(process.demo)
