// -*- C++ -*-
//
// Package:    GensimAnalyzer
// Class:      GensimAnalyzer
// 
/**\class GensimAnalyzer GensimAnalyzer.cc GenAnalyzer/GensimAnalyzer/src/GensimAnalyzer.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
 */
//
// Original Author:  grant riley
//         Created:  Tue May 24 09:00 2015 CDT 2016
// $Id$
//
//


// system include files
#include <memory>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

// user include files
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TLorentzVector.h"
#include "FWCore/Utilities/interface/InputTag.h"
//
// class declaration
//
class TObject;
class TTree;
class TH1D;
class TFile;
class HepMCProduct;


class GensimAnalyzer : public edm::EDAnalyzer {
  public:
    explicit GensimAnalyzer(const edm::ParameterSet&);
    virtual ~GensimAnalyzer();
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;
    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void endRun(edm::Run const&, edm::EventSetup const&);
    virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
    virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
    TFile *fFile; //= new TFile(fRootFileName,"RECREATE");
    std::string fRootFileName;

    //    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    TTree *fTree;
    int eventN;
    int accEvN;
    int muN,jpN;
  float FourMu_Mass, FourMu_Rapidity, FourMu_pT, Psi1To2_dY, Psi_3dOpening, Psi_Mass[2], Psi_y[2], Psi_pT[2], Psi_px[2], Psi_py[2], Psi_pz[2], Psi_E[2], Psitemp_px[2], Psitemp_py[2], Psitemp_pz[2], Psitemp_E[2], Mu_Angle[2], Mu_Helicity[2], Mu_Eta[4], Mu_px[4], Mu_py[4], Mu_pz[4], Mu_pT[4], Mu_E[4], Mutemp_px[4], Mutemp_py[4], Mutemp_pz[4], Mutemp_E[4];
    int Mu_charge[4], Mutemp_charge[4], Iout[4], Iin[2], inN, Imom[2][10] ;
    int  pTOrderI[4];
    edm::EDGetTokenT<edm::HepMCProduct>  fGenEvtHandle;
  //    edm::EDGetTokenT<reco::GenParticle>  fgenp_token;

    //edm::EDGetTokenT<std::vector<edm::HepMCProduct> > fGenEvtHandle;
    // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
GensimAnalyzer::GensimAnalyzer(const edm::ParameterSet& iConfig):
  fRootFileName(iConfig.getUntrackedParameter<std::string>("rootFileName", std::string("GENEventTree.root"))),

  fGenEvtHandle(consumes<edm::HepMCProduct> (iConfig.getUntrackedParameter<edm::InputTag>("genEvtHandle", edm::InputTag("generatorSmeared"))))

{
  //now do what ever initialization is needed
}


GensimAnalyzer::~GensimAnalyzer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
  void
GensimAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  edm::Handle< edm::HepMCProduct> genEvtHandle;
  try{
  iEvent.getByToken( fGenEvtHandle, genEvtHandle) ;
}catch (cms::Exception &ex){
    std::cout << "No gen collection.... " << std::endl;
  }

  const HepMC::GenEvent* Evt = genEvtHandle->GetEvent() ;

  //
  // this is an example loop over the hierarchy of vertices
  //
  TLorentzVector m0(-1., -1., -1., -1.);
  TLorentzVector m1(-1., -1., -1., -1.);
  TLorentzVector m2(-1., -1., -1., -1.);
  TLorentzVector m3(-1., -1., -1., -1.);
  TLorentzVector p0(-1., -1., -1., -1.);
  TLorentzVector p1(-1., -1., -1., -1.);
  TLorentzVector m4m(-1., -1., -1., -1.);
  ++eventN;


  jpN=0;
  muN=0;

  int jev[2];
  int mpv[4];
  int nzp(0);

  for ( HepMC::GenEvent::vertex_const_iterator
      itVtx=Evt->vertices_begin(); itVtx!=Evt->vertices_end(); ++itVtx )
  {
    //
    // this is an example loop over particles coming out of each vertex in the loop
    //

    for ( HepMC::GenVertex::particles_out_const_iterator
        itPartOut=(*itVtx)->particles_out_const_begin();
        itPartOut!=(*itVtx)->particles_out_const_end(); ++itPartOut )
    {
      // and more of your code...
      inN=0;

      // collect the vertex of the Higgs and compare tio J/psi
      // for higgs its still ID=23 stupid


      //if(abs((*itPartOut)->pdg_id()==25)) std::cout << "Higgs !!!" << std::endl;
      if(abs((*itPartOut)->pdg_id()==23)) {
	nzp++;
        if(nzp>2) std::cout << "More than 2 Z-prime" << std::endl;
      }

      //      if(abs((*itPartOut)->pdg_id())==23&&(*itPartOut)->status()==2){
      if(abs((*itPartOut)->pdg_id())==443){

	//	std::cout << "Z-prime !!!" << std::endl;

	Iin[jpN] = (*itPartOut)->pdg_id();
        Psitemp_px[jpN] = (*itPartOut)->momentum().px(); 
        Psitemp_py[jpN] = (*itPartOut)->momentum().py(); 
        Psitemp_pz[jpN] = (*itPartOut)->momentum().pz(); 
        Psitemp_E[jpN] = (*itPartOut)->momentum().e();

        
	if(jpN<2){
	  jev[jpN] = (*itPartOut)->end_vertex()->barcode();
	}else{
	  std::cout << "More than two J/psi" << std::endl; 
	}

        ++jpN;

      }


      
      if(abs((*itPartOut)->pdg_id())==13&&(*itPartOut)->status()==1){

	// what is your parent ?  Is it the jpsi that came from the higgs 
	// Higgs id == 35
	// Z     id == 23
	//	std::cout << "muons ! " << std::endl;

	for(int ii=0; ii<2; ii++){

	  if( muN > 4 ){
	    std::cout << "More than 4 muons ! " << std::endl;
	    break;
	  }
	  mpv[muN] = (*itPartOut)->production_vertex()->barcode();

	  // find connection muon to z prime
	  

	  if(mpv[muN] == jev[ii]){
	  
	    std::cout << muN << " jev " << jev[ii] << std::endl;

	    Iout[muN] = (*itPartOut)->pdg_id();
	    Mu_px[muN] = (*itPartOut)->momentum().px(); 
	    Mu_py[muN] = (*itPartOut)->momentum().py(); 
	    Mu_pz[muN] = (*itPartOut)->momentum().pz(); 
	    Mu_E[muN] = (*itPartOut)->momentum().e();
	    Mu_charge[muN] = -(((*itPartOut)->pdg_id()>0)-((*itPartOut)->pdg_id()<0));

	    ++muN;
	  }

	}

      }

    }

  }

  //--- collect four muons - but are they the original ones?

  if (abs(Iout[0])==13&&abs(Iout[1])==13&&abs(Iout[2])==13&&abs(Iout[3])==13){

    //if (Iout[0]==443&&Iin[1]==443){
    //      std::cout<<Evt->event_number()<<" "<<Iin[0]<<" -> "<<Iout[0]<<" "<<Iout[1]<<std::endl;
    //std::cout<<Evt->event_number()<<" "<<Iin[1]<<" -> "<<Iout[2]<<" "<<Iout[3]<<std::endl;
    
    
    m0.SetPx(Mu_px[0]);m0.SetPy(Mu_py[0]);m0.SetPz(Mu_pz[0]);m0.SetE(Mu_E[0]);
    m1.SetPx(Mu_px[1]);m1.SetPy(Mu_py[1]);m1.SetPz(Mu_pz[1]);m1.SetE(Mu_E[1]);
    m2.SetPx(Mu_px[2]);m2.SetPy(Mu_py[2]);m2.SetPz(Mu_pz[2]);m2.SetE(Mu_E[2]);
    m3.SetPx(Mu_px[3]);m3.SetPy(Mu_py[3]);m3.SetPz(Mu_pz[3]);m3.SetE(Mu_E[3]);
    Mu_Eta[0] = m0.PseudoRapidity();
    Mu_Eta[1] = m1.PseudoRapidity();
    Mu_Eta[2] = m2.PseudoRapidity();
    Mu_Eta[3] = m3.PseudoRapidity();
    Mu_pT[0] = m0.Pt();
    Mu_pT[1] = m1.Pt();
    Mu_pT[2] = m2.Pt();
    Mu_pT[3] = m3.Pt();
    TLorentzVector p0 = m0+m1;
    TLorentzVector p1 = m2+m3;
    
    //-------------------------------------------------------

    for(int itemp=0; itemp<4; ++itemp){
      pTOrderI[itemp] = itemp;
    }
    int htemp = -1;
    for (int ione = 0; ione<4; ++ione){
      for (int itwo=ione+1; itwo<4; ++itwo){
	if (Mu_pT[pTOrderI[ione]]<Mu_pT[pTOrderI[itwo]]){
	  htemp =  pTOrderI[ione]; pTOrderI[ione]=pTOrderI[itwo]; pTOrderI[itwo] = htemp;
	}
      }
    }
    //p0.SetPx(Psitemp_px[0]);p0.SetPy(Psitemp_py[0]);p0.SetPz(Psitemp_pz[0]);p0.SetE(Psitemp_E[0]);
    //p1.SetPx(Psitemp_px[1]);p1.SetPy(Psitemp_py[1]);p1.SetPz(Psitemp_pz[1]);p1.SetE(Psitemp_E[1]);
    
    if( p0.Pt() >= p1.Pt()){

        Psi_px[0] = p0.Px();
        Psi_py[0] = p0.Py();
        Psi_pz[0] = p0.Pz();
        Psi_E[0] =  p0.E();
        Psi_px[1] = p1.Px();
        Psi_py[1] = p1.Py();
        Psi_pz[1] = p1.Pz();
        Psi_E[1] =  p1.E();
        Psi_Mass[0] = p0.M();
        Psi_Mass[1] = p1.M();
        Psi_y[0] = p0.Rapidity();
        Psi_y[1] = p1.Rapidity();
        Psi_pT[0] = p0.Pt();
        Psi_pT[1] = p1.Pt();

	// helicity angle calculation?
        TVector3 boosttoj0 = -(p0.BoostVector());
        m1.Boost(boosttoj0);
        TVector3 m13= m1.Vect();
        TVector3 p03= p0.Vect();
        TVector3 boosttoj1 = -(p1.BoostVector());
        m3.Boost(boosttoj1);
        TVector3 m33= m3.Vect();
        TVector3 p13= p1.Vect();

        Mu_Angle[0] = ((m13.Dot(p03))/((m13.Mag())*(p03.Mag())));
        Mu_Angle[1] = ((m33.Dot(p13))/((m33.Mag())*(p13.Mag())));

	// helicity angle!



      }
      else{
        Psi_px[1] = p0.Px();
        Psi_py[1] = p0.Py();
        Psi_pz[1] = p0.Pz();
        Psi_E[1] =  p0.E();
        Psi_px[0] = p1.Px();
        Psi_py[0] = p1.Py();
        Psi_pz[0] = p1.Pz();
        Psi_E[0] =  p1.E();
        Psi_Mass[1] = p0.M();
        Psi_Mass[0] = p1.M();
        Psi_y[1] = p0.Rapidity();
        Psi_y[0] = p1.Rapidity();
        Psi_pT[1] = p0.Pt();
        Psi_pT[0] = p1.Pt();
        TVector3 boosttoj0 = -(p0.BoostVector());
        m1.Boost(boosttoj0);
        TVector3 m13= m1.Vect();
        TVector3 p03= p0.Vect();
        TVector3 boosttoj1 = -(p1.BoostVector());
        m3.Boost(boosttoj1);
        TVector3 m33= m3.Vect();
        TVector3 p13= p1.Vect();

        Mu_Angle[1] = ((m13.Dot(p03))/((m13.Mag())*(p03.Mag())));
        Mu_Angle[0] = ((m33.Dot(p13))/((m33.Mag())*(p13.Mag())));
      }


      Psi_3dOpening = cos(p0.Angle(p1.Vect()));
      Psi1To2_dY = fabs(p0.Rapidity() - p1.Rapidity());

      m4m = p0+p1;
      FourMu_Mass = m4m.M();
      FourMu_Rapidity = m4m.Rapidity();
      FourMu_pT = m4m.Pt();

      // calculate the helicity angle

      Float_t temp;
      {      
	TLorentzVector particle = m0;
	TLorentzVector grandparent = m4m;
	
	TVector3 boosttoparent = -(p0.BoostVector());
	
	particle.Boost(boosttoparent);
	grandparent.Boost(boosttoparent);
	
	TVector3 particle3 = particle.Vect();
	TVector3 grandparent3 = grandparent.Vect();
	Float_t numerator = particle3.Dot(grandparent3);
	Float_t denominator = (particle3.Mag())*(grandparent3.Mag());
	
	temp = numerator/denominator;
      }
      Mu_Helicity[0] = temp;

      {
	TLorentzVector particle = m2;
	TLorentzVector grandparent = m4m;
	
	TVector3 boosttoparent = -(p1.BoostVector());
	particle.Boost(boosttoparent);
	grandparent.Boost(boosttoparent);
	
	TVector3 particle3 = particle.Vect();
	TVector3 grandparent3 = grandparent.Vect();
	Float_t numerator = particle3.Dot(grandparent3);
	Float_t denominator = (particle3.Mag())*(grandparent3.Mag());
	temp = numerator/denominator;
      }
      Mu_Helicity[1] = temp;

      //-------------

      ++accEvN;
      fTree->Fill();
      //std::cout<<"filled"<<std::endl;
    }

#ifdef THIS_IS_AN_EVENT_EXAMPLE
    Handle<ExampleData> pIn;
    iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
    ESHandle<SetupData> pSetup;
    iSetup.get<SetupRecord>().get(pSetup);
#endif
    }

    // ------------ method called once each job just before starting event loop  ------------
    void GensimAnalyzer::beginJob()
    {
      fFile = TFile::Open(fRootFileName.c_str(),"RECREATE");
      fFile->cd();
      fTree = new TTree("Events","Events");
      eventN = 0;
      accEvN= 0;
      fTree->Branch("eventN",         &eventN,              "eventN/i");
      fTree->Branch("FourMu_Mass",    &FourMu_Mass,         "FourMu_Mass/F");
      fTree->Branch("FourMu_Rapidity",&FourMu_Rapidity,     "FourMu_Rapidity/F");
      fTree->Branch("FourMu_pT",      &FourMu_pT,           "FourMu_pT/F");
      fTree->Branch("Psi1To2_dY",     &Psi1To2_dY,          "Psi1To2_dY/F");
      fTree->Branch("Psi_Mass",       Psi_Mass,             "Psi_Mass[2]/F");
      fTree->Branch("Psi_E",           Psi_E,                 "Psi_E[2]/F");
      fTree->Branch("Psi_px",          Psi_px,                "Psi_px[2]/F");
      fTree->Branch("Psi_py",          Psi_py,                "Psi_py[2]/F");
      fTree->Branch("Psi_pz",          Psi_pz,                "Psi_pz[2]/F");
      fTree->Branch("Psi_y",          Psi_y,                "Psi_y[2]/F");
      fTree->Branch("Psi_pT",         Psi_pT,               "Psi_pT[2]/F");
      fTree->Branch("Psi_3dOpening",  &Psi_3dOpening,        "Psi_3dOpening/F");
      fTree->Branch("Mu_Angle",       Mu_Angle,             "Mu_Angle[2]/F");
      fTree->Branch("Mu_Helicity",    Mu_Helicity,           "Mu_Helicity[2]/F");
      fTree->Branch("Mu_charge",      Mu_charge,            "Mu_charge[4]/I");
      fTree->Branch("Mu_Eta",         Mu_Eta,               "Mu_Eta[4]/F");
      fTree->Branch("Mu_E",           Mu_E,                 "Mu_E[4]/F");
      fTree->Branch("Mu_px",          Mu_px,                "Mu_px[4]/F");
      fTree->Branch("Mu_py",          Mu_py,                "Mu_py[4]/F");
      fTree->Branch("Mu_pz",          Mu_pz,                "Mu_pz[4]/F");
      fTree->Branch("Mu_pT",          Mu_pT,                "Mu_pT[4]/F");
      fTree->Branch("pTOrderI",       pTOrderI,             "pTOrderI[4]/I");
      fTree->Branch("Imom",       Imom,             "Imom[2][10]/I");

    }
    // ------------ method called once each job just after ending the event loop  ------------
    void 
      GensimAnalyzer::endJob() 
      {
        fFile->cd();
        fFile->Write();
        fFile->Close();
        delete fFile;
        std::cout << "==>PATEventTree> Succesfully ended job" << std::endl;
        std::cout << "Events Processed: "<<eventN<<" Accepted: "<<accEvN<<std::endl;
      }

    //// ------------ method called when starting to processes a run  ------------
    void 
      GensimAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
      {

      }

    // ------------ method called when ending the processing of a run  ------------
    void 
      GensimAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
      {
      }

    // ------------ method called when starting to processes a luminosity block  ------------
    void 
      GensimAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
      {
      }

    // ------------ method called when ending the processing of a luminosity block  ------------
    void 
      GensimAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
      {
      }

    //// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------

    //define this as a plug-in
    DEFINE_FWK_MODULE(GensimAnalyzer);
