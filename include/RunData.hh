#ifndef RunData_h
#define RunData_h 1

#include "G4Run.hh"
#include "globals.hh"
#include "Randomize.hh"

class RunData : public G4Run
{
public:
	RunData() = default;
	~RunData() = default;

	void SetEventTime(G4double t);
	G4double CalcEventTime();
	G4double GetEventTime();

private:
	G4double t0 = 0.;
};

inline void RunData::SetEventTime(G4double t){t0 = t;}

inline G4double RunData::CalcEventTime(){
	t0=t0+G4RandGauss::shoot(1e-5, 5e-6); // gaussian dT with sigma = 1e-5 sec
  return t0;
}

inline G4double RunData::GetEventTime() {return t0;}

#endif