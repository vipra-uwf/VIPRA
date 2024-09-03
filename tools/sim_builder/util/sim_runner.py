
import subprocess

import util.input_handling as input

def run(installpath):
  mpirun = input.get_yes_no('MPI Run?')
  cores = 1
  if mpirun:
    cores = input.get_int('Number of Nodes')
  count = input.get_int('Simulation Count')
  map = input.get_str('Path to Map')
  peds = input.get_str('Path to Pedestrians')
  params = input.get_str('Path to Parameters')

  if mpirun:
    subprocess.run([ 'mpiexec', '-n', f'{cores}', f'{installpath}/VIPRA', f'-count={count}', f'-map={map}', f'-peds={peds}', f'-params={params}' ], check=True)
  else:
    subprocess.run([ f'{installpath}/VIPRA', f'-count={count}', f'-map={map}', f'-peds={peds}', f'-params={params}' ], check=True)
