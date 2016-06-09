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
// $Id: G4SimpleMaterialStoppingICRU73.cc,v 1.9 2009/11/09 16:51:07 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-03 $

//---------------------------------------------------------------------------
//
// GEANT4 Class file
//
// Description: Data on stopping power
//
// Author:      A.Ivantchenko 07.08.2008
//
// in the framework of the ESA Technology Research Programme
// (ESA contract 21435/08/NL/AT)
//
// Modifications:
// 29.04.2009 A.Ivantchenko added revised data for G4_Cu
//            provided by  Prof.P.Sigmund Univ. Southern Denmark
// 03.11.2009 A. Lechner: Added new methods BuildPhysicsVector according
//            to interface changes in base class G4VIonDEDXTable.
//
//----------------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4SimpleMaterialStoppingICRU73.hh"
#include "G4LPhysicsFreeVector.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SimpleMaterialStoppingICRU73::G4SimpleMaterialStoppingICRU73(G4bool flag) 
{
  spline = flag;
  minIonAtomicNmb = 3;
  maxIonAtomicNmb = 18;

  G4int matZ[25] = {1,2,4,6,7,8,10,13,14,18,22,26,28,29,32,36,42,47,50,54,74,78,79,82,92};

  G4String matName[25] = {"G4_H", "G4_He", "G4_Be", "G4_C", "G4_N", "G4_O", "G4_Ne", "G4_Al", "G4_Si", "G4_Ar", "G4_Ti", "G4_Fe", "G4_Ni", "G4_Cu", "G4_Ge", "G4_Kr", "G4_Mo", "G4_Ag", "G4_Sn", "G4_Xe", "G4_W", "G4_Pt", "G4_Au", "G4_Pb", "G4_U"};

  for( size_t i = 0; i < 25; i++ ) {
      atomicNumbersMat.push_back( matZ[i] );
      namesMat.push_back( matName[i] );
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool G4SimpleMaterialStoppingICRU73::BuildPhysicsVector(G4int ionZ, 
                                                          G4int matZ) 
{
  return IsApplicable( ionZ, matZ );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4SimpleMaterialStoppingICRU73::BuildPhysicsVector(G4int ionZ, 
                                                       const G4String& matName) 
{
  return IsApplicable( ionZ, matName );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool G4SimpleMaterialStoppingICRU73::IsApplicable(G4int ionZ, 
                                             G4int matZ)
{
  G4bool isApplicable = false;

  if(ionZ >= minIonAtomicNmb && ionZ <= maxIonAtomicNmb) {

     size_t nmbMat =  atomicNumbersMat.size();

     for( size_t i = 0; i < nmbMat; i++ ) {

        if( atomicNumbersMat[i] == matZ ) { isApplicable = true; break; }
     }
  }

  return isApplicable;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4SimpleMaterialStoppingICRU73::IsApplicable(G4int ionZ, 
					     const G4String& matName)
{
  G4bool isApplicable = false;

  if(ionZ >= minIonAtomicNmb && ionZ <= maxIonAtomicNmb) {

     size_t nmbMat =  namesMat.size();

     for( size_t i = 0; i < nmbMat; i++ ) {

         if( namesMat[i] == matName ) { isApplicable = true; break; }
     }
  }

  return isApplicable;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SimpleMaterialStoppingICRU73::~G4SimpleMaterialStoppingICRU73()
{
  size_t nmb = dedx.size();

  for(size_t i = 0; i < nmb; i++) delete dedx[i];

  dedx.clear();
  dedxKeys.clear();
  atomicNumbersMat.clear();
  namesMat.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4SimpleMaterialStoppingICRU73::GetDEDX(G4double kinEnergyPerNucleon,
                                                 G4int ionZ,
                                                 G4int matZ) {

  G4double dedxValue = -1.0;

  size_t nmbMat =  atomicNumbersMat.size();

  for( size_t i = 0; i < nmbMat; i++ ) {

     if( atomicNumbersMat[i] == matZ ) { 
        dedxValue = GetDEDX(kinEnergyPerNucleon, ionZ, namesMat[i]);
        break; 
     }
  }

  return dedxValue;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4SimpleMaterialStoppingICRU73::GetDEDX(G4double kinEnergyPerNucleon,
                                                 G4int ionZ,
                                                 const G4String& matName)
{
  G4double dedxValue = -1.0;

  G4PhysicsVector* physicsVector = GetPhysicsVector(ionZ, matName); 
 
  if(physicsVector != 0) {

     size_t nmbBins = physicsVector -> GetVectorLength();
     G4double lowerEnergyEdge = physicsVector -> GetLowEdgeEnergy(0);
     G4double upperEnergyEdge = physicsVector -> GetLowEdgeEnergy(nmbBins-1);

     if(kinEnergyPerNucleon <= upperEnergyEdge) {
     	  G4bool b;
 
        if(kinEnergyPerNucleon < lowerEnergyEdge) {
           dedxValue = physicsVector -> GetValue(lowerEnergyEdge, b) *
     	                 std::sqrt(kinEnergyPerNucleon / lowerEnergyEdge);
        }
        else {
           dedxValue = physicsVector -> GetValue(kinEnergyPerNucleon, b);
        }
     }
  }  

  return dedxValue;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4PhysicsVector* G4SimpleMaterialStoppingICRU73::CreatePhysicsVector(
                                             G4double* energy, 
		                    	     G4double* stoppower, 
					     G4double factor)
{
  G4LPhysicsFreeVector* pv = new G4LPhysicsFreeVector(31,energy[0],energy[30]);
  pv->SetSpline(spline);

  for( G4int i = 0; i < 31; i++ ) {
    pv->PutValues(i,energy[i],stoppower[i]*factor);
  }

  return pv;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4PhysicsVector* G4SimpleMaterialStoppingICRU73::GetPhysicsVector(
                                           G4int ionZ, 
					   const G4String& matName)
{
  G4PhysicsVector* physicsVector = 0;

  DEDXKey key = std::make_pair(ionZ, matName);

  // Checking if dE/dx vector was already created
  size_t nmbKeys = dedxKeys.size();
  for( size_t i = 0; i < nmbKeys; i++ ) {

    if( dedxKeys[i] == key ) { physicsVector = dedx[i]; break; }
  }
  if(physicsVector != 0) { return physicsVector; }

  // Creating dE/dx vector if ion-material pair covered
  G4double factor = MeV * cm2 / (0.001 * g);

  G4double E[31] = {.025,.03,.04,.05,.06,.07,.08,.09,.1,.15,.2,.25,.3,.4,.5,.6,.7,.8,.9,1,1.5,2,2.5,3,4,5,6,7,8,9,10};

  for( G4int i = 0; i < 31; i++ ) E[i] *= MeV;

  // Data name: (Z_NameMaterial)_(Z_Ion)[31]=
  if( matName == namesMat[0] ) {
     if( ionZ == 3 ) {
        G4double H_3[31]={8.31,8.919,9.838,10.46,10.87,11.14,11.3,11.4,11.43,11.21,10.73,10.21,9.696,8.773,7.989,7.326,6.761,6.275,5.853,5.485,4.175,3.377,2.843,2.46,1.948,1.62,1.393,1.225,1.095,0.9921,0.9082};
        physicsVector = CreatePhysicsVector(E,H_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double H_4[31]={10.5,11.36,12.72,13.72,14.44,14.96,15.33,15.58,15.76,15.9,15.53,15.01,14.46,13.38,12.41,11.55,10.79,10.12,9.533,9.006,7.054,5.801,4.933,4.297,3.427,2.862,2.464,2.169,1.941,1.759,1.61};
        physicsVector = CreatePhysicsVector(E,H_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double H_5[31]={12.19,13.29,15.12,16.52,17.59,18.4,19.01,19.47,19.81,20.46,20.31,19.88,19.34,18.2,17.1,16.1,15.2,14.38,13.65,12.98,10.42,8.704,7.478,6.56,5.278,4.428,3.823,3.371,3.019,2.738,2.509};
        physicsVector = CreatePhysicsVector(E,H_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double H_6[31]={13.42,14.75,17,18.81,20.25,21.38,22.27,22.96,23.5,24.79,24.94,24.64,24.17,23.04,21.88,20.78,19.77,18.84,17.98,17.2,14.1,11.95,10.37,9.165,7.449,6.286,5.448,4.815,4.321,3.923,3.597};
        physicsVector = CreatePhysicsVector(E,H_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double H_7[31]={14.65,16.14,18.73,20.87,22.62,24.05,25.2,26.14,26.89,28.88,29.39,29.29,28.92,27.86,26.68,25.53,24.44,23.42,22.47,21.59,18.02,15.47,13.55,12.06,9.897,8.406,7.316,6.486,5.831,5.303,4.867};
        physicsVector = CreatePhysicsVector(E,H_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double H_8[31]={15.64,17.28,20.18,22.63,24.7,26.42,27.86,29.04,30.01,32.8,33.74,33.89,33.67,32.74,31.6,30.43,29.28,28.2,27.17,26.21,22.22,19.28,17.02,15.25,12.63,10.79,9.426,8.38,7.55,6.877,6.319};
        physicsVector = CreatePhysicsVector(E,H_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double H_9[31]={16.24,18,21.15,23.88,26.22,28.23,29.92,31.35,32.54,36.14,37.55,37.97,37.91,37.16,36.08,34.91,33.75,32.63,31.55,30.54,26.25,23.01,20.48,18.46,15.44,13.29,11.67,10.42,9.415,8.595,7.913};
        physicsVector = CreatePhysicsVector(E,H_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double H_10[31]={16.75,18.6,21.96,24.92,27.51,29.77,31.71,33.37,34.79,39.25,41.16,41.89,42.03,41.48,40.49,39.36,38.19,37.05,35.94,34.89,30.34,26.83,24.06,21.82,18.41,15.94,14.08,12.61,11.43,10.46,9.649};
        physicsVector = CreatePhysicsVector(E,H_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double H_11[31]={17.23,19.15,22.67,25.81,28.63,31.11,33.29,35.19,36.84,42.23,44.76,45.89,46.29,46.05,45.2,44.14,43,41.85,40.73,39.64,34.85,31.06,28.02,25.53,21.7,18.89,16.74,15.04,13.66,12.53,11.57};
        physicsVector = CreatePhysicsVector(E,H_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double H_12[31]={18,19.94,23.52,26.77,29.72,32.37,34.73,36.82,38.65,44.89,48.02,49.56,50.24,50.33,49.67,48.7,47.6,46.47,45.34,44.23,39.25,35.23,31.96,29.25,25.04,21.91,19.5,17.58,16.01,14.71,13.61};
        physicsVector = CreatePhysicsVector(E,H_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double H_13[31]={18.67,20.65,24.32,27.67,30.74,33.53,36.06,38.32,40.33,47.38,51.11,53.06,54.04,54.51,54.05,53.2,52.17,51.07,49.94,48.83,43.71,39.47,36,33.09,28.51,25.07,22.39,20.25,18.49,17.02,15.77};
        physicsVector = CreatePhysicsVector(E,H_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double H_14[31]={19.33,21.35,25.1,28.53,31.69,34.6,37.26,39.67,41.83,49.62,53.93,56.29,57.56,58.4,58.15,57.43,56.47,55.41,54.31,53.2,47.99,43.59,39.93,36.85,31.95,28.23,25.32,22.96,21.02,19.39,18.01};
        physicsVector = CreatePhysicsVector(E,H_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double H_15[31]={20.27,22.34,26.16,29.67,32.92,35.93,38.7,41.24,43.54,52.03,56.89,59.67,61.24,62.49,62.48,61.9,61.04,60.04,58.96,57.86,52.58,48.02,44.17,40.89,35.65,31.64,28.46,25.88,23.75,21.95,20.41};
        physicsVector = CreatePhysicsVector(E,H_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double H_16[31]={20.94,23.07,26.99,30.59,33.94,37.06,39.96,42.63,45.08,54.29,59.75,62.97,64.88,66.56,66.82,66.41,65.66,64.72,63.69,62.62,57.29,52.58,48.55,45.1,39.52,35.2,31.76,28.96,26.62,24.64,22.95};
        physicsVector = CreatePhysicsVector(E,H_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double H_17[31]={21.89,24.05,28,31.64,35.03,38.21,41.18,43.94,46.49,56.31,62.31,65.95,68.16,70.25,70.76,70.52,69.87,69.01,68.02,66.97,61.64,56.83,52.67,49.08,43.22,38.65,34.98,31.98,29.46,27.33,25.49};
        physicsVector = CreatePhysicsVector(E,H_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double H_18[31]={22.3,24.5,28.52,32.2,35.64,38.87,41.91,44.76,47.41,57.82,64.39,68.46,71,73.53,74.31,74.25,73.72,72.94,72.01,71,65.7,60.83,56.57,52.86,46.78,41.99,38.13,34.95,32.28,30,28.04};
        physicsVector = CreatePhysicsVector(E,H_18,factor);
     }
  }

  else if( matName == namesMat[1] ) {
     if( ionZ == 3 ) {
        G4double He_3[31]={2.636,2.851,3.202,3.47,3.676,3.832,3.948,4.035,4.097,4.194,4.123,3.996,3.85,3.558,3.29,3.052,2.842,2.656,2.492,2.346,1.812,1.476,1.247,1.082,0.8605,0.7182,0.6189,0.5455,0.4889,0.4437,0.4068};
        physicsVector = CreatePhysicsVector(E,He_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double He_4[31]={3.352,3.645,4.14,4.539,4.862,5.122,5.33,5.495,5.626,5.943,5.972,5.886,5.753,5.439,5.122,4.824,4.55,4.302,4.077,3.871,3.08,2.553,2.179,1.903,1.522,1.274,1.098,0.9684,0.8679,0.7877,0.7222};
        physicsVector = CreatePhysicsVector(E,He_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double He_5[31]={3.917,4.284,4.921,5.452,5.897,6.268,6.577,6.832,7.043,7.634,7.806,7.794,7.697,7.402,7.067,6.735,6.419,6.123,5.848,5.592,4.569,3.85,3.323,2.923,2.358,1.981,1.712,1.511,1.354,1.229,1.127};
        physicsVector = CreatePhysicsVector(E,He_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double He_6[31]={4.345,4.779,5.548,6.207,6.774,7.26,7.675,8.027,8.325,9.235,9.584,9.671,9.631,9.382,9.051,8.703,8.36,8.029,7.717,7.423,6.202,5.306,4.629,4.104,3.344,2.826,2.45,2.166,1.944,1.766,1.62};
        physicsVector = CreatePhysicsVector(E,He_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double He_7[31]={4.78,5.263,6.129,6.889,7.557,8.144,8.656,9.102,9.487,10.73,11.29,11.5,11.54,11.36,11.05,10.7,10.35,9.995,9.655,9.331,7.944,6.887,6.069,5.42,4.463,3.795,3.304,2.929,2.634,2.395,2.199};
        physicsVector = CreatePhysicsVector(E,He_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double He_8[31]={5.133,5.662,6.619,7.473,8.238,8.921,9.528,10.07,10.54,12.15,12.93,13.29,13.42,13.36,13.1,12.76,12.4,12.04,11.68,11.34,9.814,8.61,7.654,6.882,5.721,4.894,4.278,3.802,3.425,3.119,2.865};
        physicsVector = CreatePhysicsVector(E,He_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double He_9[31]={5.37,5.934,6.966,7.897,8.742,9.509,10.2,10.82,11.38,13.33,14.35,14.86,15.09,15.13,14.92,14.61,14.26,13.9,13.53,13.17,11.55,10.24,9.184,8.317,6.989,6.025,5.297,4.729,4.274,3.901,3.591};
        physicsVector = CreatePhysicsVector(E,He_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double He_10[31]={5.57,6.164,7.257,8.253,9.168,10.01,10.78,11.47,12.11,14.4,15.67,16.35,16.69,16.86,16.71,16.43,16.09,15.73,15.36,14.99,13.3,11.9,10.75,9.798,8.315,7.221,6.383,5.722,5.189,4.749,4.381};
        physicsVector = CreatePhysicsVector(E,He_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double He_11[31]={5.709,6.323,7.464,8.515,9.493,10.4,11.24,12.02,12.73,15.4,16.95,17.83,18.31,18.66,18.6,18.38,18.07,17.72,17.36,16.99,15.25,13.76,12.52,11.47,9.812,8.571,7.609,6.843,6.22,5.704,5.269};
        physicsVector = CreatePhysicsVector(E,He_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double He_12[31]={5.934,6.556,7.715,8.793,9.807,10.76,11.65,12.49,13.26,16.25,18.08,19.16,19.79,20.33,20.38,20.22,19.94,19.62,19.27,18.9,17.13,15.57,14.25,13.12,11.32,9.946,8.87,8.005,7.297,6.706,6.206};
        physicsVector = CreatePhysicsVector(E,He_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double He_13[31]={6.134,6.771,7.956,9.062,10.11,11.1,12.04,12.92,13.74,17.03,19.13,20.42,21.21,21.95,22.13,22.04,21.82,21.52,21.19,20.83,19.04,17.43,16.03,14.83,12.89,11.38,10.2,9.233,8.438,7.771,7.205};
        physicsVector = CreatePhysicsVector(E,He_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double He_14[31]={6.339,6.99,8.198,9.326,10.4,11.42,12.39,13.31,14.18,17.73,20.07,21.57,22.51,23.46,23.76,23.75,23.57,23.31,22.99,22.65,20.85,19.2,17.75,16.48,14.41,12.8,11.51,10.46,9.59,8.853,8.225};
        physicsVector = CreatePhysicsVector(E,He_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double He_15[31]={6.634,7.302,8.541,9.696,10.79,11.84,12.85,13.81,14.72,18.51,21.1,22.8,23.91,25.08,25.52,25.6,25.49,25.27,24.98,24.66,22.88,21.18,19.66,18.33,16.12,14.38,12.98,11.82,10.86,10.05,9.347};
        physicsVector = CreatePhysicsVector(E,He_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double He_16[31]={6.833,7.524,8.803,9.993,11.12,12.21,13.25,14.24,15.2,19.23,22.07,23.99,25.27,26.69,27.28,27.46,27.42,27.25,27,26.7,24.96,23.23,21.66,20.26,17.91,16.05,14.53,13.27,12.22,11.32,10.54};
        physicsVector = CreatePhysicsVector(E,He_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double He_17[31]={7.144,7.843,9.139,10.34,11.49,12.59,13.65,14.68,15.66,19.88,22.95,25.06,26.5,28.14,28.87,29.15,29.17,29.05,28.83,28.55,26.84,25.09,23.48,22.03,19.58,17.61,16,14.66,13.52,12.55,11.72};
        physicsVector = CreatePhysicsVector(E,He_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double He_18[31]={7.268,7.978,9.289,10.51,11.66,12.78,13.85,14.89,15.9,20.3,23.58,25.89,27.49,29.36,30.24,30.62,30.71,30.63,30.44,30.19,28.52,26.76,25.12,23.64,21.12,19.07,17.38,15.97,14.77,13.75,12.85};
        physicsVector = CreatePhysicsVector(E,He_18,factor);
     }
  }

  else if( matName == namesMat[2] ) {
     if( ionZ == 3 ) {
        G4double Be_3[31]={2.148,2.307,2.562,2.753,2.897,3.003,3.08,3.132,3.166,3.193,3.127,3.033,2.931,2.727,2.538,2.368,2.217,2.082,1.961,1.854,1.451,1.192,1.013,0.8818,0.7042,0.5894,0.5089,0.4493,0.4032,0.3664,0.3363};
        physicsVector = CreatePhysicsVector(E,Be_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Be_4[31]={2.702,2.928,3.303,3.599,3.833,4.017,4.162,4.273,4.357,4.533,4.535,4.474,4.386,4.177,3.961,3.754,3.561,3.382,3.218,3.067,2.476,2.07,1.777,1.557,1.251,1.049,0.9061,0.7996,0.7172,0.6515,0.5978};
        physicsVector = CreatePhysicsVector(E,Be_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Be_5[31]={3.141,3.432,3.925,4.326,4.654,4.922,5.141,5.32,5.463,5.831,5.93,5.924,5.868,5.687,5.471,5.247,5.029,4.82,4.624,4.44,3.682,3.131,2.72,2.402,1.947,1.639,1.418,1.252,1.123,1.02,0.9352};
        physicsVector = CreatePhysicsVector(E,Be_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Be_6[31]={3.484,3.837,4.444,4.949,5.372,5.727,6.025,6.275,6.483,7.078,7.297,7.363,7.353,7.22,7.02,6.796,6.566,6.339,6.12,5.911,5.014,4.332,3.805,3.388,2.775,2.35,2.039,1.803,1.619,1.47,1.349};
        physicsVector = CreatePhysicsVector(E,Be_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Be_7[31]={3.795,4.198,4.903,5.499,6.008,6.445,6.819,7.14,7.413,8.257,8.619,8.774,8.824,8.761,8.595,8.383,8.153,7.917,7.684,7.457,6.449,5.649,5.011,4.497,3.723,3.173,2.764,2.451,2.203,2.003,1.838};
        physicsVector = CreatePhysicsVector(E,Be_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Be_8[31]={4.05,4.499,5.294,5.978,6.57,7.085,7.534,7.924,8.263,9.368,9.89,10.15,10.27,10.3,10.18,9.998,9.78,9.546,9.308,9.071,7.978,7.075,6.336,5.727,4.791,4.109,3.597,3.197,2.879,2.62,2.406};
        physicsVector = CreatePhysicsVector(E,Be_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Be_9[31]={4.249,4.737,5.609,6.369,7.035,7.621,8.137,8.591,8.991,10.35,11.03,11.39,11.59,11.7,11.64,11.48,11.28,11.05,10.81,10.57,9.42,8.439,7.621,6.937,5.866,5.072,4.465,3.987,3.602,3.286,3.023};
        physicsVector = CreatePhysicsVector(E,Be_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Be_10[31]={4.423,4.942,5.88,6.708,7.442,8.094,8.674,9.189,9.647,11.26,12.11,12.58,12.85,13.07,13.07,12.94,12.76,12.55,12.31,12.07,10.87,9.823,8.936,8.184,6.988,6.087,5.389,4.833,4.381,4.008,3.695};
        physicsVector = CreatePhysicsVector(E,Be_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Be_11[31]={4.542,5.085,6.08,6.967,7.763,8.478,9.12,9.697,10.22,12.09,13.14,13.75,14.11,14.46,14.53,14.46,14.31,14.11,13.89,13.65,12.43,11.33,10.38,9.559,8.236,7.222,6.425,5.784,5.259,4.822,4.452};
        physicsVector = CreatePhysicsVector(E,Be_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Be_12[31]={4.691,5.249,6.282,7.213,8.055,8.819,9.511,10.14,10.71,12.83,14.06,14.8,15.27,15.75,15.91,15.89,15.77,15.6,15.39,15.16,13.93,12.79,11.79,10.91,9.483,8.37,7.485,6.765,6.171,5.673,5.249};
        physicsVector = CreatePhysicsVector(E,Be_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Be_13[31]={4.831,5.402,6.469,7.44,8.325,9.133,9.871,10.54,11.16,13.5,14.93,15.81,16.38,17.01,17.26,17.31,17.24,17.09,16.91,16.69,15.47,14.3,13.25,12.32,10.79,9.579,8.605,7.807,7.142,6.582,6.103};
        physicsVector = CreatePhysicsVector(E,Be_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Be_14[31]={4.976,5.558,6.651,7.655,8.577,9.423,10.2,10.92,11.57,14.12,15.73,16.74,17.42,18.19,18.54,18.66,18.63,18.52,18.35,18.15,16.95,15.75,14.66,13.69,12.07,10.77,9.722,8.852,8.123,7.505,6.974};
        physicsVector = CreatePhysicsVector(E,Be_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Be_15[31]={5.153,5.747,6.866,7.902,8.859,9.743,10.56,11.31,12.01,14.76,16.55,17.72,18.5,19.44,19.9,20.1,20.14,20.08,19.94,19.77,18.62,17.4,16.27,15.26,13.53,12.13,10.99,10.04,9.232,8.545,7.953};
        physicsVector = CreatePhysicsVector(E,Be_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Be_16[31]={5.3,5.907,7.053,8.12,9.113,10.03,10.89,11.68,12.42,15.37,17.35,18.66,19.56,20.67,21.25,21.54,21.65,21.63,21.54,21.39,20.3,19.09,17.93,16.87,15.05,13.56,12.33,11.29,10.41,9.655,9.001};
        physicsVector = CreatePhysicsVector(E,Be_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Be_17[31]={5.5,6.113,7.275,8.362,9.379,10.33,11.21,12.04,12.81,15.93,18.08,19.53,20.55,21.82,22.51,22.89,23.05,23.08,23.02,22.9,21.87,20.65,19.47,18.38,16.49,14.92,13.6,12.5,11.55,10.73,10.02};
        physicsVector = CreatePhysicsVector(E,Be_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Be_18[31]={5.619,6.238,7.409,8.508,9.542,10.51,11.42,12.28,13.07,16.35,18.66,20.25,21.37,22.8,23.61,24.07,24.29,24.37,24.34,24.25,23.27,22.05,20.85,19.74,17.78,16.14,14.77,13.61,12.61,11.75,10.99};
        physicsVector = CreatePhysicsVector(E,Be_18,factor);
     }
}

  else if( matName == namesMat[3] ) {
     if( ionZ == 3 ) {
        G4double C_3[31]={2.043,2.2,2.457,2.658,2.815,2.939,3.035,3.109,3.162,3.238,3.178,3.082,2.978,2.774,2.587,2.419,2.27,2.137,2.018,1.911,1.508,1.246,1.063,0.9292,0.7457,0.6261,0.5418,0.4792,0.4306,0.3918,0.3599};
        physicsVector = CreatePhysicsVector(E,C_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double C_4[31]={2.54,2.761,3.135,3.441,3.694,3.902,4.072,4.21,4.321,4.588,4.608,4.546,4.455,4.245,4.032,3.83,3.641,3.467,3.306,3.159,2.572,2.163,1.866,1.641,1.325,1.115,0.9653,0.8533,0.7664,0.6969,0.64};
        physicsVector = CreatePhysicsVector(E,C_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double C_5[31]={2.919,3.203,3.692,4.102,4.449,4.743,4.992,5.202,5.378,5.882,6.02,6.019,5.96,5.776,5.562,5.346,5.136,4.935,4.745,4.566,3.821,3.272,2.856,2.532,2.063,1.743,1.512,1.337,1.201,1.092,1.002};
        physicsVector = CreatePhysicsVector(E,C_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double C_6[31]={3.206,3.548,4.148,4.659,5.099,5.479,5.809,6.094,6.34,7.113,7.401,7.482,7.471,7.335,7.139,6.923,6.703,6.487,6.278,6.077,5.205,4.529,3.998,3.574,2.944,2.502,2.177,1.929,1.734,1.576,1.447};
        physicsVector = CreatePhysicsVector(E,C_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double C_7[31]={3.445,3.836,4.53,5.131,5.657,6.119,6.527,6.885,7.2,8.259,8.728,8.914,8.969,8.905,8.742,8.541,8.324,8.103,7.884,7.668,6.698,5.91,5.273,4.751,3.957,3.386,2.958,2.627,2.364,2.152,1.975};
        physicsVector = CreatePhysicsVector(E,C_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double C_8[31]={3.638,4.069,4.848,5.533,6.141,6.682,7.165,7.595,7.979,9.328,9.997,10.31,10.44,10.47,10.36,10.18,9.98,9.764,9.543,9.321,8.282,7.401,6.666,6.053,5.096,4.39,3.853,3.432,3.095,2.819,2.59};
        physicsVector = CreatePhysicsVector(E,C_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double C_9[31]={3.792,4.257,5.106,5.864,6.544,7.156,7.707,8.203,8.65,10.28,11.15,11.59,11.8,11.93,11.87,11.72,11.54,11.33,11.11,10.89,9.8,8.848,8.037,7.348,6.254,5.431,4.794,4.289,3.88,3.543,3.261};
        physicsVector = CreatePhysicsVector(E,C_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double C_10[31]={3.927,4.419,5.328,6.149,6.893,7.57,8.185,8.744,9.252,11.15,12.23,12.81,13.12,13.35,13.35,13.24,13.08,12.88,12.67,12.45,11.33,10.32,9.442,8.686,7.464,6.529,5.796,5.208,4.728,4.329,3.993};
        physicsVector = CreatePhysicsVector(E,C_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double C_11[31]={4.019,4.53,5.486,6.362,7.165,7.903,8.579,9.2,9.768,11.94,13.26,14,14.42,14.79,14.86,14.8,14.67,14.49,14.29,14.08,12.95,11.89,10.96,10.14,8.79,7.742,6.908,6.232,5.675,5.208,4.813};
        physicsVector = CreatePhysicsVector(E,C_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double C_12[31]={4.132,4.653,5.638,6.555,7.407,8.194,8.922,9.593,10.21,12.64,14.18,15.08,15.62,16.13,16.29,16.28,16.18,16.03,15.84,15.64,14.51,13.42,12.43,11.56,10.11,8.965,8.041,7.285,6.656,6.126,5.674};
        physicsVector = CreatePhysicsVector(E,C_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double C_13[31]={4.239,4.77,5.781,6.732,7.623,8.455,9.228,9.947,10.61,13.28,15.03,16.11,16.77,17.44,17.7,17.75,17.69,17.57,17.41,17.22,16.11,14.99,13.97,13.05,11.5,10.25,9.242,8.412,7.705,7.108,6.597};
        physicsVector = CreatePhysicsVector(E,C_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double C_14[31]={4.352,4.888,5.918,6.895,7.819,8.689,9.503,10.26,10.97,13.85,15.81,17.06,17.84,18.68,19.04,19.16,19.15,19.06,18.91,18.74,17.66,16.53,15.47,14.51,12.87,11.54,10.45,9.535,8.767,8.111,7.545};
        physicsVector = CreatePhysicsVector(E,C_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double C_15[31]={4.48,5.022,6.068,7.07,8.023,8.927,9.779,10.58,11.33,14.43,16.59,18.01,18.94,19.96,20.44,20.64,20.69,20.65,20.54,20.39,19.37,18.24,17.16,16.16,14.43,13,11.82,10.82,9.972,9.246,8.616};
        physicsVector = CreatePhysicsVector(E,C_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double C_16[31]={4.597,5.146,6.21,7.235,8.216,9.151,10.04,10.88,11.67,14.97,17.33,18.94,20,21.22,21.83,22.12,22.23,22.24,22.17,22.06,21.11,19.99,18.89,17.86,16.05,14.53,13.26,12.18,11.25,10.45,9.757};
        physicsVector = CreatePhysicsVector(E,C_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double C_17[31]={4.745,5.296,6.37,7.409,8.41,9.37,10.29,11.16,11.98,15.46,18.01,19.79,21,22.41,23.14,23.52,23.7,23.75,23.72,23.63,22.76,21.65,20.54,19.48,17.59,16,14.65,13.49,12.5,11.64,10.88};
        physicsVector = CreatePhysicsVector(E,C_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double C_18[31]={4.847,5.401,6.479,7.526,8.54,9.518,10.46,11.35,12.21,15.85,18.57,20.52,21.86,23.47,24.32,24.78,25.02,25.12,25.12,25.06,24.25,23.16,22.02,20.94,19,17.34,15.92,14.71,13.66,12.75,11.95};
        physicsVector = CreatePhysicsVector(E,C_18,factor);
     }
}

  else if( matName == namesMat[4] ) {
     if( ionZ == 3 ) {
        G4double N_3[31]={2.13,2.311,2.613,2.849,3.03,3.166,3.264,3.333,3.381,3.429,3.351,3.239,3.12,2.891,2.685,2.503,2.343,2.201,2.075,1.963,1.544,1.274,1.086,0.9483,0.7603,0.638,0.5519,0.4879,0.4382,0.3986,0.3661};
        physicsVector = CreatePhysicsVector(E,N_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double N_4[31]={2.65,2.892,3.309,3.658,3.947,4.181,4.367,4.512,4.622,4.861,4.862,4.781,4.671,4.427,4.186,3.963,3.757,3.57,3.399,3.242,2.629,2.208,1.903,1.673,1.35,1.136,0.9828,0.8685,0.7799,0.709,0.6509};
        physicsVector = CreatePhysicsVector(E,N_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double N_5[31]={3.07,3.37,3.894,4.342,4.728,5.057,5.333,5.56,5.745,6.232,6.351,6.331,6.251,6.026,5.778,5.534,5.301,5.08,4.875,4.684,3.903,3.335,2.909,2.578,2.1,1.774,1.538,1.36,1.222,1.11,1.019};
        physicsVector = CreatePhysicsVector(E,N_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double N_6[31]={3.409,3.764,4.389,4.93,5.404,5.821,6.183,6.494,6.756,7.532,7.805,7.868,7.836,7.656,7.418,7.168,6.92,6.681,6.452,6.235,5.315,4.615,4.07,3.637,2.995,2.545,2.215,1.962,1.763,1.603,1.471};
        physicsVector = CreatePhysicsVector(E,N_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double N_7[31]={3.704,4.106,4.82,5.441,5.993,6.487,6.927,7.315,7.654,8.74,9.198,9.369,9.404,9.295,9.088,8.847,8.597,8.347,8.105,7.87,6.839,6.021,5.367,4.834,4.024,3.443,3.008,2.672,2.405,2.188,2.009};
        physicsVector = CreatePhysicsVector(E,N_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double N_8[31]={3.958,4.401,5.196,5.894,6.518,7.082,7.593,8.053,8.463,9.864,10.53,10.83,10.94,10.93,10.77,10.55,10.31,10.06,9.811,9.566,8.454,7.535,6.78,6.153,5.179,4.463,3.917,3.49,3.147,2.867,2.634};
        physicsVector = CreatePhysicsVector(E,N_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double N_9[31]={4.177,4.656,5.522,6.288,6.976,7.602,8.173,8.695,9.168,10.87,11.75,12.18,12.39,12.47,12.36,12.17,11.94,11.69,11.44,11.19,10.02,9.022,8.185,7.479,6.362,5.524,4.877,4.363,3.947,3.605,3.318};
        physicsVector = CreatePhysicsVector(E,N_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double N_10[31]={4.375,4.884,5.813,6.641,7.388,8.071,8.698,9.275,9.805,11.79,12.89,13.48,13.78,13.98,13.93,13.77,13.56,13.32,13.07,12.82,11.6,10.54,9.628,8.851,7.601,6.648,5.901,5.303,4.813,4.407,4.065};
        physicsVector = CreatePhysicsVector(E,N_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double N_11[31]={4.532,5.065,6.047,6.932,7.735,8.47,9.15,9.78,10.36,12.63,13.97,14.72,15.14,15.48,15.51,15.4,15.21,14.99,14.75,14.51,13.27,12.15,11.17,10.33,8.95,7.88,7.031,6.342,5.775,5.3,4.898};
        physicsVector = CreatePhysicsVector(E,N_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double N_12[31]={4.702,5.251,6.272,7.203,8.054,8.835,9.559,10.23,10.86,13.38,14.95,15.87,16.41,16.9,17.01,16.95,16.8,16.6,16.37,16.13,14.88,13.72,12.69,11.79,10.3,9.125,8.183,7.413,6.773,6.233,5.773};
        physicsVector = CreatePhysicsVector(E,N_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double N_13[31]={4.858,5.424,6.48,7.449,8.342,9.168,9.934,10.65,11.32,14.06,15.86,16.96,17.63,18.29,18.5,18.5,18.39,18.22,18.01,17.78,16.53,15.33,14.26,13.31,11.71,10.44,9.405,8.552,7.838,7.232,6.713};
        physicsVector = CreatePhysicsVector(E,N_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double N_14[31]={5.011,5.59,6.676,7.68,8.611,9.475,10.28,11.03,11.74,14.67,16.69,17.97,18.77,19.61,19.92,19.99,19.92,19.78,19.59,19.37,18.14,16.92,15.8,14.8,13.11,11.75,10.64,9.706,8.923,8.255,7.679};
        physicsVector = CreatePhysicsVector(E,N_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double N_15[31]={5.17,5.761,6.874,7.91,8.877,9.779,10.62,11.41,12.15,15.28,17.5,18.97,19.92,20.95,21.39,21.54,21.54,21.44,21.28,21.08,19.9,18.67,17.52,16.48,14.7,13.24,12.03,11.01,10.15,9.409,8.768};
        physicsVector = CreatePhysicsVector(E,N_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double N_16[31]={5.313,5.919,7.061,8.131,9.133,10.07,10.95,11.77,12.55,15.85,18.28,19.93,21.03,22.27,22.84,23.08,23.14,23.09,22.97,22.8,21.68,20.45,19.29,18.21,16.34,14.78,13.48,12.39,11.45,10.63,9.926};
        physicsVector = CreatePhysicsVector(E,N_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double N_17[31]={5.481,6.098,7.262,8.355,9.386,10.35,11.26,12.12,12.92,16.38,19,20.83,22.08,23.52,24.22,24.55,24.67,24.67,24.58,24.44,23.38,22.16,20.98,19.87,17.92,16.29,14.91,13.73,12.72,11.84,11.08};
        physicsVector = CreatePhysicsVector(E,N_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double N_18[31]={5.604,6.233,7.416,8.529,9.581,10.57,11.51,12.39,13.22,16.82,19.6,21.61,23,24.65,25.48,25.9,26.08,26.12,26.07,25.96,24.95,23.73,22.53,21.39,19.38,17.67,16.23,14.99,13.92,12.99,12.17};
        physicsVector = CreatePhysicsVector(E,N_18,factor);
     }
}

  else if( matName == namesMat[5] ) {
     if( ionZ == 3 ) {
        G4double O_3[31]={1.865,2.032,2.312,2.534,2.709,2.843,2.944,3.019,3.072,3.153,3.1,3.007,2.904,2.702,2.517,2.353,2.207,2.077,1.962,1.858,1.468,1.215,1.038,0.9083,0.7302,0.6139,0.5318,0.4706,0.4232,0.3852,0.354};
        physicsVector = CreatePhysicsVector(E,O_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double O_4[31]={2.293,2.519,2.909,3.236,3.509,3.735,3.918,4.065,4.18,4.458,4.489,4.433,4.342,4.132,3.919,3.719,3.534,3.364,3.207,3.064,2.497,2.105,1.819,1.602,1.296,1.092,0.9469,0.8378,0.753,0.6851,0.6295};
        physicsVector = CreatePhysicsVector(E,O_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double O_5[31]={2.624,2.903,3.393,3.813,4.176,4.49,4.757,4.982,5.169,5.696,5.853,5.861,5.805,5.618,5.403,5.187,4.978,4.781,4.595,4.421,3.704,3.177,2.778,2.467,2.016,1.706,1.482,1.312,1.18,1.073,0.9855};
        physicsVector = CreatePhysicsVector(E,O_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double O_6[31]={2.883,3.21,3.792,4.3,4.747,5.143,5.49,5.792,6.051,6.861,7.178,7.275,7.27,7.133,6.932,6.714,6.495,6.282,6.076,5.88,5.04,4.392,3.885,3.479,2.874,2.448,2.134,1.893,1.703,1.55,1.423};
        physicsVector = CreatePhysicsVector(E,O_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double O_7[31]={3.103,3.469,4.13,4.714,5.236,5.704,6.124,6.497,6.827,7.932,8.439,8.65,8.715,8.653,8.486,8.281,8.063,7.843,7.627,7.417,6.482,5.729,5.121,4.623,3.862,3.312,2.899,2.578,2.323,2.115,1.944};
        physicsVector = CreatePhysicsVector(E,O_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double O_8[31]={3.292,3.69,4.419,5.072,5.662,6.198,6.684,7.124,7.52,8.922,9.637,9.981,10.13,10.17,10.05,9.869,9.663,9.446,9.227,9.009,8.006,7.164,6.467,5.883,4.969,4.292,3.775,3.367,3.04,2.772,2.549};
        physicsVector = CreatePhysicsVector(E,O_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double O_9[31]={3.458,3.881,4.667,5.381,6.03,6.623,7.168,7.666,8.121,9.803,10.73,11.21,11.46,11.6,11.54,11.38,11.19,10.98,10.76,10.54,9.49,8.58,7.808,7.152,6.106,5.316,4.702,4.213,3.815,3.487,3.212};
        physicsVector = CreatePhysicsVector(E,O_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double O_10[31]={3.612,4.056,4.889,5.655,6.357,7.003,7.601,8.152,8.661,10.61,11.75,12.39,12.74,13,13,12.88,12.71,12.51,12.3,12.08,10.99,10.02,9.187,8.466,7.298,6.4,5.692,5.122,4.655,4.266,3.937};
        physicsVector = CreatePhysicsVector(E,O_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double O_11[31]={3.734,4.194,5.066,5.876,6.626,7.321,7.967,8.569,9.128,11.33,12.71,13.52,13.98,14.39,14.47,14.4,14.26,14.08,13.88,13.66,12.56,11.55,10.66,9.877,8.591,7.584,6.78,6.126,5.584,5.13,4.744};
        physicsVector = CreatePhysicsVector(E,O_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double O_12[31]={3.869,4.34,5.238,6.083,6.872,7.608,8.296,8.939,9.541,11.98,13.57,14.55,15.14,15.71,15.88,15.86,15.75,15.59,15.4,15.19,14.09,13.04,12.1,11.27,9.882,8.78,7.89,7.158,6.548,6.032,5.591};
        physicsVector = CreatePhysicsVector(E,O_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double O_13[31]={3.995,4.477,5.398,6.269,7.09,7.864,8.591,9.273,9.915,12.56,14.37,15.52,16.25,16.99,17.26,17.31,17.24,17.11,16.94,16.74,15.64,14.57,13.59,12.72,11.23,10.04,9.066,8.257,7.577,6.998,6.501};
        physicsVector = CreatePhysicsVector(E,O_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double O_14[31]={4.121,4.612,5.552,6.446,7.295,8.1,8.86,9.576,10.25,13.09,15.09,16.42,17.29,18.21,18.58,18.7,18.68,18.58,18.43,18.24,17.17,16.07,15.06,14.15,12.58,11.3,10.25,9.372,8.626,7.989,7.437};
        physicsVector = CreatePhysicsVector(E,O_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double O_15[31]={4.248,4.749,5.704,6.618,7.493,8.327,9.118,9.867,10.58,13.59,15.79,17.3,18.31,19.44,19.94,20.15,20.19,20.13,20.01,19.85,18.83,17.73,16.7,15.75,14.1,12.73,11.59,10.63,9.812,9.106,8.493};
        physicsVector = CreatePhysicsVector(E,O_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double O_16[31]={4.365,4.877,5.853,6.788,7.688,8.549,9.37,10.15,10.89,14.07,16.46,18.15,19.31,20.64,21.28,21.58,21.68,21.67,21.59,21.46,20.51,19.42,18.37,17.39,15.66,14.21,12.99,11.95,11.06,10.29,9.614};
        physicsVector = CreatePhysicsVector(E,O_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double O_17[31]={4.501,5.022,6.013,6.964,7.882,8.766,9.61,10.42,11.18,14.52,17.07,18.93,20.23,21.78,22.56,22.95,23.12,23.16,23.11,23.01,22.12,21.04,19.98,18.97,17.18,15.66,14.37,13.26,12.3,11.47,10.73};
        physicsVector = CreatePhysicsVector(E,O_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double O_18[31]={4.604,5.133,6.139,7.102,8.034,8.934,9.799,10.63,11.42,14.88,17.59,19.61,21.06,22.82,23.73,24.21,24.44,24.53,24.52,24.44,23.61,22.54,21.46,20.43,18.58,17,15.64,14.47,13.46,12.58,11.8};
        physicsVector = CreatePhysicsVector(E,O_18,factor);
     }
}

  else if( matName == namesMat[6] ) {
     if( ionZ == 3 ) {
        G4double Ne_3[31]={1.497,1.622,1.831,1.999,2.136,2.25,2.343,2.42,2.481,2.631,2.638,2.589,2.519,2.364,2.215,2.079,1.957,1.848,1.749,1.66,1.322,1.1,0.9434,0.8276,0.6681,0.5634,0.4893,0.4339,0.3908,0.3562,0.3278};
        physicsVector = CreatePhysicsVector(E,Ne_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Ne_4[31]={1.876,2.046,2.338,2.58,2.787,2.964,3.117,3.248,3.36,3.697,3.808,3.811,3.765,3.618,3.452,3.29,3.136,2.993,2.861,2.738,2.249,1.905,1.652,1.46,1.186,1.003,0.8715,0.7727,0.6956,0.6338,0.583};
        physicsVector = CreatePhysicsVector(E,Ne_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Ne_5[31]={2.174,2.385,2.752,3.065,3.337,3.576,3.787,3.975,4.14,4.693,4.939,5.024,5.025,4.919,4.76,4.589,4.419,4.255,4.099,3.952,3.335,2.875,2.523,2.247,1.845,1.567,1.364,1.211,1.09,0.9929,0.913};
        physicsVector = CreatePhysicsVector(E,Ne_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Ne_6[31]={2.414,2.661,3.1,3.48,3.815,4.113,4.382,4.624,4.841,5.622,6.026,6.212,6.277,6.237,6.104,5.939,5.765,5.591,5.42,5.256,4.538,3.975,3.529,3.17,2.631,2.249,1.965,1.747,1.574,1.434,1.319};
        physicsVector = CreatePhysicsVector(E,Ne_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Ne_7[31]={2.603,2.879,3.377,3.817,4.21,4.565,4.887,5.181,5.45,6.461,7.043,7.352,7.499,7.553,7.465,7.32,7.153,6.978,6.801,6.627,5.834,5.184,4.652,4.213,3.537,3.044,2.671,2.381,2.149,1.959,1.802};
        physicsVector = CreatePhysicsVector(E,Ne_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Ne_8[31]={2.789,3.09,3.64,4.133,4.578,4.984,5.356,5.698,6.013,7.245,8.009,8.452,8.695,8.863,8.833,8.72,8.569,8.401,8.225,8.048,7.204,6.481,5.873,5.36,4.55,3.945,3.479,3.111,2.813,2.569,2.365};
        physicsVector = CreatePhysicsVector(E,Ne_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Ne_9[31]={2.946,3.267,3.86,4.399,4.892,5.344,5.762,6.148,6.507,7.945,8.888,9.47,9.813,10.11,10.14,10.06,9.933,9.775,9.604,9.426,8.547,7.768,7.097,6.523,5.597,4.891,4.338,3.895,3.534,3.234,2.983};
        physicsVector = CreatePhysicsVector(E,Ne_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Ne_10[31]={3.091,3.431,4.061,4.64,5.175,5.67,6.13,6.558,6.957,8.59,9.709,10.43,10.89,11.32,11.43,11.39,11.29,11.15,10.98,10.81,9.909,9.083,8.359,7.728,6.696,5.894,5.256,4.741,4.316,3.961,3.66};
        physicsVector = CreatePhysicsVector(E,Ne_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Ne_11[31]={3.201,3.555,4.218,4.832,5.406,5.941,6.442,6.91,7.349,9.177,10.48,11.36,11.93,12.52,12.72,12.74,12.67,12.55,12.4,12.23,11.33,10.47,9.695,9.015,7.882,6.985,6.262,5.67,5.179,4.764,4.412};
        physicsVector = CreatePhysicsVector(E,Ne_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Ne_12[31]={3.316,3.68,4.365,5.005,5.608,6.176,6.71,7.213,7.688,9.693,11.17,12.2,12.9,13.66,13.96,14.04,14,13.9,13.77,13.61,12.71,11.82,11.01,10.29,9.066,8.086,7.287,6.626,6.073,5.603,5.2};
        physicsVector = CreatePhysicsVector(E,Ne_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Ne_13[31]={3.429,3.801,4.503,5.164,5.791,6.386,6.95,7.485,7.992,10.16,11.8,12.98,13.81,14.76,15.17,15.32,15.33,15.27,15.15,15,14.12,13.21,12.37,11.61,10.31,9.247,8.373,7.644,7.027,6.501,6.046};
        physicsVector = CreatePhysicsVector(E,Ne_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Ne_14[31]={3.539,3.919,4.635,5.311,5.957,6.574,7.164,7.725,8.261,10.58,12.36,13.69,14.65,15.79,16.33,16.56,16.62,16.58,16.49,16.36,15.51,14.58,13.72,12.92,11.55,10.42,9.473,8.679,8.004,7.423,6.92};
        physicsVector = CreatePhysicsVector(E,Ne_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Ne_15[31]={3.647,4.032,4.759,5.448,6.108,6.744,7.355,7.942,8.503,10.96,12.89,14.37,15.46,16.81,17.49,17.82,17.95,17.96,17.91,17.8,17,16.09,15.2,14.38,12.93,11.73,10.71,9.845,9.104,8.463,7.904};
        physicsVector = CreatePhysicsVector(E,Ne_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Ne_16[31]={3.75,4.145,4.888,5.592,6.268,6.922,7.554,8.163,8.749,11.34,13.41,15.03,16.25,17.81,18.64,19.07,19.27,19.34,19.32,19.25,18.52,17.61,16.72,15.87,14.37,13.09,12,11.07,10.26,9.562,8.947};
        physicsVector = CreatePhysicsVector(E,Ne_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Ne_17[31]={3.861,4.261,5.013,5.725,6.413,7.081,7.73,8.359,8.965,11.67,13.88,15.62,16.97,18.75,19.73,20.26,20.53,20.65,20.68,20.63,19.98,19.09,18.19,17.32,15.77,14.43,13.27,12.28,11.42,10.66,9.997};
        physicsVector = CreatePhysicsVector(E,Ne_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Ne_18[31]={3.955,4.36,5.12,5.839,6.536,7.216,7.88,8.526,9.152,11.97,14.29,16.16,17.62,19.6,20.73,21.37,21.72,21.89,21.95,21.94,21.35,20.47,19.56,18.68,17.07,15.67,14.46,13.42,12.51,11.7,10.99};
        physicsVector = CreatePhysicsVector(E,Ne_18,factor);
     }
}

  else if( matName == namesMat[7] ) {
     if( ionZ == 3 ) {
        G4double Al_3[31]={1.388,1.496,1.67,1.8,1.898,1.971,2.024,2.063,2.093,2.164,2.172,2.15,2.111,2.01,1.901,1.797,1.701,1.612,1.532,1.459,1.176,0.9856,0.8498,0.7483,0.6073,0.514,0.4476,0.3977,0.3588,0.3275,0.3018};
        physicsVector = CreatePhysicsVector(E,Al_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Al_4[31]={1.754,1.91,2.167,2.37,2.531,2.659,2.76,2.838,2.901,3.08,3.151,3.17,3.156,3.075,2.963,2.843,2.723,2.61,2.502,2.402,1.995,1.702,1.484,1.316,1.076,0.9136,0.7962,0.7076,0.6382,0.5824,0.5365};
        physicsVector = CreatePhysicsVector(E,Al_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Al_5[31]={2.047,2.247,2.586,2.86,3.084,3.269,3.421,3.545,3.646,3.96,4.113,4.188,4.214,4.181,4.086,3.966,3.838,3.709,3.584,3.464,2.952,2.562,2.261,2.022,1.67,1.425,1.245,1.107,0.9991,0.9116,0.8395};
        physicsVector = CreatePhysicsVector(E,Al_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Al_6[31]={2.284,2.525,2.94,3.284,3.571,3.813,4.016,4.188,4.332,4.8,5.049,5.192,5.269,5.301,5.241,5.136,5.01,4.876,4.741,4.608,4.013,3.537,3.157,2.847,2.378,2.042,1.791,1.596,1.442,1.316,1.212};
        physicsVector = CreatePhysicsVector(E,Al_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Al_7[31]={2.478,2.753,3.234,3.641,3.988,4.286,4.541,4.761,4.95,5.583,5.941,6.164,6.302,6.419,6.409,6.331,6.218,6.088,5.951,5.812,5.157,4.609,4.156,3.779,3.192,2.76,2.431,2.173,1.966,1.797,1.655};
        physicsVector = CreatePhysicsVector(E,Al_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Al_8[31]={2.648,2.953,3.495,3.962,4.366,4.718,5.024,5.292,5.525,6.328,6.801,7.108,7.314,7.528,7.581,7.541,7.45,7.331,7.199,7.059,6.366,5.757,5.24,4.801,4.1,3.572,3.162,2.836,2.572,2.353,2.17};
        physicsVector = CreatePhysicsVector(E,Al_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Al_9[31]={2.795,3.126,3.719,4.238,4.693,5.094,5.448,5.762,6.039,7.017,7.606,8.001,8.275,8.59,8.709,8.712,8.648,8.544,8.418,8.281,7.563,6.906,6.336,5.843,5.044,4.428,3.942,3.551,3.23,2.963,2.737};
        physicsVector = CreatePhysicsVector(E,Al_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Al_10[31]={2.924,3.276,3.913,4.477,4.978,5.424,5.822,6.178,6.496,7.647,8.355,8.841,9.188,9.614,9.809,9.863,9.833,9.751,9.638,9.508,8.781,8.087,7.471,6.931,6.04,5.34,4.781,4.325,3.947,3.63,3.361};
        physicsVector = CreatePhysicsVector(E,Al_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Al_11[31]={3.037,3.406,4.083,4.69,5.235,5.725,6.167,6.566,6.926,8.264,9.11,9.701,10.14,10.7,10.99,11.12,11.14,11.09,11.01,10.89,10.19,9.479,8.825,8.24,7.25,6.456,5.808,5.273,4.826,4.446,4.121};
        physicsVector = CreatePhysicsVector(E,Al_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Al_12[31]={3.156,3.539,4.245,4.883,5.462,5.987,6.463,6.895,7.288,8.778,9.738,10.41,10.92,11.59,11.96,12.14,12.2,12.17,12.1,11.99,11.28,10.54,9.851,9.231,8.178,7.325,6.625,6.042,5.551,5.132,4.772};
        physicsVector = CreatePhysicsVector(E,Al_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Al_13[31]={3.267,3.662,4.395,5.063,5.672,6.23,6.739,7.205,7.631,9.28,10.36,11.14,11.72,12.52,12.99,13.24,13.35,13.36,13.31,13.23,12.55,11.79,11.07,10.42,9.298,8.375,7.609,6.967,6.421,5.953,5.547};
        physicsVector = CreatePhysicsVector(E,Al_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Al_14[31]={3.374,3.779,4.534,5.227,5.864,6.449,6.988,7.484,7.941,9.742,10.95,11.82,12.48,13.41,13.98,14.3,14.47,14.52,14.5,14.43,13.79,13.02,12.29,11.61,10.42,9.435,8.61,7.912,7.315,6.799,6.349};
        physicsVector = CreatePhysicsVector(E,Al_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Al_15[31]={3.473,3.887,4.662,5.378,6.039,6.65,7.216,7.74,8.226,10.17,11.51,12.47,13.22,14.28,14.96,15.38,15.61,15.72,15.74,15.7,15.12,14.36,13.61,12.91,11.66,10.62,9.727,8.969,8.314,7.745,7.248};
        physicsVector = CreatePhysicsVector(E,Al_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Al_16[31]={3.569,3.992,4.787,5.523,6.208,6.844,7.436,7.987,8.5,10.59,12.05,13.12,13.94,15.15,15.94,16.44,16.74,16.9,16.97,16.96,16.46,15.72,14.96,14.24,12.95,11.84,10.89,10.07,9.364,8.744,8.198};
        physicsVector = CreatePhysicsVector(E,Al_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Al_17[31]={3.673,4.103,4.913,5.666,6.369,7.025,7.639,8.213,8.751,10.97,12.55,13.72,14.63,15.97,16.87,17.46,17.84,18.05,18.16,18.19,17.77,17.05,16.29,15.55,14.21,13.05,12.05,11.18,10.42,9.75,9.159};
        physicsVector = CreatePhysicsVector(E,Al_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Al_18[31]={3.756,4.193,5.016,5.782,6.499,7.171,7.803,8.397,8.956,11.29,12.99,14.25,15.24,16.7,17.7,18.38,18.82,19.09,19.24,19.3,18.96,18.26,17.49,16.74,15.37,14.16,13.12,12.2,11.4,10.69,10.07};
        physicsVector = CreatePhysicsVector(E,Al_18,factor);
     }
}

  else if( matName == namesMat[8] ) {
     if( ionZ == 3 ) {
        G4double Si_3[31]={1.468,1.612,1.817,1.946,2.033,2.093,2.137,2.169,2.192,2.242,2.237,2.208,2.167,2.066,1.96,1.856,1.759,1.67,1.589,1.514,1.224,1.027,0.8863,0.7809,0.634,0.5366,0.4672,0.4151,0.3745,0.3418,0.3149};
        physicsVector = CreatePhysicsVector(E,Si_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Si_4[31]={1.792,1.998,2.323,2.55,2.71,2.828,2.918,2.988,3.043,3.195,3.247,3.254,3.236,3.155,3.047,2.929,2.811,2.697,2.589,2.488,2.071,1.77,1.545,1.371,1.122,0.9528,0.8305,0.7381,0.6658,0.6075,0.5596};
        physicsVector = CreatePhysicsVector(E,Si_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Si_5[31]={2.047,2.299,2.729,3.055,3.297,3.48,3.623,3.738,3.831,4.113,4.241,4.3,4.318,4.283,4.193,4.079,3.954,3.827,3.702,3.581,3.06,2.66,2.35,2.103,1.739,1.484,1.297,1.154,1.042,0.9505,0.8753};
        physicsVector = CreatePhysicsVector(E,Si_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Si_6[31]={2.263,2.546,3.059,3.478,3.804,4.057,4.259,4.424,4.56,4.992,5.211,5.333,5.397,5.424,5.37,5.272,5.152,5.022,4.889,4.757,4.154,3.667,3.276,2.957,2.473,2.125,1.864,1.662,1.502,1.371,1.263};
        physicsVector = CreatePhysicsVector(E,Si_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Si_7[31]={2.453,2.758,3.329,3.825,4.23,4.555,4.819,5.036,5.218,5.814,6.138,6.333,6.454,6.561,6.557,6.489,6.386,6.262,6.129,5.992,5.333,4.773,4.308,3.92,3.315,2.869,2.529,2.261,2.047,1.871,1.724};
        physicsVector = CreatePhysicsVector(E,Si_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Si_8[31]={2.634,2.956,3.568,4.125,4.603,4.997,5.323,5.595,5.825,6.596,7.031,7.307,7.49,7.688,7.746,7.719,7.64,7.531,7.404,7.268,6.575,5.955,5.425,4.974,4.254,3.709,3.285,2.949,2.675,2.448,2.259};
        physicsVector = CreatePhysicsVector(E,Si_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Si_9[31]={2.803,3.14,3.782,4.384,4.921,5.379,5.765,6.091,6.369,7.318,7.869,8.227,8.475,8.767,8.889,8.907,8.858,8.767,8.651,8.521,7.809,7.143,6.56,6.055,5.232,4.598,4.096,3.691,3.359,3.083,2.849};
        physicsVector = CreatePhysicsVector(E,Si_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Si_10[31]={2.965,3.318,3.984,4.616,5.2,5.715,6.158,6.537,6.864,7.996,8.666,9.112,9.426,9.819,10.01,10.08,10.07,10,9.901,9.779,9.063,8.361,7.732,7.179,6.263,5.542,4.965,4.494,4.103,3.775,3.496};
        physicsVector = CreatePhysicsVector(E,Si_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Si_11[31]={3.107,3.473,4.161,4.817,5.437,6.001,6.496,6.927,7.302,8.623,9.423,9.963,10.35,10.86,11.13,11.26,11.29,11.25,11.17,11.07,10.36,9.634,8.966,8.369,7.365,6.562,5.909,5.37,4.919,4.537,4.21};
        physicsVector = CreatePhysicsVector(E,Si_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Si_12[31]={3.257,3.639,4.347,5.018,5.665,6.268,6.809,7.287,7.707,9.209,10.13,10.77,11.23,11.85,12.21,12.4,12.47,12.47,12.41,12.32,11.64,10.89,10.19,9.554,8.473,7.595,6.874,6.273,5.765,5.333,4.96};
        physicsVector = CreatePhysicsVector(E,Si_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Si_13[31]={3.396,3.794,4.523,5.207,5.87,6.502,7.083,7.604,8.067,9.75,10.81,11.54,12.08,12.81,13.26,13.52,13.65,13.68,13.65,13.58,12.94,12.18,11.45,10.78,9.63,8.682,7.894,7.231,6.668,6.184,5.764};
        physicsVector = CreatePhysicsVector(E,Si_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Si_14[31]={3.53,3.941,4.694,5.39,6.063,6.716,7.328,7.887,8.39,10.25,11.43,12.26,12.88,13.73,14.27,14.6,14.79,14.86,14.87,14.82,14.22,13.45,12.7,12.01,10.79,9.782,8.932,8.212,7.595,7.062,6.598};
        physicsVector = CreatePhysicsVector(E,Si_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Si_15[31]={3.652,4.078,4.853,5.562,6.244,6.91,7.547,8.139,8.678,10.71,12.03,12.96,13.66,14.64,15.29,15.7,15.95,16.08,16.12,16.1,15.58,14.83,14.07,13.35,12.08,11,10.08,9.303,8.628,8.042,7.527};
        physicsVector = CreatePhysicsVector(E,Si_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Si_16[31]={3.768,4.21,5.011,5.738,6.429,7.107,7.763,8.382,8.954,11.15,12.6,13.64,14.42,15.54,16.29,16.79,17.1,17.29,17.37,17.39,16.96,16.22,15.46,14.72,13.4,12.26,11.28,10.44,9.713,9.073,8.509};
        physicsVector = CreatePhysicsVector(E,Si_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Si_17[31]={3.887,4.344,5.168,5.91,6.609,7.292,7.961,8.602,9.201,11.55,13.14,14.28,15.14,16.39,17.24,17.83,18.21,18.45,18.58,18.64,18.3,17.59,16.82,16.07,14.7,13.51,12.48,11.59,10.8,10.12,9.505};
        physicsVector = CreatePhysicsVector(E,Si_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Si_18[31]={3.994,4.465,5.314,6.074,6.781,7.469,8.146,8.803,9.426,11.92,13.63,14.87,15.82,17.2,18.15,18.82,19.27,19.56,19.74,19.83,19.57,18.89,18.12,17.35,15.94,14.7,13.62,12.68,11.85,11.12,10.47};
        physicsVector = CreatePhysicsVector(E,Si_18,factor);
     }
}

  else if( matName == namesMat[9] ) {
     if( ionZ == 3 ) {
        G4double Ar_3[31]={1.288,1.41,1.603,1.742,1.838,1.904,1.949,1.979,1.998,2.017,1.985,1.936,1.882,1.773,1.672,1.58,1.498,1.423,1.355,1.293,1.053,0.8894,0.7709,0.6814,0.5556,0.4714,0.4111,0.3656,0.3301,0.3014,0.2778};
        physicsVector = CreatePhysicsVector(E,Ar_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Ar_4[31]={1.578,1.748,2.032,2.253,2.422,2.549,2.645,2.716,2.769,2.886,2.898,2.87,2.825,2.716,2.604,2.495,2.393,2.297,2.207,2.123,1.779,1.528,1.34,1.193,0.9809,0.8355,0.7297,0.6495,0.5864,0.5355,0.4936};
        physicsVector = CreatePhysicsVector(E,Ar_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Ar_5[31]={1.804,2.017,2.382,2.68,2.92,3.111,3.261,3.38,3.474,3.723,3.801,3.81,3.787,3.699,3.591,3.479,3.367,3.258,3.154,3.054,2.624,2.292,2.032,1.825,1.516,1.299,1.138,1.014,0.9162,0.837,0.7714};
        physicsVector = CreatePhysicsVector(E,Ar_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Ar_6[31]={1.988,2.236,2.672,3.04,3.347,3.601,3.808,3.978,4.116,4.52,4.682,4.742,4.752,4.7,4.609,4.502,4.39,4.276,4.164,4.054,3.559,3.155,2.828,2.561,2.151,1.855,1.632,1.458,1.319,1.206,1.112};
        physicsVector = CreatePhysicsVector(E,Ar_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Ar_7[31]={2.142,2.417,2.912,3.34,3.708,4.02,4.283,4.504,4.689,5.262,5.524,5.648,5.701,5.701,5.638,5.547,5.442,5.331,5.218,5.104,4.564,4.101,3.713,3.388,2.878,2.5,2.209,1.98,1.795,1.643,1.516};
        physicsVector = CreatePhysicsVector(E,Ar_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Ar_8[31]={2.285,2.582,3.126,3.606,4.027,4.394,4.71,4.98,5.212,5.963,6.336,6.532,6.635,6.7,6.673,6.605,6.515,6.412,6.302,6.188,5.621,5.111,4.67,4.292,3.686,3.225,2.865,2.576,2.342,2.147,1.983};
        physicsVector = CreatePhysicsVector(E,Ar_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Ar_9[31]={2.419,2.731,3.314,3.838,4.306,4.721,5.085,5.402,5.678,6.609,7.1,7.374,7.533,7.667,7.682,7.64,7.567,7.475,7.372,7.261,6.682,6.134,5.649,5.226,4.534,3.996,3.57,3.224,2.94,2.702,2.501};
        physicsVector = CreatePhysicsVector(E,Ar_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Ar_10[31]={2.55,2.874,3.487,4.048,4.556,5.014,5.422,5.783,6.101,7.21,7.824,8.183,8.402,8.616,8.679,8.668,8.616,8.539,8.446,8.342,7.76,7.185,6.662,6.199,5.428,4.818,4.327,3.925,3.59,3.308,3.068};
        physicsVector = CreatePhysicsVector(E,Ar_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Ar_11[31]={2.671,3.004,3.641,4.233,4.776,5.271,5.718,6.12,6.478,7.764,8.509,8.961,9.249,9.555,9.674,9.701,9.676,9.617,9.537,9.443,8.874,8.28,7.726,7.227,6.381,5.7,5.145,4.686,4.3,3.973,3.692};
        physicsVector = CreatePhysicsVector(E,Ar_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Ar_12[31]={2.799,3.14,3.794,4.41,4.981,5.509,5.992,6.43,6.826,8.279,9.153,9.699,10.06,10.46,10.64,10.71,10.71,10.67,10.61,10.52,9.972,9.364,8.785,8.254,7.342,6.599,5.986,5.473,5.039,4.668,4.348};
        physicsVector = CreatePhysicsVector(E,Ar_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Ar_13[31]={2.923,3.272,3.942,4.577,5.173,5.727,6.24,6.712,7.142,8.757,9.761,10.41,10.84,11.34,11.59,11.7,11.74,11.73,11.68,11.61,11.09,10.48,9.876,9.319,8.347,7.543,6.873,6.308,5.826,5.411,5.051};
        physicsVector = CreatePhysicsVector(E,Ar_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Ar_14[31]={3.044,3.402,4.085,4.736,5.351,5.928,6.467,6.967,7.428,9.196,10.33,11.07,11.58,12.19,12.51,12.67,12.74,12.76,12.73,12.68,12.2,11.58,10.97,10.39,9.361,8.502,7.779,7.165,6.637,6.181,5.782};
        physicsVector = CreatePhysicsVector(E,Ar_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Ar_15[31]={3.158,3.525,4.22,4.883,5.514,6.11,6.671,7.197,7.686,9.599,10.86,11.71,12.3,13.02,13.42,13.65,13.76,13.81,13.81,13.78,13.36,12.76,12.14,11.54,10.46,9.552,8.774,8.108,7.532,7.03,6.589};
        physicsVector = CreatePhysicsVector(E,Ar_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Ar_16[31]={3.27,3.647,4.357,5.032,5.678,6.291,6.872,7.42,7.933,9.984,11.37,12.32,13,13.85,14.33,14.61,14.78,14.86,14.9,14.89,14.53,13.95,13.33,12.72,11.6,10.64,9.809,9.093,8.47,7.923,7.441};
        physicsVector = CreatePhysicsVector(E,Ar_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Ar_17[31]={3.382,3.77,4.494,5.178,5.835,6.462,7.059,7.625,8.16,10.34,11.84,12.9,13.66,14.63,15.2,15.55,15.76,15.88,15.95,15.96,15.68,15.13,14.51,13.89,12.73,11.72,10.85,10.09,9.419,8.83,8.309};
        physicsVector = CreatePhysicsVector(E,Ar_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Ar_18[31]={3.489,3.89,4.63,5.325,5.991,6.63,7.24,7.822,8.376,10.67,12.29,13.45,14.29,15.39,16.04,16.45,16.71,16.87,16.97,17.01,16.8,16.26,15.64,15.01,13.82,12.77,11.85,11.04,10.34,9.712,9.155};
        physicsVector = CreatePhysicsVector(E,Ar_18,factor);
     }
}

  else if( matName == namesMat[10] ) {
     if( ionZ == 3 ) {
        G4double Ti_3[31]={1.008,1.108,1.272,1.4,1.5,1.577,1.635,1.68,1.715,1.785,1.777,1.743,1.7,1.612,1.527,1.448,1.376,1.311,1.251,1.197,0.9831,0.8357,0.7282,0.6465,0.5304,0.452,0.3954,0.3525,0.3188,0.2916,0.269};
        physicsVector = CreatePhysicsVector(E,Ti_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Ti_4[31]={1.218,1.356,1.594,1.789,1.949,2.081,2.187,2.274,2.345,2.538,2.587,2.579,2.548,2.464,2.372,2.281,2.193,2.11,2.032,1.959,1.656,1.433,1.263,1.13,0.9349,0.7999,0.701,0.6255,0.5659,0.5177,0.4777};
        physicsVector = CreatePhysicsVector(E,Ti_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Ti_5[31]={1.374,1.544,1.846,2.103,2.321,2.507,2.664,2.796,2.907,3.249,3.381,3.419,3.414,3.353,3.268,3.175,3.081,2.988,2.898,2.812,2.438,2.145,1.912,1.725,1.442,1.241,1.091,0.9755,0.8833,0.8084,0.7461};
        physicsVector = CreatePhysicsVector(E,Ti_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Ti_6[31]={1.498,1.692,2.047,2.358,2.631,2.869,3.075,3.253,3.406,3.912,4.147,4.247,4.279,4.258,4.19,4.105,4.012,3.916,3.821,3.727,3.3,2.947,2.657,2.416,2.043,1.77,1.563,1.401,1.27,1.164,1.075};
        physicsVector = CreatePhysicsVector(E,Ti_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Ti_7[31]={1.6,1.812,2.207,2.563,2.883,3.168,3.42,3.643,3.839,4.516,4.869,5.044,5.125,5.16,5.122,5.053,4.968,4.876,4.781,4.685,4.224,3.823,3.481,3.191,2.73,2.382,2.113,1.9,1.727,1.583,1.464};
        physicsVector = CreatePhysicsVector(E,Ti_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Ti_8[31]={1.696,1.922,2.348,2.742,3.102,3.428,3.723,3.988,4.225,5.076,5.555,5.816,5.956,6.06,6.06,6.014,5.944,5.86,5.768,5.673,5.194,4.756,4.371,4.036,3.49,3.068,2.736,2.468,2.249,2.067,1.912};
        physicsVector = CreatePhysicsVector(E,Ti_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Ti_9[31]={1.789,2.024,2.474,2.896,3.289,3.651,3.984,4.287,4.561,5.582,6.192,6.547,6.751,6.932,6.976,6.956,6.903,6.831,6.746,6.655,6.17,5.705,5.284,4.911,4.29,3.8,3.407,3.087,2.822,2.599,2.41};
        physicsVector = CreatePhysicsVector(E,Ti_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Ti_10[31]={1.883,2.125,2.593,3.037,3.457,3.849,4.214,4.551,4.86,6.044,6.786,7.241,7.517,7.788,7.882,7.894,7.862,7.804,7.73,7.645,7.164,6.679,6.23,5.824,5.134,4.58,4.129,3.756,3.445,3.181,2.955};
        physicsVector = CreatePhysicsVector(E,Ti_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Ti_11[31]={1.969,2.218,2.7,3.162,3.603,4.02,4.413,4.78,5.121,6.462,7.338,7.9,8.255,8.629,8.784,8.835,8.83,8.79,8.728,8.653,8.186,7.69,7.218,6.783,6.031,5.415,4.906,4.482,4.124,3.818,3.554};
        physicsVector = CreatePhysicsVector(E,Ti_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Ti_12[31]={2.062,2.316,2.808,3.283,3.74,4.178,4.594,4.987,5.356,6.841,7.846,8.514,8.953,9.436,9.658,9.753,9.777,9.758,9.711,9.646,9.197,8.694,8.204,7.745,6.939,6.267,5.706,5.233,4.831,4.484,4.184};
        physicsVector = CreatePhysicsVector(E,Ti_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Ti_13[31]={2.154,2.414,2.917,3.402,3.873,4.326,4.762,5.177,5.57,7.192,8.324,9.101,9.626,10.23,10.52,10.66,10.72,10.72,10.7,10.64,10.23,9.723,9.219,8.741,7.886,7.162,6.55,6.029,5.583,5.196,4.859};
        physicsVector = CreatePhysicsVector(E,Ti_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Ti_14[31]={2.246,2.512,3.023,3.518,3.998,4.465,4.915,5.348,5.762,7.509,8.763,9.648,10.26,10.98,11.35,11.55,11.64,11.67,11.66,11.63,11.25,10.75,10.24,9.74,8.842,8.071,7.413,6.848,6.359,5.934,5.561};
        physicsVector = CreatePhysicsVector(E,Ti_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Ti_15[31]={2.33,2.602,3.122,3.624,4.112,4.589,5.052,5.5,5.932,7.792,9.166,10.16,10.86,11.72,12.18,12.43,12.57,12.64,12.65,12.64,12.31,11.83,11.32,10.81,9.876,9.059,8.354,7.742,7.21,6.744,6.333};
        physicsVector = CreatePhysicsVector(E,Ti_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Ti_16[31]={2.246,2.512,3.023,3.518,3.998,4.465,4.915,5.348,5.762,7.509,8.763,9.648,10.26,10.98,11.35,11.55,11.64,11.67,11.66,11.63,11.25,10.75,10.24,9.74,8.842,8.071,7.413,6.848,6.359,5.934,5.561};
        physicsVector = CreatePhysicsVector(E,Ti_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Ti_17[31]={2.5,2.786,3.327,3.844,4.346,4.838,5.32,5.792,6.25,8.302,9.896,11.1,11.99,13.13,13.77,14.16,14.39,14.53,14.61,14.64,14.45,14.01,13.51,12.99,12,11.1,10.32,9.62,9.006,8.461,7.976};
        physicsVector = CreatePhysicsVector(E,Ti_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Ti_18[31]={2.585,2.879,3.433,3.958,4.467,4.965,5.454,5.934,6.403,8.533,10.23,11.53,12.52,13.8,14.53,14.99,15.27,15.45,15.55,15.6,15.48,15.07,14.57,14.04,13.02,12.1,11.27,10.54,9.887,9.308,8.791};
        physicsVector = CreatePhysicsVector(E,Ti_18,factor);
     }
}

  else if( matName == namesMat[11] ) {
     if( ionZ == 3 ) {
        G4double Fe_3[31]={0.8835,0.9749,1.128,1.249,1.346,1.422,1.481,1.526,1.561,1.644,1.652,1.63,1.595,1.515,1.436,1.363,1.297,1.236,1.18,1.13,0.9314,0.7945,0.6945,0.6183,0.5096,0.4358,0.3822,0.3414,0.3092,0.2831,0.2615};
        physicsVector = CreatePhysicsVector(E,Fe_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Fe_4[31]={1.055,1.181,1.398,1.58,1.732,1.859,1.964,2.051,2.121,2.322,2.391,2.401,2.383,2.312,2.227,2.142,2.061,1.984,1.912,1.844,1.565,1.359,1.202,1.078,0.8965,0.7699,0.6767,0.6051,0.5484,0.5024,0.4642};
        physicsVector = CreatePhysicsVector(E,Fe_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Fe_5[31]={1.178,1.33,1.603,1.839,2.044,2.221,2.373,2.502,2.612,2.956,3.108,3.17,3.182,3.14,3.064,2.978,2.891,2.805,2.722,2.642,2.298,2.029,1.815,1.642,1.38,1.193,1.052,0.9424,0.8551,0.7837,0.7243};
        physicsVector = CreatePhysicsVector(E,Fe_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Fe_6[31]={1.273,1.446,1.764,2.047,2.299,2.522,2.719,2.892,3.042,3.543,3.794,3.92,3.976,3.98,3.924,3.846,3.76,3.672,3.584,3.497,3.105,2.782,2.516,2.295,1.952,1.698,1.504,1.351,1.228,1.127,1.042};
        physicsVector = CreatePhysicsVector(E,Fe_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Fe_7[31]={1.349,1.535,1.886,2.208,2.5,2.765,3.003,3.217,3.407,4.072,4.434,4.636,4.745,4.815,4.792,4.731,4.654,4.569,4.481,4.392,3.968,3.602,3.291,3.026,2.602,2.281,2.03,1.83,1.667,1.532,1.418};
        physicsVector = CreatePhysicsVector(E,Fe_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Fe_8[31]={1.422,1.617,1.994,2.347,2.674,2.974,3.249,3.5,3.727,4.558,5.037,5.324,5.495,5.643,5.663,5.627,5.564,5.487,5.403,5.315,4.875,4.475,4.126,3.821,3.321,2.933,2.624,2.374,2.168,1.996,1.85};
        physicsVector = CreatePhysicsVector(E,Fe_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Fe_9[31]={1.492,1.695,2.089,2.466,2.82,3.151,3.458,3.741,4.002,4.993,5.594,5.97,6.208,6.444,6.514,6.508,6.463,6.397,6.319,6.235,5.788,5.364,4.984,4.646,4.079,3.629,3.265,2.967,2.718,2.509,2.33};
        physicsVector = CreatePhysicsVector(E,Fe_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Fe_10[31]={1.564,1.772,2.179,2.574,2.951,3.306,3.64,3.953,4.244,5.386,6.111,6.58,6.89,7.226,7.354,7.383,7.36,7.309,7.241,7.163,6.721,6.279,5.872,5.505,4.879,4.37,3.953,3.607,3.316,3.068,2.855};
        physicsVector = CreatePhysicsVector(E,Fe_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Fe_11[31]={1.632,1.845,2.262,2.67,3.064,3.441,3.799,4.136,4.454,5.738,6.588,7.155,7.543,7.99,8.187,8.259,8.264,8.232,8.177,8.108,7.678,7.226,6.799,6.407,5.726,5.162,4.694,4.3,3.965,3.679,3.431};
        physicsVector = CreatePhysicsVector(E,Fe_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Fe_12[31]={1.706,1.923,2.348,2.765,3.173,3.566,3.943,4.302,4.643,6.055,7.026,7.69,8.156,8.719,8.992,9.113,9.15,9.14,9.1,9.041,8.628,8.169,7.726,7.314,6.585,5.972,5.456,5.018,4.643,4.319,4.037};
        physicsVector = CreatePhysicsVector(E,Fe_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Fe_13[31]={1.778,2,2.432,2.857,3.274,3.681,4.074,4.451,4.811,6.343,7.433,8.196,8.742,9.426,9.779,9.955,10.03,10.05,10.02,9.979,9.594,9.135,8.681,8.25,7.479,6.82,6.259,5.778,5.363,5.002,4.685};
        physicsVector = CreatePhysicsVector(E,Fe_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Fe_14[31]={1.851,2.078,2.517,2.948,3.373,3.79,4.196,4.588,4.965,6.603,7.808,8.669,9.296,10.1,10.54,10.77,10.89,10.93,10.93,10.9,10.55,10.1,9.637,9.193,8.384,7.684,7.082,6.56,6.107,5.71,5.36};
        physicsVector = CreatePhysicsVector(E,Fe_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Fe_15[31]={1.919,2.151,2.597,3.033,3.463,3.888,4.305,4.709,5.101,6.833,8.149,9.109,9.819,10.76,11.29,11.59,11.75,11.83,11.86,11.85,11.56,11.12,10.65,10.2,9.358,8.618,7.974,7.411,6.918,6.484,6.099};
        physicsVector = CreatePhysicsVector(E,Fe_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Fe_16[31]={1.987,2.225,2.681,3.122,3.559,3.991,4.416,4.831,5.235,7.05,8.47,9.527,10.32,11.4,12.03,12.4,12.61,12.73,12.78,12.8,12.56,12.14,11.68,11.22,10.36,9.581,8.898,8.296,7.765,7.294,6.875};
        physicsVector = CreatePhysicsVector(E,Fe_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Fe_17[31]={2.055,2.3,2.764,3.211,3.651,4.087,4.519,4.943,5.357,7.244,8.76,9.911,10.79,12,12.73,13.18,13.44,13.6,13.68,13.72,13.56,13.16,12.71,12.24,11.36,10.55,9.832,9.195,8.628,8.123,7.671};
        physicsVector = CreatePhysicsVector(E,Fe_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Fe_18[31]={2.127,2.378,2.854,3.308,3.753,4.194,4.631,5.062,5.485,7.434,9.04,10.28,11.24,12.58,13.42,13.94,14.26,14.46,14.57,14.63,14.53,14.16,13.71,13.24,12.33,11.49,10.74,10.07,9.473,8.937,8.455};
        physicsVector = CreatePhysicsVector(E,Fe_18,factor);
     }
}

  else if( matName == namesMat[12] ) {
     if( ionZ == 3 ) {
        G4double Ni_3[31]={0.8314,0.9198,1.07,1.193,1.294,1.376,1.443,1.495,1.536,1.634,1.649,1.632,1.601,1.525,1.447,1.375,1.308,1.247,1.192,1.141,0.942,0.8042,0.7035,0.6266,0.5169,0.4423,0.3881,0.3468,0.3142,0.2878,0.266};
        physicsVector = CreatePhysicsVector(E,Ni_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Ni_4[31]={0.9896,1.109,1.32,1.5,1.654,1.786,1.899,1.994,2.072,2.301,2.382,2.399,2.387,2.322,2.24,2.157,2.076,1.999,1.927,1.859,1.579,1.373,1.215,1.091,0.9083,0.7808,0.6867,0.6144,0.5571,0.5105,0.4718};
        physicsVector = CreatePhysicsVector(E,Ni_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Ni_5[31]={1.103,1.245,1.506,1.737,1.941,2.12,2.278,2.417,2.537,2.92,3.091,3.162,3.183,3.151,3.08,2.995,2.908,2.823,2.74,2.66,2.316,2.046,1.832,1.659,1.397,1.208,1.066,0.9562,0.8681,0.796,0.736};
        physicsVector = CreatePhysicsVector(E,Ni_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Ni_6[31]={1.191,1.35,1.649,1.923,2.17,2.393,2.594,2.774,2.934,3.485,3.763,3.904,3.971,3.991,3.942,3.867,3.781,3.693,3.605,3.518,3.125,2.802,2.537,2.316,1.972,1.718,1.523,1.37,1.246,1.144,1.058};
        physicsVector = CreatePhysicsVector(E,Ni_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Ni_7[31]={1.262,1.432,1.759,2.065,2.35,2.611,2.85,3.068,3.266,3.989,4.387,4.609,4.732,4.822,4.809,4.753,4.677,4.592,4.504,4.415,3.991,3.625,3.315,3.05,2.627,2.305,2.054,1.853,1.69,1.553,1.439};
        physicsVector = CreatePhysicsVector(E,Ni_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Ni_8[31]={1.329,1.508,1.855,2.187,2.502,2.796,3.069,3.322,3.555,4.445,4.972,5.283,5.471,5.645,5.679,5.65,5.59,5.513,5.429,5.341,4.899,4.5,4.151,3.847,3.349,2.961,2.653,2.402,2.196,2.023,1.876};
        physicsVector = CreatePhysicsVector(E,Ni_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Ni_9[31]={1.395,1.581,1.942,2.293,2.631,2.952,3.254,3.536,3.8,4.848,5.506,5.914,6.173,6.44,6.529,6.533,6.492,6.427,6.35,6.265,5.816,5.392,5.012,4.675,4.111,3.662,3.299,3,2.751,2.541,2.361};
        physicsVector = CreatePhysicsVector(E,Ni_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Ni_10[31]={1.461,1.653,2.026,2.39,2.746,3.088,3.414,3.722,4.013,5.207,5.998,6.507,6.842,7.213,7.366,7.409,7.392,7.343,7.276,7.198,6.751,6.309,5.903,5.538,4.914,4.408,3.992,3.646,3.355,3.107,2.893};
        physicsVector = CreatePhysicsVector(E,Ni_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Ni_11[31]={1.522,1.719,2.102,2.477,2.846,3.205,3.552,3.883,4.198,5.524,6.447,7.062,7.479,7.967,8.194,8.284,8.298,8.269,8.215,8.146,7.713,7.259,6.833,6.442,5.765,5.204,4.737,4.344,4.01,3.723,3.474};
        physicsVector = CreatePhysicsVector(E,Ni_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Ni_12[31]={1.588,1.791,2.183,2.566,2.944,3.316,3.679,4.029,4.364,5.809,6.856,7.576,8.076,8.685,8.993,9.136,9.186,9.18,9.142,9.084,8.668,8.207,7.764,7.353,6.628,6.019,5.505,5.068,4.693,4.369,4.087};
        physicsVector = CreatePhysicsVector(E,Ni_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Ni_13[31]={1.652,1.86,2.261,2.651,3.037,3.418,3.793,4.158,4.511,6.061,7.229,8.057,8.643,9.378,9.772,9.974,10.06,10.09,10.07,10.03,9.64,9.177,8.722,8.293,7.525,6.871,6.313,5.833,5.419,5.058,4.741};
        physicsVector = CreatePhysicsVector(E,Ni_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Ni_14[31]={1.717,1.93,2.34,2.737,3.129,3.517,3.901,4.278,4.645,6.29,7.571,8.504,9.178,10.04,10.53,10.79,10.92,10.98,10.98,10.96,10.61,10.15,9.683,9.239,8.435,7.74,7.141,6.621,6.169,5.772,5.423};
        physicsVector = CreatePhysicsVector(E,Ni_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Ni_15[31]={1.776,1.994,2.411,2.814,3.211,3.605,3.996,4.382,4.761,6.489,7.879,8.917,9.681,10.68,11.27,11.6,11.79,11.88,11.91,11.91,11.61,11.17,10.7,10.25,9.41,8.676,8.036,7.476,6.985,6.551,6.166};
        physicsVector = CreatePhysicsVector(E,Ni_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Ni_16[31]={1.837,2.061,2.488,2.899,3.302,3.701,4.098,4.492,4.881,6.679,8.166,9.307,10.16,11.3,11.99,12.4,12.64,12.77,12.84,12.86,12.62,12.2,11.73,11.27,10.41,9.641,8.963,8.365,7.836,7.366,6.947};
        physicsVector = CreatePhysicsVector(E,Ni_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Ni_17[31]={1.899,2.127,2.563,2.981,3.389,3.792,4.193,4.593,4.989,6.847,8.422,9.66,10.6,11.89,12.68,13.17,13.47,13.64,13.74,13.78,13.62,13.22,12.76,12.3,11.41,10.61,9.901,9.267,8.703,8.2,7.749};
        physicsVector = CreatePhysicsVector(E,Ni_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Ni_18[31]={1.965,2.199,2.645,3.072,3.486,3.894,4.299,4.704,5.106,7.015,8.67,10,11.03,12.46,13.35,13.92,14.28,14.5,14.63,14.7,14.6,14.22,13.77,13.3,12.39,11.56,10.82,10.15,9.555,9.021,8.54};
        physicsVector = CreatePhysicsVector(E,Ni_18,factor);
     }
}

  else if( matName == namesMat[13] ) {
    if( ionZ == 3) {
      G4double Cu_3[31]={0.618,0.6921,0.8207,0.9275,1.017,1.091,1.153,1.203,1.245,1.356,1.386,1.383,1.365,1.311,1.252,1.195,1.142,1.093,1.047,1.006,0.8383,0.7202,0.6328,0.5655,0.4688,0.4025,0.354,0.3169,0.2876,0.2638,0.244};
      physicsVector = CreatePhysicsVector(E,Cu_3,factor);
    }
    else if( ionZ == 4) {
      G4double Cu_4[31]={0.7253,0.8223,0.9989,1.153,1.287,1.404,1.505,1.593,1.668,1.903,1.998,2.031,2.034,1.996,1.938,1.874,1.811,1.75,1.691,1.636,1.403,1.228,1.092,0.9835,0.823,0.71,0.6261,0.5613,0.5098,0.4677,0.4328};
    physicsVector = CreatePhysicsVector(E,Cu_4,factor);
    }
    else if( ionZ == 5) {
      G4double Cu_5[31]={0.8018,0.9144,1.127,1.321,1.496,1.652,1.792,1.917,2.027,2.404,2.588,2.675,2.711,2.709,2.664,2.603,2.537,2.47,2.404,2.34,2.056,1.828,1.645,1.495,1.265,1.098,0.9717,0.8731,0.794,0.7291,0.6749};
      physicsVector = CreatePhysicsVector(E,Cu_5,factor);
    }
    else if( ionZ == 6) {
      G4double Cu_6[31]={0.8636,0.9862,1.225,1.45,1.659,1.85,2.025,2.184,2.329,2.856,3.143,3.297,3.379,3.43,3.41,3.36,3.298,3.231,3.162,3.093,2.772,2.501,2.275,2.085,1.785,1.56,1.387,1.25,1.139,1.047,0.9701};
      physicsVector = CreatePhysicsVector(E,Cu_6,factor);
    }
    else if( ionZ == 7) {
      G4double Cu_7[31]={0.9151,1.044,1.298,1.545,1.781,2.003,2.208,2.399,2.574,3.25,3.651,3.884,4.021,4.142,4.159,4.131,4.08,4.018,3.95,3.881,3.539,3.234,2.971,2.743,2.375,2.093,1.87,1.691,1.544,1.422,1.319};
      physicsVector = CreatePhysicsVector(E,Cu_7,factor);
    }
    else if( ionZ == 8) {
      G4double Cu_8[31]={0.9691,1.102,1.367,1.629,1.885,2.131,2.363,2.581,2.785,3.602,4.122,4.442,4.643,4.846,4.911,4.91,4.876,4.824,4.762,4.695,4.343,4.012,3.718,3.458,3.027,2.687,2.414,2.191,2.006,1.851,1.719};
 physicsVector = CreatePhysicsVector(E,Cu_8,factor);
    }
    else if( ionZ == 9) {
      G4double Cu_9[31]={1.024,1.161,1.432,1.704,1.974,2.238,2.491,2.732,2.961,3.907,4.548,4.961,5.229,5.524,5.644,5.676,5.662,5.623,5.569,5.507,5.154,4.806,4.487,4.2,3.713,3.321,3,2.735,2.513,2.324,2.163};
      physicsVector = CreatePhysicsVector(E,Cu_9,factor);
    }
    else if( ionZ == 10) {
      G4double Cu_10[31]={1.08,1.22,1.497,1.775,2.055,2.331,2.601,2.862,3.111,4.175,4.934,5.443,5.785,6.181,6.364,6.436,6.447,6.424,6.381,6.326,5.982,5.621,5.283,4.973,4.437,3.996,3.63,3.323,3.063,2.841,2.649};
      physicsVector = CreatePhysicsVector(E,Cu_10,factor);
}
    else if( ionZ == 11) {
      G4double Cu_11[31]={1.133,1.277,1.56,1.843,2.128,2.414,2.697,2.973,3.24,4.408,5.281,5.89,6.311,6.819,7.074,7.194,7.236,7.234,7.205,7.16,6.833,6.466,6.112,5.783,5.202,4.715,4.305,3.957,3.66,3.403,3.18};
      physicsVector = CreatePhysicsVector(E,Cu_11,factor);
    }
    else if( ionZ == 12) {
      G4double Cu_12[31]={1.192,1.34,1.628,1.914,2.203,2.494,2.786,3.074,3.356,4.614,5.594,6.3,6.801,7.425,7.759,7.931,8.009,8.031,8.018,7.985,7.678,7.309,6.943,6.598,5.979,5.451,5.001,4.615,4.282,3.993,3.74};
      physicsVector = CreatePhysicsVector(E,Cu_12,factor);
    }
    else if( ionZ == 13) {
      G4double Cu_13[31]={1.248,1.4,1.694,1.984,2.275,2.57,2.868,3.164,3.457,4.795,5.874,6.678,7.26,8.006,8.423,8.654,8.773,8.824,8.832,8.813,8.539,8.172,7.798,7.439,6.786,6.22,5.732,5.31,4.942,4.621,4.338};
      physicsVector = CreatePhysicsVector(E,Cu_13,factor);
    }
    else if( ionZ == 14) {
      G4double Cu_14[31]={1.305,1.461,1.762,2.056,2.35,2.647,2.948,3.25,3.551,4.958,6.129,7.028,7.692,8.562,9.066,9.358,9.52,9.601,9.632,9.63,9.394,9.034,8.655,8.286,7.604,7.005,6.482,6.026,5.625,5.272,4.96};
      physicsVector = CreatePhysicsVector(E,Cu_14,factor);
    }
    else if( ionZ == 15) {
      G4double Cu_15[31]={1.356,1.517,1.824,2.123,2.419,2.717,3.02,3.327,3.634,5.098,6.353,7.345,8.094,9.095,9.694,10.05,10.27,10.39,10.44,10.46,10.28,9.94,9.563,9.188,8.481,7.85,7.293,6.801,6.367,5.982,5.639};
      physicsVector = CreatePhysicsVector(E,Cu_15,factor);
    }
    else if( ionZ == 16) {
      G4double Cu_16[31]={1.408,1.574,1.89,2.194,2.494,2.795,3.099,3.409,3.72,5.232,6.562,7.642,8.475,9.61,10.31,10.74,11.01,11.17,11.26,11.3,11.18,10.86,10.49,10.11,9.381,8.721,8.133,7.609,7.142,6.726,6.353};
      physicsVector = CreatePhysicsVector(E,Cu_16,factor);
    }
    else if( ionZ == 17) {
      G4double Cu_17[31]={1.46,1.631,1.955,2.265,2.568,2.87,3.176,3.486,3.8,5.35,6.745,7.908,8.822,10.09,10.89,11.4,11.72,11.92,12.05,12.11,12.06,11.77,11.4,11.02,10.28,9.599,8.982,8.428,7.931,7.485,7.084};
      physicsVector = CreatePhysicsVector(E,Cu_17,factor);
    }
    else if( ionZ == 18) {
      G4double Cu_18[31]={1.515,1.692,2.025,2.343,2.651,2.956,3.263,3.575,3.89,5.471,6.924,8.164,9.158,10.56,11.46,12.05,12.43,12.67,12.83,12.92,12.93,12.66,12.3,11.92,11.16,10.45,9.811,9.229,8.706,8.233,7.806};
      physicsVector = CreatePhysicsVector(E,Cu_18,factor);
    }
  }
  
  else if( matName == namesMat[14] ) {
     if( ionZ == 3 ) {
        G4double Ge_3[31]={0.6441,0.7195,0.842,0.9317,0.9987,1.05,1.091,1.124,1.152,1.233,1.266,1.272,1.263,1.222,1.171,1.119,1.069,1.023,0.9797,0.9402,0.7833,0.673,0.5915,0.5289,0.439,0.3774,0.3323,0.2979,0.2705,0.2483,0.2298};
        physicsVector = CreatePhysicsVector(E,Ge_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Ge_4[31]={0.7658,0.8635,1.037,1.178,1.29,1.379,1.452,1.513,1.565,1.733,1.819,1.86,1.874,1.855,1.809,1.753,1.695,1.637,1.582,1.53,1.311,1.147,1.02,0.9192,0.7703,0.6653,0.5874,0.5272,0.4793,0.4401,0.4075};
        physicsVector = CreatePhysicsVector(E,Ge_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Ge_5[31]={0.8579,0.9711,1.182,1.367,1.522,1.651,1.759,1.85,1.929,2.198,2.352,2.441,2.488,2.51,2.482,2.432,2.373,2.311,2.249,2.188,1.92,1.707,1.536,1.396,1.183,1.028,0.9111,0.8196,0.746,0.6857,0.6352};
        physicsVector = CreatePhysicsVector(E,Ge_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Ge_6[31]={0.9323,1.057,1.294,1.512,1.706,1.873,2.016,2.139,2.247,2.626,2.856,3.001,3.091,3.168,3.17,3.136,3.083,3.022,2.958,2.893,2.59,2.335,2.123,1.946,1.668,1.46,1.3,1.173,1.07,0.9843,0.9125};
        physicsVector = CreatePhysicsVector(E,Ge_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Ge_7[31]={0.9928,1.126,1.382,1.623,1.847,2.047,2.224,2.379,2.515,3.01,3.321,3.53,3.669,3.815,3.858,3.849,3.81,3.756,3.695,3.63,3.305,3.018,2.771,2.56,2.218,1.957,1.751,1.585,1.449,1.335,1.239};
        physicsVector = CreatePhysicsVector(E,Ge_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Ge_8[31]={1.053,1.192,1.462,1.72,1.966,2.194,2.401,2.586,2.751,3.362,3.758,4.034,4.227,4.451,4.545,4.568,4.549,4.507,4.452,4.39,4.057,3.744,3.468,3.226,2.825,2.511,2.258,2.052,1.881,1.737,1.615};
        physicsVector = CreatePhysicsVector(E,Ge_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Ge_9[31]={1.111,1.254,1.535,1.806,2.068,2.317,2.549,2.76,2.951,3.677,4.159,4.504,4.754,5.063,5.213,5.272,5.278,5.251,5.206,5.15,4.816,4.486,4.186,3.918,3.466,3.102,2.806,2.561,2.355,2.181,2.031};
        physicsVector = CreatePhysicsVector(E,Ge_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Ge_10[31]={1.169,1.316,1.605,1.887,2.16,2.424,2.675,2.91,3.125,3.961,4.53,4.945,5.254,5.655,5.868,5.969,6.003,5.996,5.963,5.916,5.592,5.249,4.93,4.64,4.141,3.732,3.394,3.11,2.87,2.664,2.487};
        physicsVector = CreatePhysicsVector(E,Ge_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Ge_11[31]={1.224,1.375,1.671,1.96,2.242,2.518,2.784,3.038,3.275,4.216,4.872,5.358,5.729,6.228,6.511,6.662,6.73,6.746,6.73,6.693,6.388,6.039,5.705,5.396,4.855,4.404,4.024,3.704,3.428,3.191,2.984};
        physicsVector = CreatePhysicsVector(E,Ge_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Ge_12[31]={1.281,1.438,1.739,2.035,2.325,2.608,2.885,3.154,3.409,4.448,5.187,5.742,6.174,6.772,7.129,7.334,7.44,7.484,7.486,7.463,7.181,6.829,6.482,6.158,5.58,5.091,4.674,4.318,4.01,3.743,3.508};
        physicsVector = CreatePhysicsVector(E,Ge_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Ge_13[31]={1.335,1.498,1.806,2.106,2.402,2.692,2.977,3.256,3.525,4.654,5.477,6.101,6.593,7.293,7.728,7.991,8.14,8.215,8.241,8.235,7.987,7.637,7.282,6.945,6.335,5.81,5.358,4.968,4.628,4.33,4.068};
        physicsVector = CreatePhysicsVector(E,Ge_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Ge_14[31]={1.389,1.558,1.873,2.178,2.478,2.773,3.064,3.351,3.631,4.841,5.744,6.438,6.99,7.791,8.305,8.628,8.823,8.931,8.982,8.995,8.789,8.446,8.086,7.738,7.1,6.543,6.059,5.637,5.267,4.941,4.651};
        physicsVector = CreatePhysicsVector(E,Ge_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Ge_15[31]={1.437,1.612,1.936,2.244,2.548,2.848,3.143,3.435,3.723,5.006,5.988,6.75,7.363,8.27,8.868,9.258,9.504,9.652,9.733,9.768,9.621,9.294,8.935,8.58,7.918,7.331,6.815,6.361,5.959,5.603,5.286};
        physicsVector = CreatePhysicsVector(E,Ge_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Ge_16[31]={1.485,1.667,2.002,2.315,2.622,2.927,3.227,3.523,3.818,5.162,6.218,7.047,7.721,8.733,9.419,9.878,10.18,10.37,10.48,10.54,10.46,10.15,9.797,9.438,8.756,8.143,7.598,7.113,6.682,6.297,5.952};
        physicsVector = CreatePhysicsVector(E,Ge_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Ge_17[31]={1.533,1.721,2.066,2.384,2.695,3.002,3.305,3.605,3.903,5.3,6.424,7.319,8.05,9.166,9.94,10.47,10.83,11.06,11.21,11.29,11.28,11,10.66,10.3,9.599,8.963,8.39,7.878,7.419,7.007,6.636};
        physicsVector = CreatePhysicsVector(E,Ge_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Ge_18[31]={1.585,1.779,2.137,2.463,2.778,3.088,3.395,3.698,4,5.439,6.629,7.587,8.375,9.592,10.45,11.05,11.47,11.74,11.93,12.04,12.1,11.84,11.5,11.14,10.42,9.764,9.167,8.629,8.145,7.708,7.313};
        physicsVector = CreatePhysicsVector(E,Ge_18,factor);
     }
}

  else if( matName == namesMat[15] ) {
     if( ionZ == 3 ) {
        G4double Kr_3[31]={0.826,0.9063,1.029,1.113,1.17,1.211,1.241,1.265,1.282,1.324,1.328,1.317,1.298,1.247,1.188,1.13,1.076,1.026,0.9803,0.9389,0.7771,0.6654,0.5835,0.521,0.4315,0.3705,0.326,0.292,0.2652,0.2433,0.2252};
        physicsVector = CreatePhysicsVector(E,Kr_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Kr_4[31]={0.9936,1.106,1.292,1.432,1.534,1.611,1.671,1.72,1.759,1.872,1.917,1.931,1.929,1.894,1.838,1.775,1.71,1.647,1.588,1.532,1.304,1.135,1.007,0.9061,0.7574,0.6533,0.5763,0.517,0.4698,0.4313,0.3993};
        physicsVector = CreatePhysicsVector(E,Kr_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Kr_5[31]={1.119,1.256,1.495,1.689,1.84,1.958,2.052,2.128,2.192,2.393,2.492,2.543,2.567,2.565,2.525,2.466,2.399,2.33,2.262,2.197,1.913,1.692,1.518,1.377,1.163,1.01,0.8939,0.8036,0.7312,0.6718,0.6223};
        physicsVector = CreatePhysicsVector(E,Kr_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Kr_6[31]={1.219,1.375,1.656,1.896,2.094,2.254,2.384,2.492,2.583,2.882,3.043,3.138,3.195,3.24,3.227,3.182,3.121,3.051,2.98,2.908,2.583,2.318,2.101,1.921,1.641,1.434,1.275,1.15,1.048,0.9642,0.8938};
        physicsVector = CreatePhysicsVector(E,Kr_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Kr_7[31]={1.301,1.47,1.784,2.062,2.301,2.501,2.668,2.807,2.927,3.331,3.562,3.706,3.802,3.904,3.928,3.907,3.859,3.795,3.725,3.653,3.301,2.999,2.744,2.528,2.183,1.921,1.717,1.553,1.419,1.308,1.214};
        physicsVector = CreatePhysicsVector(E,Kr_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Kr_8[31]={1.38,1.559,1.897,2.204,2.479,2.716,2.918,3.091,3.239,3.753,4.057,4.256,4.394,4.559,4.628,4.637,4.608,4.556,4.491,4.421,4.054,3.723,3.435,3.186,2.78,2.465,2.214,2.01,1.842,1.7,1.58};
        physicsVector = CreatePhysicsVector(E,Kr_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Kr_9[31]={1.456,1.642,1.998,2.329,2.631,2.901,3.136,3.339,3.516,4.14,4.521,4.776,4.959,5.192,5.311,5.355,5.348,5.311,5.256,5.19,4.82,4.466,4.151,3.874,3.412,3.047,2.752,2.509,2.306,2.134,1.988};
        physicsVector = CreatePhysicsVector(E,Kr_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Kr_10[31]={1.533,1.724,2.094,2.443,2.767,3.064,3.329,3.561,3.765,4.499,4.958,5.273,5.503,5.809,5.981,6.064,6.086,6.068,6.025,5.967,5.602,5.232,4.894,4.592,4.08,3.668,3.329,3.048,2.81,2.608,2.433};
        physicsVector = CreatePhysicsVector(E,Kr_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Kr_11[31]={1.606,1.803,2.183,2.546,2.889,3.208,3.498,3.758,3.988,4.831,5.369,5.745,6.024,6.408,6.641,6.769,6.823,6.828,6.801,6.754,6.404,6.024,5.668,5.344,4.786,4.329,3.949,3.629,3.357,3.122,2.919};
        physicsVector = CreatePhysicsVector(E,Kr_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Kr_12[31]={1.684,1.886,2.275,2.649,3.005,3.341,3.653,3.937,4.193,5.141,5.757,6.193,6.522,6.984,7.278,7.454,7.546,7.577,7.569,7.535,7.205,6.818,6.447,6.104,5.506,5.008,4.589,4.233,3.928,3.664,3.433};
        physicsVector = CreatePhysicsVector(E,Kr_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Kr_13[31]={1.759,1.968,2.365,2.748,3.114,3.464,3.793,4.099,4.377,5.427,6.122,6.621,7,7.541,7.898,8.126,8.257,8.319,8.334,8.316,8.019,7.632,7.249,6.89,6.255,5.718,5.263,4.872,4.534,4.24,3.981};
        physicsVector = CreatePhysicsVector(E,Kr_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Kr_14[31]={1.833,2.049,2.455,2.845,3.22,3.581,3.924,4.247,4.544,5.69,6.464,7.024,7.454,8.075,8.496,8.777,8.951,9.046,9.085,9.087,8.831,8.447,8.056,7.684,7.016,6.445,5.955,5.531,5.162,4.839,4.553};
        physicsVector = CreatePhysicsVector(E,Kr_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Kr_15[31]={1.901,2.126,2.541,2.936,3.318,3.687,4.041,4.378,4.693,5.932,6.783,7.407,7.889,8.594,9.083,9.422,9.643,9.775,9.844,9.867,9.667,9.297,8.903,8.522,7.825,7.221,6.698,6.241,5.841,5.487,5.173};
        physicsVector = CreatePhysicsVector(E,Kr_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Kr_16[31]={1.97,2.204,2.631,3.033,3.421,3.796,4.16,4.509,4.838,6.164,7.089,7.776,8.311,9.102,9.66,10.06,10.33,10.5,10.6,10.65,10.51,10.16,9.764,9.376,8.656,8.021,7.466,6.978,6.547,6.164,5.823};
        physicsVector = CreatePhysicsVector(E,Kr_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Kr_17[31]={2.038,2.28,2.72,3.128,3.521,3.902,4.272,4.63,4.972,6.375,7.374,8.122,8.709,9.584,10.21,10.67,10.99,11.2,11.33,11.41,11.34,11.01,10.62,10.23,9.491,8.831,8.247,7.73,7.27,6.86,6.492};
        physicsVector = CreatePhysicsVector(E,Kr_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Kr_18[31]={2.11,2.362,2.817,3.234,3.633,4.019,4.394,4.759,5.111,6.587,7.656,8.465,9.104,10.06,10.76,11.27,11.64,11.9,12.06,12.16,12.17,11.86,11.47,11.07,10.31,9.628,9.016,8.472,7.987,7.551,7.158};
        physicsVector = CreatePhysicsVector(E,Kr_18,factor);
     }
}

  else if( matName == namesMat[16] ) {
     if( ionZ == 3 ) {
        G4double Mo_3[31]={0.6534,0.7272,0.8512,0.9467,1.019,1.073,1.114,1.147,1.173,1.235,1.244,1.236,1.22,1.177,1.13,1.083,1.037,0.9929,0.9519,0.9138,0.7605,0.6532,0.5743,0.5137,0.4269,0.3673,0.3238,0.2904,0.264,0.2424,0.2245};
        physicsVector = CreatePhysicsVector(E,Mo_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Mo_4[31]={0.7836,0.8807,1.052,1.196,1.312,1.406,1.48,1.542,1.592,1.741,1.796,1.812,1.81,1.782,1.739,1.689,1.638,1.586,1.534,1.485,1.272,1.112,0.9894,0.8922,0.7484,0.6472,0.5719,0.5138,0.4674,0.4295,0.3979};
        physicsVector = CreatePhysicsVector(E,Mo_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Mo_5[31]={0.8829,0.9968,1.205,1.389,1.547,1.68,1.791,1.883,1.962,2.215,2.331,2.385,2.408,2.408,2.379,2.337,2.287,2.232,2.176,2.12,1.862,1.654,1.489,1.354,1.148,0.9989,0.8861,0.7979,0.7269,0.6686,0.6199};
        physicsVector = CreatePhysicsVector(E,Mo_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Mo_6[31]={0.9635,1.089,1.326,1.541,1.735,1.905,2.05,2.175,2.283,2.649,2.84,2.942,2.997,3.038,3.034,3.006,2.964,2.913,2.857,2.798,2.509,2.262,2.056,1.885,1.616,1.417,1.262,1.14,1.041,0.9587,0.8896};
        physicsVector = CreatePhysicsVector(E,Mo_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Mo_7[31]={1.029,1.164,1.421,1.661,1.882,2.083,2.261,2.418,2.554,3.039,3.313,3.471,3.566,3.659,3.687,3.681,3.653,3.611,3.56,3.504,3.199,2.92,2.681,2.476,2.147,1.895,1.697,1.538,1.408,1.299,1.207};
        physicsVector = CreatePhysicsVector(E,Mo_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Mo_8[31]={1.092,1.232,1.505,1.764,2.007,2.234,2.441,2.627,2.792,3.394,3.757,3.978,4.118,4.273,4.34,4.361,4.352,4.323,4.281,4.23,3.922,3.619,3.351,3.116,2.73,2.428,2.186,1.988,1.825,1.687,1.57};
        physicsVector = CreatePhysicsVector(E,Mo_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Mo_9[31]={1.15,1.296,1.581,1.854,2.115,2.361,2.592,2.804,2.995,3.712,4.166,4.455,4.645,4.867,4.979,5.03,5.043,5.031,5,4.957,4.656,4.337,4.045,3.785,3.348,2.999,2.714,2.479,2.283,2.115,1.972};
        physicsVector = CreatePhysicsVector(E,Mo_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Mo_10[31]={1.205,1.357,1.651,1.937,2.211,2.473,2.722,2.956,3.171,3.998,4.543,4.904,5.148,5.446,5.606,5.692,5.731,5.738,5.722,5.689,5.406,5.076,4.765,4.483,4,3.606,3.281,3.01,2.78,2.583,2.413};
        physicsVector = CreatePhysicsVector(E,Mo_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Mo_11[31]={1.256,1.414,1.717,2.012,2.297,2.572,2.836,3.088,3.324,4.256,4.89,5.325,5.628,6.008,6.224,6.35,6.418,6.448,6.449,6.429,6.173,5.839,5.514,5.213,4.688,4.253,3.888,3.58,3.317,3.09,2.892};
        physicsVector = CreatePhysicsVector(E,Mo_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Mo_12[31]={1.308,1.471,1.783,2.086,2.381,2.666,2.942,3.208,3.462,4.49,5.211,5.721,6.083,6.547,6.822,6.991,7.092,7.147,7.167,7.163,6.939,6.606,6.269,5.952,5.39,4.917,4.517,4.175,3.88,3.624,3.4};
        physicsVector = CreatePhysicsVector(E,Mo_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Mo_13[31]={1.356,1.526,1.847,2.157,2.459,2.753,3.039,3.316,3.584,4.702,5.507,6.092,6.515,7.068,7.405,7.62,7.757,7.84,7.883,7.896,7.716,7.389,7.044,6.714,6.121,5.612,5.177,4.802,4.476,4.191,3.94};
        physicsVector = CreatePhysicsVector(E,Mo_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Mo_14[31]={1.403,1.58,1.91,2.226,2.534,2.835,3.129,3.415,3.694,4.892,5.777,6.436,6.922,7.567,7.967,8.231,8.405,8.517,8.585,8.618,8.491,8.174,7.825,7.485,6.863,6.323,5.855,5.449,5.094,4.782,4.505};
        physicsVector = CreatePhysicsVector(E,Mo_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Mo_15[31]={1.446,1.628,1.967,2.289,2.603,2.91,3.21,3.503,3.791,5.059,6.022,6.754,7.304,8.046,8.517,8.834,9.049,9.195,9.29,9.346,9.285,8.989,8.642,8.295,7.649,7.079,6.581,6.143,5.758,5.417,5.114};
        physicsVector = CreatePhysicsVector(E,Mo_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Mo_16[31]={1.49,1.677,2.028,2.357,2.676,2.988,3.294,3.594,3.889,5.22,6.255,7.057,7.672,8.515,9.059,9.43,9.689,9.87,9.993,10.07,10.08,9.812,9.47,9.119,8.453,7.857,7.33,6.863,6.45,6.081,5.751};
        physicsVector = CreatePhysicsVector(E,Mo_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Mo_17[31]={1.533,1.726,2.086,2.422,2.746,3.063,3.374,3.679,3.979,5.362,6.465,7.335,8.013,8.957,9.575,10,10.31,10.53,10.68,10.78,10.87,10.63,10.3,9.946,9.264,8.645,8.091,7.598,7.157,6.763,6.408};
        physicsVector = CreatePhysicsVector(E,Mo_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Mo_18[31]={1.583,1.781,2.153,2.497,2.827,3.149,3.465,3.775,4.08,5.508,6.675,7.61,8.351,9.397,10.09,10.58,10.92,11.18,11.36,11.49,11.66,11.44,11.12,10.76,10.07,9.424,8.846,8.327,7.862,7.443,7.065};
        physicsVector = CreatePhysicsVector(E,Mo_18,factor);
     }
}

  else if( matName == namesMat[17] ) {
     if( ionZ == 3 ) {
        G4double Ag_3[31]={0.616,0.695,0.8256,0.9274,1.006,1.064,1.108,1.141,1.165,1.222,1.224,1.207,1.184,1.131,1.079,1.031,0.9856,0.9444,0.9066,0.8719,0.7337,0.635,0.5609,0.5032,0.4193,0.3612,0.3185,0.2857,0.2596,0.2384,0.2208};
        physicsVector = CreatePhysicsVector(E,Ag_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Ag_4[31]={0.7283,0.8324,1.015,1.166,1.292,1.394,1.475,1.54,1.591,1.734,1.781,1.786,1.772,1.726,1.672,1.617,1.563,1.512,1.463,1.418,1.226,1.079,0.9641,0.8719,0.7337,0.6354,0.5619,0.505,0.4595,0.4223,0.3913};
        physicsVector = CreatePhysicsVector(E,Ag_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Ag_5[31]={0.8167,0.9362,1.158,1.352,1.52,1.664,1.785,1.886,1.969,2.219,2.326,2.367,2.374,2.348,2.3,2.246,2.19,2.133,2.079,2.026,1.792,1.602,1.448,1.32,1.123,0.9791,0.8694,0.7834,0.714,0.657,0.6092};
        physicsVector = CreatePhysicsVector(E,Ag_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Ag_6[31]={0.8963,1.023,1.271,1.499,1.702,1.884,2.043,2.18,2.297,2.669,2.85,2.937,2.974,2.98,2.948,2.901,2.847,2.79,2.733,2.676,2.413,2.188,1.998,1.836,1.579,1.387,1.237,1.118,1.021,0.9412,0.8736};
        physicsVector = CreatePhysicsVector(E,Ag_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Ag_7[31]={0.9709,1.099,1.362,1.615,1.847,2.06,2.252,2.424,2.574,3.076,3.34,3.483,3.558,3.608,3.6,3.567,3.521,3.468,3.412,3.354,3.074,2.822,2.601,2.409,2.095,1.853,1.661,1.507,1.38,1.274,1.184};
        physicsVector = CreatePhysicsVector(E,Ag_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Ag_8[31]={1.047,1.177,1.447,1.716,1.972,2.21,2.431,2.632,2.814,3.45,3.804,4.01,4.129,4.233,4.256,4.241,4.207,4.161,4.109,4.054,3.767,3.495,3.248,3.029,2.661,2.37,2.136,1.945,1.786,1.653,1.538};
        physicsVector = CreatePhysicsVector(E,Ag_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Ag_9[31]={1.12,1.254,1.527,1.807,2.08,2.339,2.582,2.809,3.019,3.786,4.234,4.508,4.676,4.843,4.902,4.91,4.891,4.855,4.809,4.758,4.472,4.186,3.92,3.677,3.262,2.926,2.651,2.424,2.233,2.071,1.931};
        physicsVector = CreatePhysicsVector(E,Ag_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Ag_10[31]={1.189,1.331,1.607,1.893,2.178,2.453,2.715,2.963,3.196,4.09,4.633,4.979,5.202,5.44,5.541,5.576,5.574,5.551,5.514,5.468,5.192,4.898,4.616,4.354,3.896,3.517,3.204,2.941,2.718,2.527,2.362};
        physicsVector = CreatePhysicsVector(E,Ag_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Ag_11[31]={1.251,1.402,1.685,1.974,2.267,2.555,2.832,3.098,3.35,4.362,5.003,5.424,5.705,6.023,6.173,6.24,6.26,6.252,6.226,6.189,5.928,5.632,5.338,5.06,4.563,4.145,3.794,3.496,3.241,3.021,2.829};
        physicsVector = CreatePhysicsVector(E,Ag_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Ag_12[31]={1.311,1.472,1.766,2.058,2.356,2.653,2.942,3.221,3.49,4.609,5.348,5.844,6.185,6.586,6.788,6.892,6.937,6.947,6.934,6.906,6.666,6.37,6.068,5.776,5.246,4.792,4.406,4.075,3.789,3.541,3.324};
        physicsVector = CreatePhysicsVector(E,Ag_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Ag_13[31]={1.366,1.537,1.845,2.14,2.441,2.744,3.043,3.334,3.615,4.83,5.666,6.24,6.644,7.133,7.392,7.535,7.608,7.638,7.641,7.624,7.413,7.124,6.817,6.515,5.956,5.468,5.048,4.685,4.37,4.094,3.851};
        physicsVector = CreatePhysicsVector(E,Ag_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Ag_14[31]={1.419,1.597,1.922,2.221,2.525,2.832,3.138,3.438,3.73,5.028,5.958,6.61,7.077,7.659,7.978,8.163,8.266,8.319,8.338,8.336,8.16,7.88,7.572,7.262,6.678,6.16,5.709,5.316,4.972,4.67,4.402};
        physicsVector = CreatePhysicsVector(E,Ag_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Ag_15[31]={1.465,1.65,1.99,2.298,2.603,2.912,3.223,3.53,3.831,5.199,6.222,6.951,7.483,8.164,8.55,8.782,8.92,8.999,9.039,9.052,8.924,8.662,8.357,8.043,7.439,6.894,6.413,5.99,5.617,5.287,4.993};
        physicsVector = CreatePhysicsVector(E,Ag_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Ag_16[31]={1.511,1.703,2.059,2.377,2.685,2.997,3.312,3.625,3.933,5.361,6.471,7.277,7.875,8.659,9.115,9.397,9.572,9.678,9.74,9.771,9.692,9.452,9.154,8.839,8.218,7.648,7.14,6.689,6.289,5.932,5.613};
        physicsVector = CreatePhysicsVector(E,Ag_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Ag_17[31]={1.555,1.753,2.123,2.454,2.767,3.08,3.397,3.714,4.028,5.505,6.696,7.578,8.242,9.129,9.659,9.994,10.21,10.34,10.43,10.48,10.45,10.24,9.951,9.637,9.004,8.413,7.88,7.403,6.976,6.595,6.251};
        physicsVector = CreatePhysicsVector(E,Ag_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Ag_18[31]={1.605,1.81,2.193,2.539,2.858,3.175,3.494,3.814,4.133,5.653,6.919,7.875,8.603,9.596,10.2,10.59,10.84,11.01,11.12,11.18,11.21,11.02,10.74,10.43,9.784,9.172,8.615,8.113,7.663,7.258,6.892};
        physicsVector = CreatePhysicsVector(E,Ag_18,factor);
     }
}

  else if( matName == namesMat[18] ) {
     if( ionZ == 3 ) {
        G4double Sn_3[31]={0.5096,0.5567,0.632,0.6872,0.728,0.7589,0.7833,0.8031,0.8194,0.8713,0.8964,0.9074,0.9098,0.8992,0.8778,0.8518,0.8241,0.7964,0.7696,0.744,0.6366,0.5568,0.4957,0.4473,0.3757,0.3253,0.2879,0.259,0.2361,0.2174,0.2018};
        physicsVector = CreatePhysicsVector(E,Sn_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Sn_4[31]={0.6712,0.7388,0.8518,0.9402,1.009,1.063,1.107,1.143,1.173,1.273,1.328,1.36,1.376,1.382,1.368,1.343,1.313,1.281,1.247,1.214,1.066,0.9472,0.8523,0.7751,0.6573,0.5722,0.508,0.4579,0.4179,0.3851,0.3577};
        physicsVector = CreatePhysicsVector(E,Sn_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Sn_5[31]={0.8182,0.904,1.052,1.173,1.271,1.351,1.416,1.471,1.517,1.674,1.766,1.824,1.86,1.892,1.893,1.876,1.848,1.815,1.779,1.742,1.562,1.409,1.282,1.175,1.007,0.8819,0.7861,0.7105,0.6494,0.5991,0.5569};
        physicsVector = CreatePhysicsVector(E,Sn_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Sn_6[31]={0.9537,1.056,1.235,1.386,1.513,1.619,1.708,1.782,1.846,2.067,2.201,2.289,2.349,2.416,2.438,2.433,2.413,2.383,2.348,2.309,2.109,1.928,1.771,1.635,1.417,1.25,1.119,1.015,0.9293,0.8585,0.7988};
        physicsVector = CreatePhysicsVector(E,Sn_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Sn_7[31]={1.078,1.194,1.401,1.579,1.733,1.864,1.976,2.072,2.155,2.445,2.624,2.747,2.835,2.941,2.99,3.004,2.995,2.973,2.941,2.905,2.696,2.492,2.31,2.148,1.881,1.671,1.504,1.368,1.256,1.162,1.083};
        physicsVector = CreatePhysicsVector(E,Sn_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Sn_8[31]={1.198,1.327,1.559,1.762,1.94,2.096,2.231,2.348,2.45,2.813,3.042,3.202,3.319,3.47,3.55,3.585,3.592,3.579,3.555,3.522,3.312,3.093,2.889,2.705,2.392,2.14,1.935,1.767,1.627,1.508,1.407};
        physicsVector = CreatePhysicsVector(E,Sn_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Sn_9[31]={1.311,1.452,1.705,1.93,2.13,2.308,2.465,2.603,2.725,3.164,3.444,3.643,3.791,3.99,4.104,4.164,4.188,4.187,4.171,4.145,3.942,3.713,3.492,3.289,2.935,2.644,2.403,2.203,2.035,1.891,1.768};
        physicsVector = CreatePhysicsVector(E,Sn_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Sn_10[31]={1.42,1.571,1.845,2.089,2.309,2.507,2.684,2.842,2.983,3.499,3.833,4.073,4.254,4.504,4.655,4.742,4.786,4.8,4.795,4.776,4.587,4.353,4.12,3.9,3.509,3.181,2.906,2.675,2.478,2.309,2.162};
        physicsVector = CreatePhysicsVector(E,Sn_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Sn_11[31]={1.52,1.681,1.974,2.235,2.473,2.689,2.885,3.062,3.222,3.816,4.206,4.488,4.704,5.008,5.199,5.317,5.384,5.417,5.424,5.415,5.248,5.013,4.772,4.539,4.116,3.753,3.445,3.182,2.957,2.762,2.592};
        physicsVector = CreatePhysicsVector(E,Sn_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Sn_12[31]={1.612,1.782,2.091,2.368,2.621,2.853,3.065,3.26,3.437,4.108,4.554,4.879,5.13,5.488,5.722,5.874,5.967,6.019,6.042,6.045,5.906,5.675,5.429,5.186,4.735,4.342,4.004,3.712,3.459,3.239,3.047};
        physicsVector = CreatePhysicsVector(E,Sn_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Sn_13[31]={1.705,1.883,2.208,2.5,2.767,3.014,3.242,3.453,3.646,4.392,4.896,5.266,5.552,5.968,6.246,6.432,6.552,6.625,6.665,6.681,6.576,6.354,6.105,5.855,5.38,4.959,4.591,4.271,3.992,3.747,3.531};
        physicsVector = CreatePhysicsVector(E,Sn_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Sn_14[31]={1.793,1.98,2.32,2.626,2.906,3.166,3.408,3.633,3.842,4.66,5.222,5.637,5.961,6.434,6.758,6.979,7.128,7.224,7.282,7.312,7.246,7.036,6.788,6.532,6.038,5.591,5.197,4.85,4.545,4.276,4.038};
        physicsVector = CreatePhysicsVector(E,Sn_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Sn_15[31]={1.874,2.07,2.424,2.742,3.035,3.307,3.561,3.799,4.021,4.909,5.529,5.991,6.353,6.889,7.26,7.521,7.702,7.823,7.902,7.949,7.931,7.742,7.499,7.241,6.732,6.262,5.841,5.468,5.138,4.844,4.583};
        physicsVector = CreatePhysicsVector(E,Sn_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Sn_16[31]={1.954,2.158,2.527,2.858,3.163,3.446,3.712,3.962,4.197,5.152,5.83,6.338,6.74,7.339,7.761,8.062,8.275,8.423,8.524,8.588,8.624,8.456,8.222,7.965,7.443,6.952,6.508,6.11,5.755,5.438,5.154};
        physicsVector = CreatePhysicsVector(E,Sn_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Sn_17[31]={2.03,2.242,2.625,2.969,3.285,3.579,3.855,4.116,4.363,5.381,6.116,6.671,7.112,7.776,8.248,8.59,8.837,9.013,9.136,9.219,9.311,9.168,8.945,8.69,8.161,7.653,7.187,6.767,6.389,6.049,5.743};
        physicsVector = CreatePhysicsVector(E,Sn_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Sn_18[31]={2.114,2.334,2.732,3.089,3.417,3.722,4.009,4.28,4.538,5.618,6.409,7.012,7.493,8.22,8.742,9.124,9.404,9.607,9.752,9.853,9.998,9.878,9.664,9.411,8.874,8.349,7.863,7.421,7.022,6.661,6.335};
        physicsVector = CreatePhysicsVector(E,Sn_18,factor);
     }
}

  else if( matName == namesMat[19] ) {
     if( ionZ == 3 ) {
        G4double Xe_3[31]={0.5617,0.6027,0.6687,0.7189,0.7574,0.7872,0.8104,0.8285,0.8426,0.8788,0.8879,0.8864,0.88,0.8595,0.8342,0.807,0.7794,0.7523,0.7263,0.7016,0.5984,0.5227,0.4653,0.4202,0.3538,0.3069,0.272,0.245,0.2235,0.2058,0.1912};
        physicsVector = CreatePhysicsVector(E,Xe_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Xe_4[31]={0.7468,0.8068,0.9059,0.9845,1.048,1.099,1.14,1.174,1.202,1.285,1.32,1.334,1.337,1.327,1.305,1.277,1.246,1.213,1.18,1.147,1.003,0.8898,0.8004,0.7282,0.6187,0.5396,0.4798,0.433,0.3954,0.3646,0.3388};
        physicsVector = CreatePhysicsVector(E,Xe_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Xe_5[31]={0.9138,0.9925,1.125,1.233,1.321,1.395,1.457,1.509,1.552,1.691,1.759,1.795,1.813,1.822,1.81,1.787,1.757,1.722,1.686,1.649,1.473,1.325,1.204,1.104,0.9474,0.8314,0.7421,0.6715,0.6143,0.567,0.5273};
        physicsVector = CreatePhysicsVector(E,Xe_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Xe_6[31]={1.065,1.162,1.327,1.463,1.578,1.675,1.758,1.829,1.889,2.09,2.197,2.259,2.297,2.332,2.336,2.322,2.297,2.265,2.229,2.19,1.991,1.814,1.664,1.537,1.333,1.178,1.056,0.9584,0.8786,0.8122,0.7561};
        physicsVector = CreatePhysicsVector(E,Xe_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Xe_7[31]={1.201,1.315,1.51,1.675,1.814,1.935,2.039,2.129,2.207,2.475,2.625,2.717,2.778,2.845,2.871,2.872,2.856,2.829,2.796,2.758,2.547,2.348,2.172,2.02,1.769,1.574,1.418,1.292,1.187,1.099,1.025};
        physicsVector = CreatePhysicsVector(E,Xe_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Xe_8[31]={1.329,1.459,1.685,1.875,2.04,2.183,2.307,2.417,2.513,2.851,3.048,3.174,3.26,3.364,3.414,3.432,3.429,3.411,3.382,3.347,3.133,2.916,2.719,2.544,2.25,2.015,1.825,1.668,1.537,1.426,1.331};
        physicsVector = CreatePhysicsVector(E,Xe_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Xe_9[31]={1.448,1.593,1.845,2.061,2.249,2.413,2.559,2.687,2.801,3.212,3.458,3.619,3.732,3.875,3.953,3.991,4.002,3.994,3.973,3.943,3.733,3.504,3.29,3.095,2.762,2.49,2.266,2.079,1.922,1.787,1.672};
        physicsVector = CreatePhysicsVector(E,Xe_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Xe_10[31]={1.56,1.718,1.997,2.236,2.446,2.631,2.796,2.944,3.075,3.558,3.856,4.055,4.197,4.382,4.49,4.55,4.578,4.583,4.571,4.548,4.348,4.112,3.883,3.673,3.304,2.997,2.741,2.524,2.34,2.182,2.045};
        physicsVector = CreatePhysicsVector(E,Xe_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Xe_11[31]={1.664,1.834,2.136,2.398,2.629,2.834,3.018,3.183,3.332,3.888,4.239,4.478,4.65,4.881,5.022,5.108,5.155,5.175,5.175,5.16,4.979,4.74,4.501,4.276,3.876,3.536,3.249,3.003,2.792,2.609,2.45};
        physicsVector = CreatePhysicsVector(E,Xe_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Xe_12[31]={1.759,1.94,2.263,2.545,2.795,3.019,3.22,3.402,3.567,4.195,4.6,4.879,5.082,5.359,5.535,5.649,5.718,5.755,5.768,5.764,5.607,5.37,5.125,4.889,4.461,4.093,3.776,3.503,3.267,3.061,2.88};
        physicsVector = CreatePhysicsVector(E,Xe_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Xe_13[31]={1.854,2.046,2.389,2.69,2.958,3.199,3.418,3.616,3.797,4.495,4.955,5.276,5.512,5.838,6.05,6.192,6.284,6.339,6.367,6.375,6.248,6.017,5.767,5.522,5.071,4.675,4.331,4.031,3.77,3.54,3.337};
        physicsVector = CreatePhysicsVector(E,Xe_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Xe_14[31]={1.946,2.147,2.508,2.828,3.113,3.371,3.605,3.819,4.015,4.781,5.295,5.659,5.929,6.305,6.555,6.726,6.842,6.916,6.959,6.979,6.889,6.667,6.416,6.165,5.693,5.273,4.903,4.578,4.293,4.04,3.817};
        physicsVector = CreatePhysicsVector(E,Xe_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Xe_15[31]={2.032,2.241,2.619,2.955,3.257,3.53,3.779,4.008,4.218,5.049,5.619,6.026,6.331,6.762,7.052,7.256,7.398,7.495,7.557,7.592,7.545,7.339,7.091,6.837,6.348,5.905,5.511,5.161,4.852,4.577,4.331};
        physicsVector = CreatePhysicsVector(E,Xe_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Xe_16[31]={2.116,2.335,2.729,3.08,3.397,3.686,3.949,4.192,4.416,5.311,5.935,6.386,6.728,7.214,7.547,7.784,7.954,8.073,8.154,8.206,8.206,8.02,7.778,7.522,7.021,6.557,6.14,5.767,5.435,5.137,4.87};
        physicsVector = CreatePhysicsVector(E,Xe_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Xe_17[31]={2.199,2.425,2.835,3.2,3.532,3.834,4.111,4.367,4.604,5.561,6.238,6.733,7.111,7.655,8.03,8.302,8.5,8.643,8.744,8.812,8.864,8.699,8.465,8.211,7.7,7.22,6.782,6.387,6.033,5.714,5.427};
        physicsVector = CreatePhysicsVector(E,Xe_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Xe_18[31]={2.288,2.523,2.948,3.329,3.675,3.991,4.282,4.551,4.801,5.818,6.548,7.088,7.503,8.104,8.522,8.826,9.052,9.217,9.337,9.421,9.521,9.377,9.15,8.896,8.375,7.879,7.422,7.007,6.632,6.293,5.987};
        physicsVector = CreatePhysicsVector(E,Xe_18,factor);
     }
}

  else if( matName == namesMat[20] ) {
     if( ionZ == 3 ) {
        G4double W_3[31]={0.2439,0.2673,0.3038,0.3307,0.3513,0.368,0.3819,0.3938,0.4041,0.4399,0.4601,0.4714,0.4774,0.4805,0.4778,0.4725,0.4657,0.4583,0.4506,0.4428,0.4054,0.3722,0.3432,0.3182,0.2778,0.2471,0.2231,0.2039,0.1882,0.1751,0.164};
        physicsVector = CreatePhysicsVector(E,W_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double W_4[31]={0.3226,0.3566,0.4129,0.4564,0.4909,0.5191,0.543,0.5637,0.5818,0.648,0.6891,0.7156,0.7327,0.7499,0.7556,0.7553,0.7512,0.7447,0.7368,0.7281,0.6801,0.6322,0.5883,0.5491,0.4837,0.4326,0.392,0.3592,0.332,0.3093,0.2899};
        physicsVector = CreatePhysicsVector(E,W_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double W_5[31]={0.3961,0.4392,0.514,0.5747,0.6241,0.6653,0.7005,0.7311,0.7582,0.8591,0.925,0.9703,1.002,1.04,1.059,1.067,1.069,1.066,1.06,1.052,1,0.9409,0.8833,0.8299,0.7381,0.6641,0.6042,0.5551,0.5141,0.4795,0.4499};
        physicsVector = CreatePhysicsVector(E,W_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double W_6[31]={0.466,0.5168,0.608,0.6852,0.7498,0.8046,0.8517,0.893,0.9298,1.069,1.162,1.229,1.278,1.342,1.379,1.399,1.409,1.412,1.41,1.405,1.355,1.289,1.22,1.154,1.036,0.9378,0.8569,0.7896,0.7329,0.6847,0.6431};
        physicsVector = CreatePhysicsVector(E,W_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double W_7[31]={0.5322,0.5896,0.695,0.7871,0.8664,0.9347,0.9941,1.047,1.093,1.273,1.396,1.487,1.556,1.65,1.708,1.743,1.764,1.775,1.779,1.779,1.738,1.669,1.592,1.514,1.372,1.25,1.147,1.061,0.9871,0.9237,0.8687};
        physicsVector = CreatePhysicsVector(E,W_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double W_8[31]={0.6002,0.664,0.7818,0.8875,0.9807,1.062,1.134,1.198,1.255,1.477,1.632,1.748,1.837,1.964,2.046,2.099,2.132,2.153,2.165,2.17,2.144,2.075,1.993,1.906,1.742,1.597,1.473,1.366,1.274,1.195,1.125};
        physicsVector = CreatePhysicsVector(E,W_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double W_9[31]={0.6666,0.7365,0.8655,0.9831,1.089,1.183,1.267,1.342,1.41,1.674,1.862,2.004,2.116,2.277,2.384,2.456,2.504,2.536,2.557,2.569,2.561,2.497,2.412,2.319,2.136,1.97,1.825,1.699,1.59,1.494,1.41};
        physicsVector = CreatePhysicsVector(E,W_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double W_10[31]={0.7327,0.8086,0.9483,1.076,1.194,1.3,1.395,1.481,1.559,1.867,2.088,2.258,2.392,2.59,2.724,2.817,2.881,2.926,2.956,2.977,2.992,2.936,2.85,2.753,2.554,2.369,2.205,2.06,1.932,1.82,1.721};
        physicsVector = CreatePhysicsVector(E,W_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double W_11[31]={0.7964,0.8785,1.028,1.166,1.294,1.411,1.517,1.614,1.702,2.053,2.308,2.506,2.663,2.898,3.062,3.178,3.261,3.32,3.362,3.392,3.435,3.39,3.307,3.207,2.995,2.793,2.61,2.446,2.301,2.173,2.058};
        physicsVector = CreatePhysicsVector(E,W_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double W_12[31]={0.8567,0.9447,1.104,1.251,1.387,1.514,1.63,1.737,1.835,2.228,2.517,2.742,2.923,3.195,3.389,3.531,3.634,3.709,3.764,3.805,3.879,3.848,3.769,3.669,3.448,3.231,3.031,2.851,2.689,2.545,2.415};
        physicsVector = CreatePhysicsVector(E,W_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double W_13[31]={0.9191,1.013,1.183,1.338,1.483,1.618,1.744,1.86,1.968,2.402,2.725,2.979,3.184,3.496,3.721,3.888,4.012,4.103,4.172,4.224,4.333,4.318,4.247,4.148,3.92,3.689,3.474,3.277,3.1,2.94,2.795};
        physicsVector = CreatePhysicsVector(E,W_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double W_14[31]={0.9809,1.081,1.262,1.425,1.577,1.721,1.856,1.981,2.097,2.572,2.928,3.21,3.439,3.791,4.049,4.242,4.386,4.496,4.578,4.642,4.79,4.794,4.732,4.636,4.403,4.161,3.931,3.719,3.527,3.352,3.193};
        physicsVector = CreatePhysicsVector(E,W_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double W_15[31]={1.039,1.146,1.336,1.508,1.667,1.818,1.96,2.094,2.218,2.732,3.121,3.431,3.686,4.079,4.371,4.592,4.761,4.889,4.988,5.065,5.257,5.285,5.234,5.144,4.91,4.658,4.415,4.189,3.981,3.791,3.618};
        physicsVector = CreatePhysicsVector(E,W_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double W_16[31]={1.098,1.211,1.412,1.592,1.759,1.917,2.067,2.208,2.34,2.893,3.314,3.653,3.932,4.368,4.695,4.945,5.137,5.286,5.401,5.492,5.731,5.783,5.747,5.663,5.43,5.171,4.916,4.676,4.454,4.25,4.062};
        physicsVector = CreatePhysicsVector(E,W_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double W_17[31]={1.154,1.273,1.486,1.674,1.848,2.013,2.169,2.317,2.457,3.046,3.499,3.865,4.169,4.649,5.01,5.29,5.507,5.676,5.809,5.915,6.202,6.282,6.262,6.186,5.957,5.692,5.428,5.175,4.94,4.722,4.521};
        physicsVector = CreatePhysicsVector(E,W_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double W_18[31]={1.219,1.344,1.569,1.767,1.949,2.12,2.283,2.438,2.586,3.211,3.696,4.09,4.419,4.94,5.336,5.643,5.884,6.073,6.223,6.343,6.677,6.782,6.776,6.709,6.484,6.215,5.94,5.677,5.429,5.199,4.985};
        physicsVector = CreatePhysicsVector(E,W_18,factor);
     }
}

  else if( matName == namesMat[21] ) {
     if( ionZ == 3 ) {
        G4double Pt_3[31]={0.2992,0.3235,0.3633,0.3948,0.4203,0.4415,0.4596,0.4752,0.4888,0.5363,0.5617,0.5743,0.5791,0.5764,0.5666,0.5541,0.5405,0.527,0.5137,0.5011,0.446,0.4022,0.3663,0.3365,0.2897,0.2551,0.2285,0.2074,0.1904,0.1763,0.1644};
        physicsVector = CreatePhysicsVector(E,Pt_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Pt_4[31]={0.4136,0.4491,0.5084,0.5565,0.5962,0.6298,0.6587,0.6839,0.7063,0.7881,0.8373,0.8667,0.8831,0.8938,0.8903,0.8805,0.8672,0.8523,0.8367,0.821,0.7467,0.6824,0.6274,0.5802,0.5043,0.4465,0.4015,0.3655,0.336,0.3115,0.2908};
        physicsVector = CreatePhysicsVector(E,Pt_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Pt_5[31]={0.5215,0.568,0.6469,0.712,0.7668,0.8137,0.8544,0.8903,0.9224,1.042,1.12,1.17,1.202,1.233,1.241,1.238,1.228,1.214,1.199,1.182,1.095,1.014,0.9405,0.8758,0.7687,0.6851,0.6186,0.5648,0.5204,0.4832,0.4516};
        physicsVector = CreatePhysicsVector(E,Pt_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Pt_6[31]={0.6218,0.679,0.7771,0.8591,0.9292,0.9899,1.043,1.09,1.132,1.294,1.402,1.475,1.526,1.584,1.608,1.615,1.611,1.601,1.588,1.572,1.48,1.385,1.296,1.215,1.077,0.9665,0.8768,0.8032,0.7419,0.6901,0.6458};
        physicsVector = CreatePhysicsVector(E,Pt_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Pt_7[31]={0.7138,0.781,0.8974,0.9958,1.081,1.155,1.221,1.28,1.333,1.538,1.679,1.779,1.851,1.939,1.984,2.004,2.01,2.006,1.997,1.984,1.893,1.79,1.688,1.593,1.425,1.287,1.173,1.079,0.9988,0.931,0.8725};
        physicsVector = CreatePhysicsVector(E,Pt_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Pt_8[31]={0.803,0.8796,1.014,1.128,1.228,1.316,1.395,1.466,1.529,1.779,1.955,2.083,2.177,2.3,2.368,2.404,2.42,2.424,2.421,2.412,2.328,2.22,2.109,2.001,1.807,1.642,1.504,1.388,1.289,1.204,1.13};
        physicsVector = CreatePhysicsVector(E,Pt_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Pt_9[31]={0.8862,0.9718,1.122,1.252,1.366,1.468,1.559,1.641,1.716,2.012,2.223,2.38,2.499,2.658,2.751,2.804,2.834,2.847,2.851,2.847,2.775,2.666,2.549,2.431,2.214,2.025,1.863,1.726,1.608,1.505,1.416};
        physicsVector = CreatePhysicsVector(E,Pt_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Pt_10[31]={0.9654,1.059,1.226,1.37,1.498,1.612,1.716,1.81,1.896,2.238,2.485,2.672,2.816,3.014,3.134,3.207,3.251,3.275,3.287,3.29,3.234,3.128,3.007,2.882,2.644,2.432,2.249,2.091,1.953,1.833,1.728};
        physicsVector = CreatePhysicsVector(E,Pt_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Pt_11[31]={1.04,1.141,1.322,1.481,1.621,1.748,1.864,1.969,2.066,2.455,2.74,2.957,3.127,3.365,3.515,3.61,3.671,3.708,3.729,3.739,3.704,3.605,3.482,3.352,3.096,2.864,2.66,2.481,2.325,2.187,2.066};
        physicsVector = CreatePhysicsVector(E,Pt_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Pt_12[31]={1.108,1.217,1.411,1.582,1.735,1.874,2,2.116,2.224,2.658,2.98,3.228,3.424,3.702,3.884,4.004,4.083,4.134,4.166,4.185,4.175,4.085,3.964,3.83,3.561,3.311,3.088,2.89,2.716,2.561,2.424};
        physicsVector = CreatePhysicsVector(E,Pt_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Pt_13[31]={1.176,1.292,1.499,1.683,1.847,1.997,2.135,2.262,2.379,2.86,3.218,3.498,3.72,4.042,4.255,4.4,4.498,4.564,4.608,4.637,4.655,4.577,4.46,4.325,4.044,3.778,3.537,3.321,3.129,2.958,2.804};
        physicsVector = CreatePhysicsVector(E,Pt_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Pt_14[31]={1.242,1.365,1.585,1.779,1.955,2.116,2.264,2.401,2.528,3.053,3.448,3.759,4.008,4.375,4.622,4.792,4.91,4.992,5.048,5.087,5.137,5.073,4.961,4.827,4.538,4.258,4,3.767,3.559,3.371,3.203};
        physicsVector = CreatePhysicsVector(E,Pt_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Pt_15[31]={1.303,1.432,1.663,1.869,2.055,2.226,2.383,2.53,2.667,3.235,3.666,4.008,4.285,4.698,4.981,5.18,5.32,5.419,5.49,5.541,5.628,5.583,5.48,5.349,5.055,4.762,4.489,4.24,4.015,3.811,3.627};
        physicsVector = CreatePhysicsVector(E,Pt_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Pt_16[31]={1.363,1.498,1.741,1.957,2.154,2.334,2.501,2.657,2.804,3.414,3.882,4.255,4.56,5.019,5.34,5.568,5.731,5.849,5.935,5.998,6.124,6.1,6.008,5.88,5.584,5.281,4.994,4.73,4.489,4.27,4.071};
        physicsVector = CreatePhysicsVector(E,Pt_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Pt_17[31]={1.42,1.561,1.815,2.041,2.247,2.437,2.613,2.778,2.933,3.584,4.087,4.492,4.824,5.331,5.69,5.947,6.134,6.271,6.373,6.449,6.618,6.616,6.537,6.416,6.121,5.809,5.51,5.232,4.977,4.743,4.53};
        physicsVector = CreatePhysicsVector(E,Pt_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Pt_18[31]={1.484,1.632,1.897,2.133,2.349,2.548,2.733,2.907,3.071,3.763,4.302,4.738,5.097,5.651,6.047,6.333,6.543,6.699,6.816,6.905,7.113,7.133,7.065,6.95,6.656,6.338,6.028,5.737,5.468,5.221,4.994};
        physicsVector = CreatePhysicsVector(E,Pt_18,factor);
     }
}

  else if( matName == namesMat[22] ) {
     if( ionZ == 3 ) {
        G4double Au_3[31]={0.3146,0.3401,0.3821,0.4151,0.4417,0.4636,0.4822,0.4981,0.512,0.5601,0.5855,0.5977,0.6018,0.5976,0.5859,0.5715,0.5563,0.5412,0.5266,0.5127,0.4536,0.4077,0.3708,0.3403,0.2927,0.2576,0.2306,0.2093,0.192,0.1778,0.1657};
        physicsVector = CreatePhysicsVector(E,Au_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Au_4[31]={0.4344,0.4717,0.5341,0.5845,0.6261,0.6609,0.6907,0.7167,0.7395,0.8225,0.8719,0.9009,0.9167,0.9255,0.9198,0.9076,0.8921,0.875,0.8575,0.84,0.7597,0.6921,0.6352,0.5868,0.5095,0.4509,0.4052,0.3687,0.3389,0.3142,0.2932};
        physicsVector = CreatePhysicsVector(E,Au_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Au_5[31]={0.5472,0.5962,0.6791,0.7473,0.8046,0.8534,0.8955,0.9324,0.9652,1.087,1.165,1.215,1.246,1.276,1.281,1.275,1.263,1.246,1.228,1.209,1.114,1.028,0.9523,0.8859,0.7766,0.6917,0.6243,0.5698,0.5249,0.4873,0.4553};
        physicsVector = CreatePhysicsVector(E,Au_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Au_6[31]={0.6522,0.7123,0.8152,0.9011,0.9743,1.037,1.093,1.141,1.185,1.349,1.457,1.531,1.581,1.637,1.659,1.663,1.656,1.643,1.627,1.608,1.506,1.405,1.313,1.23,1.088,0.9758,0.8848,0.8102,0.7482,0.6958,0.651};
        physicsVector = CreatePhysicsVector(E,Au_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Au_7[31]={0.7484,0.8189,0.9409,1.044,1.133,1.21,1.279,1.339,1.394,1.602,1.744,1.844,1.916,2.003,2.045,2.062,2.064,2.057,2.045,2.029,1.926,1.816,1.71,1.611,1.44,1.299,1.184,1.088,1.007,0.9385,0.8794};
        physicsVector = CreatePhysicsVector(E,Au_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Au_8[31]={0.8412,0.9217,1.062,1.182,1.286,1.378,1.459,1.532,1.598,1.853,2.03,2.158,2.253,2.374,2.439,2.472,2.485,2.486,2.479,2.466,2.369,2.253,2.136,2.025,1.826,1.658,1.518,1.4,1.3,1.214,1.139};
        physicsVector = CreatePhysicsVector(E,Au_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Au_9[31]={0.9278,1.018,1.175,1.311,1.43,1.536,1.63,1.715,1.793,2.094,2.307,2.465,2.584,2.742,2.833,2.883,2.909,2.919,2.919,2.911,2.824,2.706,2.582,2.46,2.237,2.044,1.88,1.74,1.621,1.517,1.427};
        physicsVector = CreatePhysicsVector(E,Au_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Au_10[31]={1.01,1.109,1.283,1.433,1.567,1.686,1.793,1.89,1.979,2.329,2.579,2.767,2.911,3.108,3.227,3.297,3.337,3.357,3.365,3.364,3.291,3.175,3.046,2.916,2.671,2.456,2.269,2.108,1.969,1.848,1.741};
        physicsVector = CreatePhysicsVector(E,Au_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Au_11[31]={1.087,1.194,1.383,1.548,1.695,1.827,1.946,2.056,2.156,2.553,2.842,3.061,3.231,3.469,3.617,3.71,3.766,3.799,3.817,3.823,3.77,3.659,3.528,3.392,3.129,2.892,2.684,2.502,2.344,2.204,2.081};
        physicsVector = CreatePhysicsVector(E,Au_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Au_12[31]={1.158,1.272,1.475,1.653,1.813,1.957,2.088,2.208,2.319,2.764,3.09,3.341,3.537,3.816,3.996,4.113,4.188,4.236,4.264,4.279,4.25,4.147,4.016,3.877,3.599,3.343,3.116,2.915,2.738,2.581,2.442};
        physicsVector = CreatePhysicsVector(E,Au_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Au_13[31]={1.228,1.349,1.566,1.757,1.929,2.084,2.227,2.358,2.48,2.972,3.336,3.618,3.842,4.165,4.377,4.519,4.614,4.676,4.716,4.74,4.739,4.646,4.519,4.377,4.087,3.815,3.568,3.349,3.154,2.981,2.825};
        physicsVector = CreatePhysicsVector(E,Au_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Au_14[31]={1.296,1.424,1.653,1.856,2.039,2.206,2.36,2.502,2.633,3.171,3.573,3.887,4.139,4.506,4.753,4.921,5.035,5.113,5.165,5.2,5.229,5.15,5.028,4.886,4.587,4.299,4.036,3.8,3.588,3.398,3.227};
        physicsVector = CreatePhysicsVector(E,Au_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Au_15[31]={1.358,1.493,1.734,1.949,2.142,2.32,2.483,2.635,2.776,3.359,3.798,4.145,4.424,4.838,5.122,5.318,5.455,5.551,5.617,5.663,5.729,5.668,5.554,5.414,5.109,4.809,4.53,4.276,4.047,3.841,3.654};
        physicsVector = CreatePhysicsVector(E,Au_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Au_16[31]={1.42,1.561,1.814,2.039,2.244,2.431,2.605,2.766,2.917,3.544,4.021,4.399,4.706,5.168,5.489,5.715,5.875,5.989,6.071,6.13,6.234,6.192,6.088,5.952,5.645,5.333,5.04,4.771,4.526,4.303,4.101};
        physicsVector = CreatePhysicsVector(E,Au_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Au_17[31]={1.479,1.626,1.89,2.125,2.339,2.536,2.719,2.89,3.05,3.719,4.232,4.642,4.978,5.489,5.848,6.104,6.288,6.422,6.519,6.591,6.737,6.717,6.624,6.494,6.187,5.867,5.561,5.277,5.017,4.78,4.563};
        physicsVector = CreatePhysicsVector(E,Au_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Au_18[31]={1.543,1.697,1.973,2.219,2.443,2.65,2.842,3.022,3.191,3.903,4.453,4.895,5.259,5.817,6.213,6.499,6.706,6.859,6.971,7.056,7.241,7.241,7.16,7.036,6.729,6.401,6.083,5.787,5.513,5.261,5.031};
        physicsVector = CreatePhysicsVector(E,Au_18,factor);
     }
}

  else if( matName == namesMat[23] ) {
     if( ionZ == 3 ) {
        G4double Pb_3[31]={0.3483,0.3786,0.426,0.4605,0.4861,0.5059,0.5217,0.5346,0.5454,0.5801,0.5962,0.602,0.6016,0.5907,0.5742,0.5559,0.5375,0.5197,0.5029,0.4873,0.4243,0.3787,0.3436,0.3154,0.2722,0.2406,0.2163,0.1971,0.1814,0.1684,0.1574};
        physicsVector = CreatePhysicsVector(E,Pb_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double Pb_4[31]={0.4635,0.5078,0.5801,0.6356,0.6786,0.7126,0.7402,0.7631,0.7826,0.8488,0.8853,0.9051,0.9142,0.9137,0.9012,0.8835,0.8633,0.8423,0.8215,0.8013,0.714,0.646,0.5913,0.5461,0.4751,0.4219,0.3805,0.3473,0.3202,0.2976,0.2784};
        physicsVector = CreatePhysicsVector(E,Pb_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double Pb_5[31]={0.5692,0.6265,0.7228,0.7996,0.8612,0.911,0.9521,0.9866,1.016,1.119,1.18,1.218,1.241,1.258,1.255,1.242,1.224,1.202,1.18,1.157,1.052,0.9641,0.8904,0.8276,0.7265,0.6487,0.5871,0.5373,0.4962,0.4617,0.4323};
        physicsVector = CreatePhysicsVector(E,Pb_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double Pb_6[31]={0.6665,0.7355,0.8541,0.9515,1.032,1.098,1.154,1.201,1.242,1.386,1.475,1.534,1.573,1.614,1.626,1.621,1.607,1.588,1.566,1.543,1.427,1.323,1.232,1.153,1.021,0.9172,0.8336,0.765,0.7079,0.6597,0.6184};
        physicsVector = CreatePhysicsVector(E,Pb_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double Pb_7[31]={0.7558,0.8351,0.9739,1.091,1.189,1.272,1.343,1.403,1.456,1.645,1.765,1.848,1.906,1.974,2.005,2.012,2.006,1.992,1.973,1.952,1.831,1.715,1.61,1.515,1.354,1.224,1.117,1.029,0.954,0.8905,0.8358};
        physicsVector = CreatePhysicsVector(E,Pb_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double Pb_8[31]={0.8429,0.9317,1.089,1.223,1.339,1.439,1.524,1.599,1.664,1.9,2.053,2.161,2.24,2.34,2.391,2.414,2.418,2.411,2.396,2.378,2.259,2.135,2.018,1.91,1.722,1.565,1.435,1.325,1.232,1.153,1.083};
        physicsVector = CreatePhysicsVector(E,Pb_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double Pb_9[31]={0.9254,1.023,1.197,1.348,1.479,1.594,1.695,1.783,1.86,2.146,2.334,2.469,2.57,2.703,2.778,2.817,2.833,2.835,2.827,2.812,2.7,2.571,2.445,2.327,2.114,1.934,1.781,1.651,1.539,1.442,1.358};
        physicsVector = CreatePhysicsVector(E,Pb_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double Pb_10[31]={1.005,1.11,1.299,1.465,1.612,1.742,1.856,1.958,2.048,2.384,2.609,2.772,2.896,3.065,3.166,3.224,3.253,3.265,3.264,3.255,3.154,3.024,2.892,2.765,2.531,2.327,2.153,2.002,1.872,1.758,1.658};
        physicsVector = CreatePhysicsVector(E,Pb_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double Pb_11[31]={1.081,1.193,1.396,1.576,1.736,1.88,2.008,2.123,2.226,2.613,2.875,3.068,3.216,3.422,3.551,3.63,3.676,3.699,3.708,3.706,3.621,3.493,3.358,3.223,2.97,2.746,2.55,2.38,2.231,2.1,1.984};
        physicsVector = CreatePhysicsVector(E,Pb_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double Pb_12[31]={1.152,1.271,1.487,1.679,1.851,2.007,2.148,2.275,2.39,2.828,3.128,3.351,3.523,3.767,3.925,4.028,4.092,4.129,4.148,4.154,4.09,3.968,3.831,3.691,3.422,3.18,2.965,2.776,2.609,2.461,2.33};
        physicsVector = CreatePhysicsVector(E,Pb_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double Pb_13[31]={1.223,1.349,1.577,1.781,1.965,2.133,2.285,2.424,2.551,3.04,3.378,3.632,3.83,4.114,4.303,4.429,4.512,4.563,4.593,4.608,4.569,4.455,4.318,4.175,3.894,3.634,3.401,3.193,3.009,2.845,2.698};
        physicsVector = CreatePhysicsVector(E,Pb_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double Pb_14[31]={1.293,1.426,1.666,1.88,2.075,2.253,2.417,2.567,2.705,3.243,3.621,3.905,4.129,4.455,4.675,4.827,4.928,4.995,5.038,5.062,5.051,4.947,4.813,4.669,4.376,4.101,3.851,3.627,3.426,3.246,3.084};
        physicsVector = CreatePhysicsVector(E,Pb_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double Pb_15[31]={1.358,1.497,1.748,1.973,2.178,2.366,2.54,2.7,2.848,3.435,3.851,4.167,4.417,4.787,5.042,5.22,5.344,5.428,5.484,5.52,5.543,5.454,5.326,5.181,4.882,4.594,4.328,4.087,3.869,3.673,3.496};
        physicsVector = CreatePhysicsVector(E,Pb_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double Pb_16[31]={1.423,1.569,1.831,2.065,2.28,2.477,2.661,2.831,2.989,3.624,4.078,4.426,4.703,5.118,5.408,5.614,5.76,5.863,5.934,5.982,6.04,5.968,5.847,5.705,5.401,5.101,4.82,4.564,4.33,4.119,3.927};
        physicsVector = CreatePhysicsVector(E,Pb_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double Pb_17[31]={1.485,1.637,1.91,2.154,2.377,2.584,2.776,2.955,3.122,3.802,4.296,4.675,4.98,5.44,5.765,6,6.17,6.291,6.378,6.439,6.536,6.483,6.371,6.233,5.927,5.617,5.324,5.053,4.805,4.579,4.372};
        physicsVector = CreatePhysicsVector(E,Pb_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double Pb_18[31]={1.554,1.713,1.998,2.252,2.485,2.7,2.901,3.089,3.265,3.99,4.522,4.934,5.266,5.771,6.132,6.394,6.586,6.726,6.827,6.901,7.035,6.999,6.897,6.762,6.454,6.136,5.831,5.547,5.285,5.044,4.824};
        physicsVector = CreatePhysicsVector(E,Pb_18,factor);
     }
}

  else if( matName == namesMat[24] ) {
     if( ionZ == 3 ) {
        G4double U_3[31]={0.4158,0.4488,0.4987,0.5342,0.5603,0.5798,0.5944,0.6054,0.6139,0.6323,0.6313,0.623,0.6119,0.5877,0.564,0.5413,0.5196,0.4992,0.4801,0.4623,0.3915,0.3427,0.3074,0.2806,0.2419,0.2148,0.1943,0.1781,0.1649,0.1538,0.1443};
        physicsVector = CreatePhysicsVector(E,U_3,factor);
     }
     else if( ionZ == 4 ) {
        G4double U_4[31]={0.5309,0.5804,0.6584,0.7166,0.7614,0.7965,0.8243,0.8465,0.8645,0.9152,0.9314,0.9324,0.9264,0.9057,0.8816,0.8569,0.8318,0.8068,0.7824,0.7591,0.6601,0.5867,0.5313,0.4878,0.4235,0.3773,0.3421,0.314,0.291,0.2716,0.2551};
        physicsVector = CreatePhysicsVector(E,U_4,factor);
     }
     else if( ionZ == 5 ) {
        G4double U_5[31]={0.6283,0.6928,0.7987,0.8807,0.9455,0.9979,1.041,1.076,1.105,1.196,1.235,1.25,1.253,1.243,1.224,1.201,1.175,1.149,1.122,1.095,0.9736,0.8778,0.8024,0.7416,0.6491,0.581,0.5283,0.4859,0.4508,0.4212,0.3959};
        physicsVector = CreatePhysicsVector(E,U_5,factor);
     }
     else if( ionZ == 6 ) {
        G4double U_6[31]={0.7126,0.79,0.9217,1.027,1.112,1.183,1.241,1.291,1.332,1.468,1.535,1.569,1.585,1.59,1.58,1.563,1.54,1.514,1.486,1.458,1.322,1.207,1.113,1.036,0.9142,0.8227,0.7506,0.692,0.6431,0.6017,0.566};
        physicsVector = CreatePhysicsVector(E,U_6,factor);
     }
     else if( ionZ == 7 ) {
        G4double U_7[31]={0.786,0.8741,1.029,1.156,1.261,1.35,1.425,1.489,1.543,1.728,1.828,1.883,1.914,1.941,1.944,1.935,1.917,1.895,1.869,1.841,1.697,1.567,1.457,1.364,1.215,1.099,1.007,0.9307,0.8667,0.812,0.7647};
        physicsVector = CreatePhysicsVector(E,U_7,factor);
     }
     else if( ionZ == 8 ) {
        G4double U_8[31]={0.8561,0.9532,1.127,1.275,1.4,1.506,1.597,1.676,1.744,1.982,2.116,2.196,2.245,2.296,2.314,2.316,2.306,2.289,2.267,2.241,2.094,1.953,1.829,1.722,1.546,1.407,1.294,1.2,1.12,1.051,0.9908};
        physicsVector = CreatePhysicsVector(E,U_8,factor);
     }
     else if( ionZ == 9 ) {
        G4double U_9[31]={0.9213,1.026,1.217,1.383,1.526,1.649,1.756,1.849,1.932,2.223,2.394,2.501,2.57,2.648,2.684,2.698,2.698,2.688,2.67,2.648,2.504,2.355,2.22,2.101,1.902,1.74,1.607,1.495,1.398,1.315,1.242};
        physicsVector = CreatePhysicsVector(E,U_9,factor);
     }
     else if( ionZ == 10 ) {
        G4double U_10[31]={0.9839,1.096,1.3,1.483,1.642,1.782,1.904,2.012,2.107,2.453,2.664,2.8,2.891,2.999,3.055,3.083,3.094,3.092,3.08,3.063,2.927,2.773,2.629,2.5,2.279,2.097,1.944,1.814,1.701,1.603,1.516};
        physicsVector = CreatePhysicsVector(E,U_10,factor);
     }
     else if( ionZ == 11 ) {
        G4double U_11[31]={1.043,1.161,1.378,1.574,1.749,1.903,2.04,2.162,2.271,2.672,2.924,3.09,3.204,3.344,3.422,3.468,3.491,3.499,3.496,3.484,3.361,3.206,3.056,2.918,2.677,2.476,2.304,2.157,2.028,1.915,1.814};
        physicsVector = CreatePhysicsVector(E,U_11,factor);
     }
     else if( ionZ == 12 ) {
        G4double U_12[31]={1.098,1.222,1.45,1.657,1.845,2.013,2.164,2.298,2.42,2.875,3.168,3.365,3.503,3.677,3.78,3.844,3.883,3.902,3.908,3.903,3.798,3.645,3.49,3.345,3.088,2.87,2.681,2.517,2.372,2.244,2.131};
        physicsVector = CreatePhysicsVector(E,U_12,factor);
     }
     else if( ionZ == 13 ) {
        G4double U_13[31]={1.155,1.284,1.522,1.74,1.939,2.12,2.283,2.431,2.564,3.074,3.408,3.637,3.801,4.011,4.14,4.224,4.277,4.309,4.324,4.328,4.244,4.096,3.939,3.788,3.517,3.282,3.077,2.896,2.737,2.595,2.467};
        physicsVector = CreatePhysicsVector(E,U_13,factor);
     }
     else if( ionZ == 14 ) {
        G4double U_14[31]={1.209,1.345,1.592,1.819,2.029,2.221,2.396,2.555,2.7,3.262,3.638,3.9,4.09,4.34,4.495,4.599,4.669,4.713,4.739,4.751,4.692,4.551,4.394,4.24,3.956,3.707,3.486,3.291,3.117,2.961,2.82};
        physicsVector = CreatePhysicsVector(E,U_14,factor);
     }
     else if( ionZ == 15 ) {
        G4double U_15[31]={1.261,1.402,1.658,1.893,2.111,2.313,2.499,2.669,2.825,3.437,3.854,4.15,4.367,4.658,4.844,4.971,5.058,5.118,5.156,5.178,5.149,5.02,4.865,4.709,4.416,4.153,3.919,3.709,3.521,3.351,3.197};
        physicsVector = CreatePhysicsVector(E,U_15,factor);
     }
     else if( ionZ == 16 ) {
        G4double U_16[31]={1.313,1.459,1.725,1.967,2.193,2.404,2.599,2.78,2.946,3.607,4.065,4.396,4.641,4.975,5.192,5.342,5.449,5.524,5.575,5.608,5.611,5.495,5.345,5.189,4.888,4.614,4.367,4.143,3.941,3.757,3.59};
        physicsVector = CreatePhysicsVector(E,U_16,factor);
     }
     else if( ionZ == 17 ) {
        G4double U_17[31]={1.363,1.514,1.789,2.038,2.271,2.49,2.694,2.884,3.06,3.767,4.266,4.63,4.904,5.283,5.532,5.707,5.833,5.924,5.989,6.033,6.073,5.972,5.828,5.673,5.368,5.084,4.825,4.589,4.373,4.177,3.998};
        physicsVector = CreatePhysicsVector(E,U_17,factor);
     }
     else if( ionZ == 18 ) {
        G4double U_18[31]={1.419,1.577,1.861,2.118,2.358,2.585,2.797,2.996,3.181,3.935,4.474,4.872,5.175,5.599,5.879,6.078,6.223,6.33,6.408,6.463,6.537,6.451,6.313,6.159,5.849,5.556,5.286,5.038,4.811,4.603,4.412};
        physicsVector = CreatePhysicsVector(E,U_18,factor);
     }
  }

  if(physicsVector != 0) {
     dedxKeys.push_back( key );
     dedx.push_back( physicsVector );
  }

  return physicsVector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4PhysicsVector* G4SimpleMaterialStoppingICRU73::GetPhysicsVector(G4int ionZ, 
						 G4int matZ)
{
  G4PhysicsVector* physicsVector = 0;

  size_t nmbMat =  atomicNumbersMat.size();

  for( size_t i = 0; i < nmbMat; i++ ) {

     if( atomicNumbersMat[i] == matZ ) { 
        physicsVector = GetPhysicsVector(ionZ, namesMat[i]);
        break; 
     }
  }

  return physicsVector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
