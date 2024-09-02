
from argparse import ArgumentParser

def _add_args(parser):
  parser.add_argument('-n', '--name', required=True, dest='name', help='Name the simulation configuration for future use', metavar='str')
  parser.add_argument('-f', '--file', dest='file', help='Path to the configuration file', metavar='str')
  parser.add_argument('--cache', dest='cachedir', help='Where simulations should be saved for later use', metavar='str', default='./simcache')

  parser.add_argument('-c', dest='compile', help='Should the simulation compiled', action='store_true')
  parser.add_argument('-r', '--run', dest='run', action='store_true', help='Run the simulation with the provided arguments')
  parser.add_argument('--args', dest='args', nargs='+')
  

def parse():
  parser = ArgumentParser()

  _add_args(parser)

  return parser.parse_args()
