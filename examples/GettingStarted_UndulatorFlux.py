# Import the OSCARS SR module
import oscars.sr

# Optionally import the plotting tools (matplotlib)
from oscars.plots_mpl import *

# Create an OSCARS SR object
osr = oscars.sr.sr()

# Clear all existing fields and create an undulator field
osr.clear_bfields()
osr.add_bfield_undulator(bfield=[0, 1, 0], period=[0, 0, 0.050], nperiods=31)

# Define simple electron beam
osr.set_particle_beam(type='electron', name='beam_0', energy_GeV=3, x0=[0, 0, -1], d0=[0, 0, 1], current=0.5)

# Define the start and stop times for the calculation
osr.set_ctstartstop(0, 2)

# Calculate spectrum at 30 [m].  Note use of the nthreads argument.
flux = osr.calculate_flux_rectangle(plane='XY',
                                    energy_eV=143.8,
                                    width=[0.01, 0.01],
                                    npoints=[101, 101],
                                    translation=[0, 0, 30],
                                    nthreads=8)

# Plot flux
plot_flux(flux)
