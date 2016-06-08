// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: Em4PrimaryGeneratorAction.hh,v 1.1.4.1 1999/12/07 20:47:05 gunter Exp $
// GEANT4 tag $Name: geant4-01-00 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef Em4PrimaryGeneratorAction_h
#define Em4PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class Em4PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    Em4PrimaryGeneratorAction();    
   ~Em4PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);

  private:
    G4ParticleGun*  particleGun;	//pointer a to G4 service class
};

#endif

