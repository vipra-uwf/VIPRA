
import os
import shutil
import json

from .modules import create_config

class SimCache:
  def __init__(self):
    self.simMap = {}
    self.cachedir = None

  def init(self, cachepath):
    if not os.path.exists(cachepath):
      os.makedirs(cachepath)

    if not os.path.isdir(cachepath):
      raise Exception(f'Cache Path: {cachepath} is not a directory')

    self.cachedir = os.path.realpath('./simcache')

    for subdir, dirs, files in os.walk(cachepath):
      for dir in dirs:
        self.simMap[dir] = {
          'path': os.path.realpath(f'{cachepath}/{dir}'),
        }
        if os.path.isfile(f'{cachepath}/{dir}/config.json'):
          with open(f'{cachepath}/{dir}/config.json') as f:
            self.simMap[dir]['config'] = json.load(f)
        else:
          self.simMap[dir]['config'] = None

  def configure_sim(self, name):
    fullpath = f'{self.cachedir}/{name}'

    if self.has_sim(name) and os.path.isfile(f'{fullpath}/config.json'):
      print(f'Simulation {name} already exists\n use -c to recompile or -r to run')
      exit()

    if not os.path.exists(fullpath):
      os.makedirs(fullpath)

    config = create_config()

    with open(f'{fullpath}/config.json', 'w') as configFile:
     configFile.write(json.dumps(config))

    self.simMap[name] = {
      'path': fullpath,
      'config': config
    }

  def has_sim(self, name):
    return (name in self.simMap)

  def path(self, name):
    if not self.has_sim(name):
      raise Exception(f'No Simulation cached with name: \"{name}\"')
    return self.simMap[name]['path']

  def config_path(self, name):
    return f'{self.path(name)}/config.json'

  def use_mpi(self, name):
    return self.simMap[name]['config']['use_mpi']

  def configured(self, name):
    return os.path.isfile(f'{self.simMap[name]['path']}/config.json')
