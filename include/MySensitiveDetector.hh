#ifndef MySensitiveDetector_h
#define MySensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicsOrderedFreeVector.hh"

#include "G4AnalysisManager.hh"

extern const bool gDebug;

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    MySensitiveDetector(G4String);
    ~MySensitiveDetector();
    
private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};

#endif
