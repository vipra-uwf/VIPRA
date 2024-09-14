
# VIPRA Trajectories Visualization

# visualize.py

`visualize.py` is used to view the output from a simulation run.

## Setup

### Requirements

- Python 3.7^

- FFmpeg

- `pip` Modules:
  - `json`
  - `sys`
  - `os`
  - `numpy`
  - `matplotlib`


## Running

`python3 visualize.py *flags*`

### Flags

1. **REQUIRED** `-peds *sim output path*` : sets the path to the simulation output *(no default)*
2. **REQUIRED** `-obs *obstacle map path*` : sets the path to the obstacle map *(no default)*
3. `-outpath *output gif path*` : sets the path for the output gif *(default ./output.gif)*
4. `-idx` : prints the pedestrians index next to their position *(default OFF)*
5. `-idxClr` : colors pedestrian indexes to their color *(default OFF)*
6. `-shldr *len*` : draws shoulders (axis aligned) on pedestrians, len being half the total shoulder width *(default OFF)*
7. `-xDim *minX, maxX*` : sets the min and max X for the plot *(default: -1, 25)*
8. `-yDim *minY, maxY*` : sets the min and max Y for the plot *(default: -1, 4)*
9. `-fps *fps*` : sets the fps of the output Gif *(default 10)*
10. `-pedClr` : sets each pedestrian a different color *(default OFF)*
11. `-obsClr *color*` : sets the color of obstacles *(default k)*
12. `-bckClr *color*` : sets the color of the background *(default w)*
13. `-dif *sim output filepath*` : shows differences between sim runs
14. `-difalpha *0.0-1.0*` : sets the alpha for dif *(default 0.25)*
15. `-diflines` : draws lines between pedestrians that have a dif *(default OFF)*
16. `-dlmax *float*` : scales dif line alpha by max distance *(default OFF)*
17. `-overrides *filename*` : overrides helper functions with the module provided

**color options: https://matplotlib.org/stable/tutorials/colors/colors.html**

## Plotting Custom Information

For plotting extra information added to the output use the `overrides` flag.

Create a python file that includes the method:
```python
plotExtras(ax, timestep, output)
``` 
- `ax` is the matplotlib plot to draw to
- `timestep` is the current timestep
- `output` is the entire JSON output from the simulation

Returns:
- Nothing

### Pedestrian Colors 

Pedestrian colors can be updated throughout the GIF by overriding the method:
```python
updateColors(output)
```
- `output` is the entire JSON output from the simulation

Returns:
- Numpy array containing each pedestrian's color