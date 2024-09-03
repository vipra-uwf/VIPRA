
import os
import shutil

class SimCache:
  def __init__(self):
    self.simMap = {}
    self.cachedir = None

  def init(self, cachepath):
    if not os.path.exists(cachepath):
      os.makedirs(cachepath)

    if not os.path.isdir(cachepath):
      raise Exception(f'Cache Path: {cachepath} is not a directory')

    self.cachedir = os.path.realpath(cachepath)

    for subdir, dirs, files in os.walk(cachepath):
      for dir in dirs:
        self.simMap[dir] = {
          'path': os.path.realpath(f'{cachepath}/{dir}')
        }

  def add_sim(self, name, configpath):
    fullpath = f'{self.cachedir}/{name}'
    if not os.path.exists(fullpath):
      os.makedirs(fullpath)

    if configpath:
      shutil.copy(configpath, f'{fullpath}/config.json')
    else:
      if not os.path.exists(f'{fullpath}/config.json'):
        raise Exception(f'Simulation \"{name}\" was never provided a configuration file. Provide the path with the -f/--file argument')
    
    self.simMap[name] = {
      'path': fullpath
    }

  def has_sim(self, name):
    return (name in self.simMap)

  def path(self, name):
    if not self.has_sim(name):
      raise Exception(f'No Simulation cached with name: \"{name}\"')
    return self.simMap[name]['path']

  def config_path(self, name):
    return f'{self.path(name)}/config.json'
