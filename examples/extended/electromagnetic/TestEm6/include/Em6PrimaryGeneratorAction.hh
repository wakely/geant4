// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// Class Description:
// Inicialisation of particle gun.
// Class Description - end
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef Em6PrimaryGeneratorAction_h
#define Em6PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class Em6DetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class Em6PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public: // Without description

    Em6PrimaryGeneratorAction(Em6DetectorConstruction*);    
   ~Em6PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event*);
    static G4String GetPrimaryName() ;                

  private:
    G4ParticleGun*                particleGun;
    Em6DetectorConstruction*      Em6Detector;
    
    static G4String thePrimaryParticleName;
};

#endif

