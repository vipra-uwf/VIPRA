
import json

MODULE_TYPES = [
  "model",
  "pedestrians",
  "goals",
  "map",
  "output",
  "input"
]

def load_config(filepath):
  with open(filepath) as configFile:
    config = json.load(configFile)
    return config
  
  raise f'Unable to load config file at: {filepath}'
