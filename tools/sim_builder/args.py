
from argparse import ArgumentParser

def _add_args(parser):
  parser.add_argument('-n', '--name', required=True, dest='name', help='Name the simulation configuration for future use', metavar='str')
  parser.add_argument('-c', dest='compile', help='Should the simulation compiled', action='store_true')
  parser.add_argument('-r', '--run', dest='run', action='store_true', help='Run the simulation with the provided arguments')
  

def parse():
  parser = ArgumentParser()

  _add_args(parser)

  return parser.parse_args()
