import sys
import json
import math
import numpy as np
import datetime

def getArgs():
  indexes = False
  shoulders = False
  shldrLen = 0
  flagCnt = 1
  outpath = "output.gif"
  peds = ""
  obs = ""
  xMin = -1
  yMin = -1
  xMax = 25
  yMax = 4
  fps = 10
  idxColor = False
  pedColor = False
  obsColor = 'k'
  bckColor = 'w'
  dif = None
  difalpha = 0.25
  diflines = False
  diflineMax = None
  funcOverrides = None

  i = 1
  while i < len(sys.argv):
    if sys.argv[i] == '-idx':
      indexes = True
    elif sys.argv[i] == '-shldr':
      i += 1
      shldrLen = float(sys.argv[i])
      shoulders = True
    elif sys.argv[i] == '-outpath':
      outpath = sys.argv[i + 1]
      i += 1
    elif sys.argv[i] == '-peds':
      i += 1
      peds = sys.argv[i]
    elif sys.argv[i] == '-obs':
      i += 1
      obs = sys.argv[i]
    elif sys.argv[i] == '-xDim':
      i += 1
      dims = sys.argv[i].split(',')
      xMin = float(dims[0])
      xMax = float(dims[1])
    elif sys.argv[i] == '-yDim':
      i += 1
      dims = sys.argv[i].split(',')
      yMin = float(dims[0])
      yMax = float(dims[1])
    elif sys.argv[i] == '-fps':
      i += 1
      fps = float(sys.argv[i])
    elif sys.argv[i] == '-idxClr':
      idxColor = True
    elif sys.argv[i] == '-pedClr':
      pedColor = True
    elif sys.argv[i] == '-obsClr':
      i += 1
      obsColor = sys.argv[i]
    elif sys.argv[i] == '-bckClr':
      i += 1
      bckColor = sys.argv[i]
    elif sys.argv[i] == '-dif':
      i += 1
      dif = sys.argv[i]
    elif sys.argv[i] == '-difalpha':
      i += 1
      difalpha = float(sys.argv[i])
    elif sys.argv[i] == '-diflines':
      diflines = True
    elif sys.argv[i] == '-dlmax':
      i += 1
      diflineMax = float(sys.argv[i])
    elif sys.argv[i] == '-overrides':
      i += 1
      funcOverrides = sys.argv[i]
    else:
      if (sys.argv[i][0] == '-'):
        print(f'Unknown Flag: {sys.argv[i]}')
        exit(-1)
    
    i += 1
    

  if peds == "" or obs == "":
    print("The Flags -peds and -obs Are Required")
    exit(-1)

  return dict(
    indexes  = indexes,
    shoulders= shoulders,
    shldrLen = shldrLen,
    outpath  = outpath,
    peds     = peds,
    obs      = obs,
    xDim     = [xMin, xMax],
    yDim     = [yMin, yMax],
    fps      = fps,
    idxColor = idxColor,
    pedColor = pedColor,
    obsColor = obsColor,
    bckColor = bckColor,
    dif = dif,
    difalpha = difalpha,
    diflines = diflines,
    diflineMax = diflineMax,
    overrides = funcOverrides
  )


def getObs(obs):
  obsCoords = json.load(open(obs))
  obsX = []
  obsY = []
  for obj in obsCoords["obstacle"]:
    obsX.append(float(obj["x"]))
    obsY.append(float(obj["y"]))

  return (obsX, obsY)

def getPeds(peds):
  if peds:
    return json.load(open(peds))
  else:
    return {}


def getPoints(timestep):
  pointsX = []
  pointsY = []
  for ped in timestep["pedestrians"]:
    pointsX.append(ped["position"][0])
    pointsY.append(ped["position"][1])

  return (pointsX, pointsY)

def makeColors(peds, args):
  count = len(peds["timesteps"][0]["pedestrians"])
  if args['pedColor']:
    return np.random.rand(count, 3)
  else:
    return np.full((count,3),[1,0,0])

def plotShoulders(pointsX, pointsY, colors, ax, args):
  shldrLen = args['shldrLen']
  if args['shoulders']:
    for index in range(0, len(pointsX)):
      x = pointsX[index]
      y = pointsY[index]
      ax.plot([x-shldrLen, x+shldrLen], [y, y], color=colors[index], linestyle='-', linewidth=0.5, alpha=args['difalpha'] if args['dif'] else 1)
      ax.plot([x, x], [y-shldrLen, y+shldrLen], color=colors[index], linestyle='-', linewidth=0.5, alpha=args['difalpha'] if args['dif'] else 1)

def plotIndexes(pointsX, pointsY, pedColors, ax, args):
  if args['indexes']:
    for index in range(0, len(pointsX)):
      ax.text(pointsX[index], pointsY[index], index, fontsize=5, c=pedColors[index] if args['idxColor'] else 'k', alpha=args['difalpha'] if args['dif'] else 1)

def plotPeds(pedsX, pedsY, pedColors, ax, args):
  return ax.scatter(pedsX, pedsY, 2, color=pedColors)

def plotObs(obsX, obsY, ax, args):
  return ax.scatter(obsX, obsY, 1, c=args['obsColor'])

def prepPlot(ax, i, args):
  xDim = args['xDim']
  yDim = args['yDim']

  ax.clear()
  ax.set_xlim(xDim[0], xDim[1])
  ax.set_ylim(yDim[0], yDim[1])
  ax.autoscale(False)
  ax.set_facecolor(args['bckColor'])
  ax.text(xDim[1] / 2 , yDim[1], f'{datetime.timedelta(seconds=(i * 100 * 0.005))}')

percent = 0
def printProgressBar (iteration, total, prefix="Outputing", animating=False):
    if not animating:
      percent = ("{0:." + str(1) + "f}").format(100 * (iteration / float(total)))

    filledLength = int(100 * iteration // total)
    bar = '█' * filledLength + '-' * (100 - filledLength)
    print(f'\r{prefix.ljust(9, " ")} :  |{bar}| {percent}%', end = "\r")
    if iteration == total:
        print()

def plotDif(pointsX, pointsY, compX, compY, pedColors, ax, args):
  difalpha = args['difalpha']
  diflines = args['diflines']

  difXidx = np.where(~np.equal(pointsX, compX))[0]
  difYidx = np.where(~np.equal(pointsY, compY))[0]

  difX = np.array(pointsX)[difXidx]
  difY = np.array(pointsY)[difXidx]
  colors = np.array(pedColors)[difXidx] if args['pedColor'] else pedColors

  np.append(difX, np.array(pointsX)[difYidx])
  np.append(difY, np.array(pointsY)[difYidx])
  if args['pedColor']:
    np.append(colors, np.array(pedColors)[difYidx])

  ax.scatter(compX, compY, 2, color=pedColors, alpha=difalpha)

  if diflines and len(difX) > 0:
    for index in range(0, len(difXidx) - 1):
      dX = compX[difXidx[index]]
      dY = compY[difXidx[index]]
      x = pointsX[difXidx[index]]
      y = pointsY[difXidx[index]]
      dist = math.dist([dX, dY], [x, y])
      ax.plot([dX, x], [dY, y], color=pedColors[difXidx[index]] if args['pedColor'] else 'r', linestyle='-', linewidth=0.5, alpha=min(dist / args['diflineMax'], 1.0) if args['diflineMax'] else difalpha)

    for index in range(0, len(difYidx) - 1):
      dX = compX[difYidx[index]]
      dY = compY[difYidx[index]]
      x = pointsX[difYidx[index]]
      y = pointsY[difYidx[index]]
      dist = math.dist([dX, dY], [x, y])
      ax.plot([dX, x], [dY, y], color=pedColors[difYidx[index]] if args['pedColor'] else 'r', linestyle='-', linewidth=0.5, alpha=min(dist / args['diflineMax'], 1.0) if args['diflineMax'] else difalpha)
    
  return ax.scatter(difX, difY, 2, color=colors)

def plotExtras(ax, timestep, output, pedcolors):
  return

def updateColors(output):
  return