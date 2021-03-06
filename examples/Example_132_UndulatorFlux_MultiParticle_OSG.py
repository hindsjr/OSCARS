# This is meant to be run on OSG or similar condor-type systems


# In this example OSG is used to calculate a multi-particle
# simulation.  The rank 0 process calculates the ideal single-particle
# data and then waits for the other processes to return their data.
# When a process returns the results are added to the others.  The results
# are then outputted as data files, which can be plotted using Jupyter
# Notebook or something similar.

# Command line arguments are given by condor at runtime
import sys
sys.path.append('.')

Process = sys.argv[1]
NAME = sys.argv[2]

out_file_name = NAME + '_' + Process + '.dat'

# Import the OSCARS SR and helper modules
import oscars.sr

# Get an OSCARS SR object
osr = oscars.sr.sr()

# Set a particle beam with non-zero emmitance
osr.set_particle_beam(type='electron',
                      name='beam_0',
                      energy_GeV=3,
                      x0=[0,0,-1],
                      d0=[0,0,1],
                      current=0.500,
                      sigma_energy_GeV=0.001*3,
                      beta=[1.5,0.8],
                      emittance=[0.9e-9,0.008e-9],
                      horizontal_direction=[1,0,0],
                      lattice_reference=[0,0,0])

# Must set the start and stop time for calculations
osr.set_ctstartstop(0,2)

# Clear any exsisting fields (just good habit in notebook style) and add
# an undulator field
osr.clear_bfields()
osr.add_bfield_undulator(bfield=[0,1,0],period=[0,0,0.049],nperiods=31)

# Number of particles per node of rank > 1
particles_per_node = 1000

# Translation for center of rectangle
rcenter = [0, 0, 30]

# Width of rectangle
width = [0.01,0.01]

# Number of points in flux
npoints = [101, 101]

# Energy we are interested in
energy_eV = 152

# Ideal single-particle data
if int(Process) == 0:
  osr.set_new_particle(particle='ideal')
  flux = osr.calculate_flux_rectangle(plane='XY',
                                      energy_eV=energy_eV,
                                      width=width,
                                      npoints=npoints,
                                      translation=rcenter,
                                      ofile=out_file_name)

# Multi-paricle simulation
else:
  data = osr.calculate_flux_rectangle(plane='XY',
                                       energy_eV=energy_eV,
                                       width=width,
                                       npoints=npoints,
                                       translation=rcenter,
                                       nparticles=particles_per_node,
                                       ofile=out_file_name)
  
