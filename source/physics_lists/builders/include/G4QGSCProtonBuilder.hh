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
// $Id: G4QGSCProtonBuilder.hh,v 1.4 2009/03/31 11:04:01 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-03 $
//
//---------------------------------------------------------------------------
//
// ClassName:   G4QGSCProtonBuilder
//
// Author: 2002 J.P. Wellisch
//
// Modified:
// 30.03.2009 V.Ivanchenko create cross section by new
//
//----------------------------------------------------------------------------
//
#ifndef G4QGSCProtonBuilder_h
#define G4QGSCProtonBuilder_h 

#include "globals.hh"

#include "G4HadronElasticProcess.hh"
#include "G4HadronFissionProcess.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4ProtonInelasticProcess.hh"
#include "G4VProtonBuilder.hh"

#include "G4NeutronInelasticCrossSection.hh"
#include "G4TheoFSGenerator.hh"
#include "G4QStringChipsParticleLevelInterface.hh"
#include "G4QGSModel.hh"
#include "G4QGSParticipants.hh"
#include "G4QGSMFragmentation.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4QuasiElasticChannel.hh"

class G4QGSCProtonBuilder : public G4VProtonBuilder
{
  public: 
    G4QGSCProtonBuilder(G4bool quasiElastic=false);
    virtual ~G4QGSCProtonBuilder();

  public: 
    virtual void Build(G4HadronElasticProcess * aP);
    virtual void Build(G4ProtonInelasticProcess * aP);
    
    void SetMinEnergy(G4double aM) {theMin = aM;}

  private:

    G4TheoFSGenerator * theModel;
    G4QStringChipsParticleLevelInterface * theCascade;
    G4QGSModel< G4QGSParticipants > * theStringModel;
    G4ExcitedStringDecay * theStringDecay;
    G4QuasiElasticChannel * theQuasiElastic;

    G4double theMin;

};

// 2002 by J.P. Wellisch

#endif

