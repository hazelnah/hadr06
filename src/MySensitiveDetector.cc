#include "MySensitiveDetector.hh"
#include "Run.hh"

const bool gDebug = false;

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{

}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    if (gDebug) G4cout << "Detector::ProcessHits start "<< G4endl;
    G4Track *track = aStep->GetTrack();
    G4ParticleDefinition* particle = track->GetDefinition();
    track->SetTrackStatus(fStopAndKill);
    if (gDebug) G4cout << "Detector::ProcessHits 1"<< G4endl;
    
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
    
    G4ThreeVector posPart = preStepPoint->GetPosition();
    G4ThreeVector momPart = preStepPoint->GetMomentum();
    
    // G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03;
    
    if (gDebug) G4cout << "Photon position: " << posPart << G4endl;
    if (gDebug) G4cout << "Detector::ProcessHits 2 "<< G4endl;
    
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    
    G4int copyNo = touchable->GetCopyNumber();
    // G4cout << "Copy number: " << copyNo << G4endl;
    
    if (gDebug) G4cout << "Detector::ProcessHits 3 "<< G4endl;
    
    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();
    
    if (gDebug) G4cout << "Detector::ProcessHits 4 "<< G4endl;
        
    const G4Event * event = G4RunManager::GetRunManager()->GetCurrentEvent();
    G4int evt = event->GetEventID();

    auto run = static_cast<Run*>
        (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    // G4double hitTime=run->GetEventTime() + preStepPoint->GetGlobalTime();
    G4double hitTime=run->GetEventTime()/ps + track->GetLocalTime()/ps;
    
    G4AnalysisManager *analysis = G4AnalysisManager::Instance();
    if (gDebug) G4cout << "Detector::ProcessHits 5 "<< G4endl;
    // G4cout << "EventTime: " << run->GetEventTime()/ps << " ps, " << ps << " ,"
    //        << "LocalTime: " << track->GetLocalTime()/ps << " ps.  "<< ns << " ," << G4endl;

    analysis->FillNtupleIColumn(0, 0, evt);
    analysis->FillNtupleDColumn(0, 1, posPart[0]);
    analysis->FillNtupleDColumn(0, 2, posPart[1]);
    analysis->FillNtupleDColumn(0, 3, posPart[2]);
    analysis->FillNtupleDColumn(0, 4, preStepPoint->GetKineticEnergy());
    analysis->FillNtupleDColumn(0, 5, preStepPoint->GetCharge());
    analysis->FillNtupleDColumn(0, 6, preStepPoint->GetMass());
    analysis->FillNtupleSColumn(0, 7, particle->GetParticleName());
    analysis->FillNtupleDColumn(0, 8, posDetector[0]);
    analysis->FillNtupleDColumn(0, 9, posDetector[1]);
    analysis->FillNtupleDColumn(0, 10, posDetector[2]);
    analysis->FillNtupleDColumn(0, 11, hitTime);
    analysis->AddNtupleRow(0);
    
    //if (track->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay")
    if (0)
    {
        if (gDebug) G4cout << " ~~~~~ Detector::ProcessHits 6 "<< G4endl;
        // G4cout << " ~ GetCurrentStepNumber: " << track->GetCurrentStepNumber() << G4endl;
        // G4cout << " ~ GetCreatorModelName: " << track->GetCreatorModelName() << G4endl;
        // G4cout << " ~ GetParentID: " << track->GetParentID() << G4endl;
        // G4cout << " ~ Position: x: " << track->GetVertexPosition()[0] << " y: " << track->GetVertexPosition()[1] << " z: " << track->GetVertexPosition()[2] << G4endl;
        // G4cout << "~~ Event: N of pr vert: " << event->GetNumberOfPrimaryVertex() << G4endl;
        // G4cout << "x: " << event->GetPrimaryVertex(0)->GetX0()<< "y: " << event->GetPrimaryVertex(0)->GetY0()<< "z: " << event->GetPrimaryVertex(0)->GetZ0()<< G4endl;

        analysis->FillNtupleIColumn(1, 0, evt);
        analysis->FillNtupleDColumn(1, 1, track->GetVertexPosition()[0]);
        analysis->FillNtupleDColumn(1, 2, track->GetVertexPosition()[2]);
        analysis->FillNtupleDColumn(1, 3, track->GetVertexPosition()[3]);
        
        analysis->AddNtupleRow(1);

/*        if ( track -> GetCurrentStepNumber() == 1 && track->GetParentID()==0 ) 
        {
            man->FillNtupleDColumn(2, 0, evtID);
            man->FillNtupleDColumn(2, 1, event->GetPrimaryVertex(0)->GetX0());
            man->FillNtupleDColumn(2, 2, event->GetPrimaryVertex(0)->GetY0());
            man->FillNtupleDColumn(2, 3, event->GetPrimaryVertex(0)->GetZ0());

            man->AddNtupleRow(2);
        }*/
    }
    // else 
    // {
    //     if (gDebug) G4cout << " ~~~~~ Not a decay!" << track->GetCreatorProcess()->GetProcessName() << G4endl;
    // }


    if (gDebug) G4cout << "Detector::ProcessHits 7 "<< G4endl;
    
    if (gDebug) G4cout << "Detector::ProcessHits end "<< G4endl;
    return true;
}
