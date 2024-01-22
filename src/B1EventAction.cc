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
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "B1RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
  fTime = {};
  fPositionx = {};
  fPositiony = {};
  fPositionz = {};
//  G4cout << "-----------------" << G4endl;
//  G4cout << "event begin" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event*)
{   
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
  G4cout << "event end" << G4endl;
  int trackNum = fTime.size();
  if(trackNum != 0) {
      auto analysisManager =G4AnalysisManager::Instance();
      G4cout << "Time is:" << fTime[0] << G4endl;
      G4cout << "delta Time is:" << fTime[trackNum-1]- fTime[0] << G4endl;
      double deltaTime = fTime[trackNum-1]- fTime[0];
      //std::cout << "positionx: " << fPositionx[0] << std::endl;
      //std::cout << "positionx: " << fPositionx[fPositionx.size()-1] << std::endl;
      double deltaX = fPositionx[trackNum-1] - fPositionx[0];
      double deltaY = fPositiony[trackNum-1] - fPositiony[0];
      double deltaZ = fPositionz[trackNum-1] - fPositionz[0];

      double ProtonMomentum = calculateMomentum(deltaTime,deltaX,deltaY,deltaZ);
      analysisManager->FillNtupleDColumn(0,ProtonMomentum);
      analysisManager->AddNtupleRow();
      fRunAction->fMomentum.push_back(ProtonMomentum);

  }
  G4cout << "-------------------" << G4endl;

}

double B1EventAction::calculateMomentum(double time, double dx, double dy, double dz) {
    double dis = sqrt(dx*dx + dy*dy + dz*dz)*1e-3;
    double speed = dis/(time*1e-9);
    std::cout << "speed is:" << speed << std::endl;
    double gamma = speed/299792458;
    std::cout << "momentum is :" << 938.27208812*gamma*sqrt(1-gamma*gamma) << std::endl;
    return 938.27208812*gamma*sqrt(1-gamma*gamma);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
