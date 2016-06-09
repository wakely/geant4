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
// $Id: G4FissionLevelDensityParameter.cc,v 1.7 2009/11/21 18:05:26 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-03 $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Oct 1998)
//
// J.M.Quesada (July 2009):  fission level density parameter tuned for spallation data.
// V.Ivanchenko (July 2009): fixed logic and remove wrong usage of MeV
// J.M.Quesada (30.10.09):   retuning for IAEA spallation data

#include "G4FissionLevelDensityParameter.hh"
#include "G4HadronicException.hh"


G4FissionLevelDensityParameter::
G4FissionLevelDensityParameter(const G4FissionLevelDensityParameter &) : G4VLevelDensityParameter()
{
    throw G4HadronicException(__FILE__, __LINE__, "G4FissionLevelDensityParameter::copy_constructor meant to not be accessable");
}


const G4FissionLevelDensityParameter & G4FissionLevelDensityParameter::
operator=(const G4FissionLevelDensityParameter &)
{
    throw G4HadronicException(__FILE__, __LINE__, "G4FissionLevelDensityParameter::operator= meant to not be accessable");
    return *this;
}


G4bool G4FissionLevelDensityParameter::
operator==(const G4FissionLevelDensityParameter &) const
{
    return false;
}

G4bool G4FissionLevelDensityParameter::
operator!=(const G4FissionLevelDensityParameter &) const
{
    return true;
}


G4double G4FissionLevelDensityParameter::
LevelDensityParameter(const G4int A,const G4int Z,const G4double U) const 
{
  G4double EvapLDP = 
    theEvaporationLevelDensityParameter.LevelDensityParameter(A,Z,U);

  if(Z >= 89)      { EvapLDP *= 1.02; }
  else if(Z >= 85) { EvapLDP *= (1.02 + 0.004*(89 - Z)); }
  else             { EvapLDP *= 1.04; }

  /*
  if(Z >= 89)      { EvapLDP *= 1.01; }
  else if(Z >= 85) { EvapLDP *= (1.01 + 0.002*(89 - Z)); }
  else             { EvapLDP *= 1.02; }
  */
  return EvapLDP;

  /*
  if(Z >= 89)      EvapLDP *= 1.04;
  else if(Z >= 85) EvapLDP *= (1.04 + 0.01*(89 - Z));
  else             EvapLDP *= 1.09;

  //JMQ 310509 
  return 1.07*EvapLDP;
  */
}
