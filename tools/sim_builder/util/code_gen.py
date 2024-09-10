
import os
import subprocess

from .modules import MODULE_TYPES
from .loading import Loader

CURR_DIR = os.path.dirname(os.path.realpath(__file__))
BASE_VIPRA_DIR = os.path.realpath(CURR_DIR  + '/../../../')
MODULES_DIR = BASE_VIPRA_DIR + '/modules'
BASE_MODULES_DIR = BASE_VIPRA_DIR + '/include/vipra/base_modules'


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


def _find_module(moduleName, moduleType):
  module = _find_module_in(moduleName, moduleType, MODULES_DIR)
  if module:
    return {'path': module, 'base': False}

  path = _find_module_in(moduleName, moduleType, BASE_MODULES_DIR)
  if (path == None):
    raise Exception(f'Unable to find module {moduleName}')

  return {'path': path, 'base': True}
  

def _get_namespace_name(moduleName, moduleType, base):
  moduleName = f'{moduleType.capitalize()}::{moduleName}'
  if base:
    moduleName = f'VIPRA::{moduleName}'
  
  return moduleName

def _get_input_modules(modules):
  if not modules['pedestrians']:
    raise Exception('Pedestrians Input missing')
  if not modules['map']:
    raise Exception('Map Input missing')

  peds = _find_module(modules['pedestrians'], 'input')
  obs = _find_module(modules['map'], 'input')

  inputModules = {
    'pedestrians': {
      'name': _get_namespace_name(modules['pedestrians'], 'Input', peds['base']),
      'path': peds['path']
    },
    'map': {
      'name': _get_namespace_name(modules['map'], 'Input', obs['base']),
      'path': obs['path']
    }
  }

  return inputModules

def _get_modules(modules):
  moduleList = {}
  for moduleType in MODULE_TYPES:
    if (moduleType == 'input'):
      moduleList[moduleType] = _get_input_modules(modules[moduleType])
      continue

    moduleName = modules[moduleType]
    module = _find_module(moduleName, moduleType)
    if not module['path']:
      raise Exception(f'Unable to find module named: {moduleName}')

    
    moduleName = f'{moduleType.capitalize()}::{moduleName}'
    if module['base']:
        moduleName = f'VIPRA::{moduleName}'

    moduleList[moduleType] = {
      'name': moduleName,
      'path': module['path']
    }
  
  return moduleList

def _make_includes(moduleList):
  includes = []

  for key in moduleList:
    if key == 'input':
      if (not moduleList[key]['map']['path'] or not moduleList[key]['pedestrians']['path']):
        raise Exception("Missing Input Module Paths")
      includes.append(moduleList[key]['pedestrians']['path'])
      includes.append(moduleList[key]['map']['path'])
      continue
    
    if (not moduleList[key]['path']):
      raise Exception("Missing Module Path")

    includes.append(moduleList[key]['path'])

  return includes

def _generate_sim(modules):
  moduleList = _get_modules(modules)
  if not moduleList:
    raise Exception('Unable to configure simulation with provided modules')

  includes = _make_includes(moduleList)

  with open(f'{CURR_DIR}/base_sim.txt') as file:
    code = file.read()
    code = code.replace('$module_includes', '#include \"' + '\"\n#include \"'.join(includes) + '\"')
    
    for moduleType in MODULE_TYPES:
      if moduleType == 'input':
        code = code.replace('$input_pedestrians', moduleList['input']['pedestrians']['name'])
        code = code.replace('$input_map', moduleList['input']['map']['name'])
        continue

      code = code.replace(f'${moduleType}', moduleList[moduleType]['name'])
    return code

  return None

def _compile_sim(mainpath, installpath, config):
    subprocess.run([ 'cmake', '-B', f'{BASE_VIPRA_DIR}/build', BASE_VIPRA_DIR, '-DVIPRA_DEBUG_MODE=OFF', '-DVIPRA_PERF_TESTING=OFF', '-DVIPRA_BUILD_LIBRARY=OFF', '-DVIPRA_BUILD_EXECUTABLE=ON', f'-DVIPRA_USE_MPI={config["use_mpi"]}', f'-DVIPRA_MAIN_FILE={mainpath}', f'-DVIPRA_EXECUTABLE_PATH={installpath}' ], check=True)
    subprocess.run([ 'cmake', '--build', f'{BASE_VIPRA_DIR}/build' ], check=True)

def build_sim(config, installpath):
  code = _generate_sim(config)

  with open(f'{installpath}/main.cpp', 'w+') as main:
    main.write(code)
  
  _compile_sim(f'{installpath}/main.cpp', installpath, config)
