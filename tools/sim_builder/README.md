# VIPRA Simulation Builder

A wrapper program for choosing which modules to use in a simulation without having to manually modify code

# Running

```bash
# Creating a new configuration
python3 simbuilder.py -n <Name> -f <Config filepath> -c

# Running a configuration
python3 simbuilder.py -n <Name> -r

# Creating and Running in one
python3 simbuilder.py -n <Name> -f <Config filepath> -c -r
```

# Flags

Required:
- `-n <name>` or `--name <name>`: Chooses the name of the simulation configuration

Optional:
- `-f <config filepath>` or `--file <config filepath>`: Sets the path to the configuration file for the simulation
- `-c`: if added, the simulation will be compiled
- `-r`: if added, the simulation will be run
