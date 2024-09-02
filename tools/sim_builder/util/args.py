
from argparse import ArgumentParser

def _add_args(parser):
  parser.add_argument('-n', '--name', required=True, dest='name', help='Name the simulation configuration for future use', metavar='str')
  parser.add_argument('-f', '--file', dest='file', help='Path to the configuration file', metavar='str')
  parser.add_argument('-c', '--cache', dest='cachedir', help='Where simulations should be saved for later use', metavar='str', default='./simcache')
  parser.add_argument('-r', '--run', dest='run', action='store_true', help='Should The simulation be run after being built')
  

def parse():
  parser = ArgumentParser()

  _add_args(parser)

  return parser.parse_args()
