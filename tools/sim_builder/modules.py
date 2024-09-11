
import json
import os

CURR_DIR = os.path.dirname(os.path.realpath(__file__))
BASE_VIPRA_DIR = os.path.realpath(CURR_DIR  + '/../../')
MODULES_DIR = BASE_VIPRA_DIR + '/modules'
BASE_MODULES_DIR = BASE_VIPRA_DIR + '/include/vipra/base_modules'

MODULE_TYPES = [
  "model",
  "pedestrians",
  "goals",
  "map",
  "input",
  "output"
]

def load_config(filepath):
  with open(filepath) as configFile:
    config = json.load(configFile)
    return config
  
  raise f'Unable to load config file at: {filepath}'


def _find_module_in(moduleName, moduleType, baseDir):
  moduleDir = baseDir + f'/{moduleType}'

  if os.path.isdir(f'{moduleDir}/{moduleName}'):
    moduleDir += f'/{moduleName}'

  if not os.path.isfile(f'{moduleDir}/CMakeLists.txt'):
    return None

  if os.path.isfile(f'{moduleDir}/{moduleName}.hpp'):
    return f'{moduleDir}/{moduleName}.hpp'

  if os.path.isfile(f'{moduleDir}/{moduleName}.h'):
    return f'{moduleDir}/{moduleName}.h'

  return None


def find_module(moduleName, moduleType):
  module = _find_module_in(moduleName, moduleType, MODULES_DIR)
  if module:
    return {'path': module, 'base': False}

  path = _find_module_in(moduleName, moduleType, BASE_MODULES_DIR)
  if (path == None):
    raise Exception(f'Unable to find module {moduleName}')

  return {'path': path, 'base': True}

def get_module_options(moduleType):
  modules = []
  modules = modules + [ os.path.basename(f) for f in os.scandir(f'{MODULES_DIR}/{moduleType}') if f.is_dir() ]
  if (moduleType != 'model'):
    modules = modules + [ os.path.basename(os.path.splitext(f.path)[0]) for f in os.scandir(f'{BASE_MODULES_DIR}/{moduleType}') if f.is_file() and os.path.splitext(f.path)[1] == '.hpp' ]

  return modules

def _config_input(config):
  config['input'] = {}

  print(f'Options: {", ".join(get_module_options('input'))}')
  config['input']['pedestrians'] = input('pedestrian input module: ')
  if find_module(config['input']['pedestrians'], 'input') == None:
      raise Exception(f'Unable to find input module: {config[moduleType]}')

  print(f'Options: {", ".join(get_module_options('input'))}')
  config['input']['map'] = input('map input module: ')
  if find_module(config['input']['map'], 'input') == None:
      raise Exception(f'Unable to find input module: {config[moduleType]}')

def create_config():
  config = {}

  for moduleType in MODULE_TYPES:
    if moduleType == 'input':
      _config_input(config)
    else:
      print(f'Options: {", ".join(get_module_options(moduleType))}')
      config[moduleType] = input(f'{moduleType} module: ')
      if find_module(config[moduleType], moduleType) == None:
        raise Exception(f'Unable to find {moduleType} module: {config[moduleType]}')
    print()

  useMPI = input(f'should MPI be used? (y/n) ')
  while useMPI != 'y' and useMPI != 'n':
    useMPI = input(f'should MPI be used? (y/n) ')

  config['use_mpi'] = (useMPI == 'y')

  return config


