import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter, FFMpegFileWriter
import numpy as np
import importlib

helpers = importlib.import_module('visUtil')

args = helpers.getArgs()

if (args['overrides']):
  overrides = importlib.import_module(args['overrides'])
  for name, entity in helpers.__dict__.items():
    if not name.startswith('__'):
        if hasattr(overrides, name):
          setattr(helpers, name, getattr(overrides, name))


peds = helpers.getPeds(args['peds'])
difCoords = helpers.getPeds(args['dif'])
map = helpers.getObs(args['obs'], args['dxf']);
pedColors = helpers.makeColors(peds, args)
timestepCnt = len(peds["timesteps"])

fig,ax = plt.subplots()


def animate(i):
  global pedColors
  helpers.printProgressBar(i, timestepCnt, 'Animating')
  [pointsX, pointsY] = helpers.getPoints(peds["timesteps"][i])
  [compX, compY] = helpers.getPoints(difCoords[i]) if args['dif'] else [[], []]

  helpers.prepPlot(ax, i, args)
  if args['dxf']:
    helpers.draw_dxf(map, ax, args)
  else:
    helpers.plotObs(map, ax, args)

  helpers.plotShoulders(pointsX, pointsY, pedColors, ax, args)
  points = helpers.plotPeds(pointsX, pointsY, pedColors, ax, args) if not args['dif'] else helpers.plotDif(pointsX, pointsY, compX, compY, pedColors, ax, args)
  helpers.plotIndexes(pointsX, pointsY, pedColors, ax, args)
  helpers.plotExtras(ax, i, peds, pedColors)
  
  temp = helpers.updateColors(peds)
  if temp:
    print("updating colors")
    pedColors = temp

  return ax

ani = FuncAnimation(fig, animate, frames=timestepCnt, blit=False)
ani.save(args['outpath'], writer=FFMpegFileWriter(fps=args['fps']), progress_callback=helpers.printProgressBar)
helpers.printProgressBar(timestepCnt, timestepCnt, 'Done')