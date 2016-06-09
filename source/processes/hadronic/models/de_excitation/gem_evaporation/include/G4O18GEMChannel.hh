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
//
// $Id: G4O18GEMChannel.hh,v 1.4 2009/09/15 12:54:16 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-03 $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Sept. 2001)
//


#ifndef G4O18GEMChannel_h
#define G4O18GEMChannel_h 1

#include "G4GEMChannel.hh"
#include "G4O18GEMCoulombBarrier.hh"
#include "G4O18GEMProbability.hh"

class G4O18GEMChannel : public G4GEMChannel
{
public:
  // only available constructor
  G4O18GEMChannel() : G4GEMChannel(18,8,"O18",
				   &theEvaporationProbability,
				   &theCoulombBarrier)
  {
    theEvaporationProbability.SetCoulomBarrier(&theCoulombBarrier);
  }
  
  // destructor
  ~G4O18GEMChannel() {};
  
private:
  const G4O18GEMChannel & operator=(const G4O18GEMChannel & right);  
    
  G4O18GEMChannel(const G4O18GEMChannel & right);
  
public:
  G4bool operator==(const G4O18GEMChannel & right) const;
  G4bool operator!=(const G4O18GEMChannel & right) const;
    
private:
  
  G4O18GEMCoulombBarrier theCoulombBarrier;
	
  G4O18GEMProbability theEvaporationProbability;
  
};
#endif

