#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TString.h>
#include <iostream>

void prepareData() {

TFile *file = TFile::Open("./build/Hadr06_neutrons.root");
if (!file || file->IsZombie()) {
    std::cout << "Error opening file Hadr06_neutrons.root" << std::endl;
} else {
    TTree *tree = (TTree*)file->Get("Particles");
    TTree *sourceTree = (TTree*)file->Get("Source");
    if (!tree || !sourceTree) {
        if (!tree)
        std::cout << "Particles tree not found in file." << std::endl;
        if (!sourceTree)
        std::cout << "Source tree not found in file." << std::endl;
    } else {
        Long64_t nentries = tree->GetEntries();
        Long64_t nSourceEntries = sourceTree ? sourceTree->GetEntries() : 0;
        // Create output file and output tree
        TFile *outFile = new TFile("./build/Particles_data.root", "RECREATE");
        // Clone the structure of the Particles tree, but with zero entries
        TTree *outTree = tree->CloneTree(0);
        // Add a new branch for fVrtxZ (double)
        double fVrtxZ = 0.0;
        TBranch *vrtxZBranch = outTree->Branch("fVrtxZ", &fVrtxZ, "fVrtxZ/D");
        
        static int fEv_particles = 0;
        static int fEv_source = 0;
        double fZ = 0.0;
        
        // Set up to read fEv from both trees
        tree->SetBranchAddress("fEvent", &fEv_particles);
        sourceTree->SetBranchAddress("fEvent", &fEv_source);
        sourceTree->SetBranchAddress("fZ", &fZ);
        
        // Swap the "for" cycles: outer loop over Source, inner loop over Particles
/*
for (int j = 0; j < nSourceEntries; ++j) {
    sourceTree->GetEntry(j);
    for (int i = 0; i < nentries; ++i) {
        tree->GetEntry(i);
        if (fEv_source == fEv_particles) {
            fVrtxZ = fZ;
            outTree->Fill();
            // If only the first match is needed, uncomment the next line:
            // break;
        }
    }
}
*/
        // Print status every 100 entries processed
        for (int i = 0; i < nentries; ++i) {
            if (i % 100 == 0) {
                std::cout << "Processing entry " << i << " / " << nentries << std::endl;
            }
            tree->GetEntry(i);
            TString str = TString::Format("fEvent == %d", fEv_particles);
            sourceTree->Draw("fZ", str,"goff");
            
            double* vrtxes = sourceTree->GetV1();
            long selRows = sourceTree->GetSelectedRows();
            if(selRows == 1)
            {
                fVrtxZ = vrtxes[0];
                outTree->Fill();
            }
            else std::cout << "More than 1 row." << std::endl;
        }

        outTree->Write();
        outFile->Close();
        delete outFile;
    }
    file->Close();
    delete file;
    }
}
