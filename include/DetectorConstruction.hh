//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4Material;
class DetectorMessenger;
class G4PVPlacement;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction() override;

  public:
  
    G4VPhysicalVolume* Construct() override;

    G4Material* 
    MaterialWithSingleIsotope(G4String, G4String, G4double, G4int, G4int);
         
    void SetRadius   (G4double);              
    void SetWorldSize(G4double);
    void SetWallSize (G4double);
    void SetMaterial (G4String);

  public:  
                    
     G4double           GetRadius()     {return fRadius;};      
     G4Material*        GetMaterial()   {return fMaterial;};
                   
     void               PrintParameters();
                       
  private:
     
     G4double           fRadius = 0.;
     G4Material*        fMaterial = nullptr;
     G4LogicalVolume*   fLAbsor = nullptr;
     
     G4double           fWorldSize = 0.;
     G4Material*        fWorldMat = nullptr;
     G4VPhysicalVolume* fPWorld = nullptr;
                             
     G4LogicalVolume*   logicDetector = nullptr;
     G4double           fDetectorSizeX = 40.*cm;
     G4double           fDetectorSizeY = 40.*cm;
     G4double           fDetectorSizeZ = 2*cm;
     G4int              fDetectorRows = 20;
     G4int              fDetectorCols = 20;

     DetectorMessenger* fDetectorMessenger = nullptr;

     virtual void ConstructSDandField();

  private:
    
     void               DefineMaterials();
     G4VPhysicalVolume* ConstructVolumes();
     void               CreateDetectorWall(G4LogicalVolume* logicWorld);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif

