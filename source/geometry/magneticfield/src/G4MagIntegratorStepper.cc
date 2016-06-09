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
// $Id: G4MagIntegratorStepper.cc,v 1.12 2009/11/05 18:31:15 japost Exp $
// GEANT4 tag $Name: geant4-09-03 $
//
// --------------------------------------------------------------------

#include "G4MagIntegratorStepper.hh"

// Constructor for stepper abstract base class. 
// 

G4MagIntegratorStepper::G4MagIntegratorStepper(G4EquationOfMotion* Equation,
					       G4int       num_integration_vars,
					       G4int       num_state_vars)
  : fEquation_Rhs(Equation),
    fNoIntegrationVariables(num_integration_vars),
    fNoStateVariables(num_state_vars)
    // fNumberOfVariables( std::max(num_var,fNoStateVariables) )
{
}

G4MagIntegratorStepper::~G4MagIntegratorStepper()
{
}

void G4MagIntegratorStepper::ComputeRightHandSide( const G4double y[], G4double dydx[] ) 
{
  this->RightHandSide( y, dydx );
}
