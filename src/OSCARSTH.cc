////////////////////////////////////////////////////////////////////
//
// Dean Andrew Hidas <dhidas@bnl.gov>
//
// Created on: Thu Feb  2 11:55:24 EST 2017
//
////////////////////////////////////////////////////////////////////

#include "OSCARSTH.h"

#include <iostream>

#include "TOSCARSSR.h"

OSCARSTH::OSCARSTH ()
{
  // Default constructor
}



OSCARSTH::~OSCARSTH ()
{
  // Destruction
}





double OSCARSTH::UndulatorK (double const BFieldMax, double const Period) const
{
  // Return the 'K' value for an undulator with max bfield [T], Period [m]

  return BFieldMax * Period * TOSCARSSR::Qe() / (TOSCARSSR::TwoPi() * TOSCARSSR::Me() * TOSCARSSR::C());
}


double OSCARSTH::DipoleSpectrum (double const BField, double const BeamEnergy_GeV, double const Angle, TVector2D const EnergyRange_eV) const
{
  std::cout << "BField:         " << BField << std::endl;
  std::cout << "BeamEnergy_GeV: " << BeamEnergy_GeV << std::endl;
  std::cout << "Angle:          " << Angle << std::endl;
  std::cout << "EnergyRange_eV: " << EnergyRange_eV << std::endl;

  return 0;
}
