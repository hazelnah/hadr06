#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>

void filter() {
    TFile *f = new TFile("Hadr06.root");
    TTree *tP = (TTree*)f->Get("Particles");
    TTree *tS = (TTree*)f->Get("Source");
    TFile *fout = new TFile("Hadr06_neutrons.root", "RECREATE");
    TTree *tP_neutrons = tP->CopyTree("fName==\"neutron\"");
    TTree *tS_neutrons = tS->CopyTree("");
    tP_neutrons->Write("Particles");
    tS_neutrons->Write("Source");
    fout->Close();
}







