#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

using namespace TMVA;

void regression_application(TString myMethodList = "")
{
   // Load the TMVA library
   TMVA::Tools::Instance();

   // Default MVA methods to be tested
   std::map<std::string,int> Use;

   // Select methods to use (same as in training)
   Use["PDEFoam"] = 1;
   Use["KNN"] = 1;
   Use["LD"] = 1;
   Use["BDTG"] = 1;

   std::cout << std::endl;
   std::cout << "==> Start regression application" << std::endl;

   // Select methods if specified
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = gTools().SplitString(myMethodList, ',');
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);
         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name." << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // Create the Reader object
   TMVA::Reader *reader = new TMVA::Reader("!Color:!Silent");

   // Declare variables to the reader (must match training)
   Float_t fHitX, fHitY, fHitT;
   reader->AddVariable("fHitX", &fHitX);
   reader->AddVariable("fHitY", &fHitY);
   reader->AddVariable("fHitT", &fHitT);

   // Book the MVA methods
   TString dir = "datasetreg/weights/";
   TString prefix = "TMVARegression";

   // Book method(s)
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = it->first + " method";
         TString weightfile = dir + prefix + "_" + TString(it->first) + ".weights.xml";
         reader->BookMVA(methodName, weightfile);
      }
   }

   // Book output histograms
   TH1* hists[100];
   Int_t nhists = -1;
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      TH1* h = new TH1F(it->first.c_str(), TString(it->first) + " method", 100, -100, 600);
      if (it->second) hists[++nhists] = h;
   }
   nhists++;

   // Open input file
   TFile *input(0);
   TString fname = "./build/output0.root";
   if (!gSystem->AccessPathName(fname)) {
      input = TFile::Open(fname);
   }
   else {
      std::cout << "ERROR: could not open data file: " << fname << std::endl;
      exit(1);
   }
   std::cout << "--- Using input file: " << input->GetName() << std::endl;

   // Get the tree
   TTree* theTree = (TTree*)input->Get("Particles");
   std::cout << "--- Select sample" << std::endl;
   theTree->SetBranchAddress("fHitX", &fHitX);
   theTree->SetBranchAddress("fHitY", &fHitY);
   theTree->SetBranchAddress("fHitT", &fHitT);

   // Process events
   std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
   TStopwatch sw;
   sw.Start();
   for (Long64_t ievt=0; ievt<theTree->GetEntries(); ievt++) {
      if (ievt%1000 == 0) {
         std::cout << "--- ... Processing event: " << ievt << std::endl;
      }

      theTree->GetEntry(ievt);

      // Evaluate regression and fill histograms
      for (Int_t ih=0; ih<nhists; ih++) {
         TString title = hists[ih]->GetTitle();
         Float_t val = (reader->EvaluateRegression(title))[0];
         hists[ih]->Fill(val);
      }
   }
   sw.Stop();
   std::cout << "--- End of event loop: "; sw.Print();

   // Write histograms
   TFile *target = new TFile("TMVARegApp.root", "RECREATE");
   for (Int_t ih=0; ih<nhists; ih++) hists[ih]->Write();
   target->Close();

   std::cout << "--- Created root file: \"" << target->GetName()
             << "\" containing the MVA output histograms" << std::endl;

   delete reader;
   std::cout << "==> Regression application is done!" << std::endl << std::endl;
}

int main(int argc, char** argv)
{
   TString methodList;
   for (int i=1; i<argc; i++) {
      TString regMethod(argv[i]);
      if(regMethod=="-b" || regMethod=="--batch") continue;
      if (!methodList.IsNull()) methodList += TString(",");
      methodList += regMethod;
   }
   regression_application(methodList);
   return 0;
} 