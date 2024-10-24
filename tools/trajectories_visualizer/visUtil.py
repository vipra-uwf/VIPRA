import sys
import os
import json
import math
import numpy as np
import matplotlib
import datetime
import ezdxf

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
  dxf = False

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
      if os.path.splitext(obs)[1] == '.dxf':
        dxf = True
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
    dxf      = dxf,
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

def f3d_to_array(f3d):
  return [f3d['x'], f3d['y'] if 'y' in f3d else 0]

def getObs(obs, dxf):
  if dxf:
      doc = ezdxf.readfile(obs)
      return doc.modelspace()

  obsCoords = json.load(open(obs))
  obstacles = []
  for obj in obsCoords["obstacles"]:
    obstacles.append([])
    for point in obj:
      obstacles[-1].append(f3d_to_array(point))
    obstacles[-1].append(f3d_to_array(obj[0]))

  return obstacles

def getPeds(peds):
  if peds:
    return json.load(open(peds))
  else:
    return {}


def getPoints(timestep):
  pointsX = []
  pointsY = []
  for ped in timestep:
    pointsX.append(ped[0])
    pointsY.append(ped[1])

  return (pointsX, pointsY)

def makeColors(peds, args):
  count = len(peds["timesteps"][0])
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

def plotObs(obstacles, ax, args):
  for obstacle in obstacles:
    poly = matplotlib.patches.Polygon(obstacle, linewidth=1, facecolor=args['obsColor'])
    ax.add_patch(poly)

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

def draw_dxf(msp, ax, args):
  # Iterate through entities in the model space
  for entity in msp:
      if entity.dxf.layer != 'obstacles':
        continue
      if entity.dxftype() == 'LINE':
          draw_line(ax, entity.dxf.start, entity.dxf.end)
      elif entity.dxftype() == 'CIRCLE':
          draw_circle(ax, (-entity.dxf.center.x, entity.dxf.center.y), entity.dxf.radius)
      elif entity.dxftype() == 'LWPOLYLINE':
          draw_lwpolyline(ax, entity)

# Function to draw a circle
def draw_circle(ax, center, radius):
    circle = matplotlib.pyplot.Circle(center, radius, fill=False, color='black')
    ax.add_patch(circle)

# Function to draw a lightweight polyline (LWPolyline)
def draw_lwpolyline(ax, points, is_closed):
    if is_closed:
        polygon = Polygon(points, closed=True, fill=False, edgecolor='black')
        ax.add_patch(polygon)
    else:
        ax.plot([p[0] for p in points], [p[1] for p in points], 'k-')

# Function to draw a line
def draw_line(ax, start, end):
    ax.plot([start.x, end.x], [start.y, end.y], 'k-')



def add_arc(ax, start, end, bulge):
    # Calculate the arc's radius and center
    dx, dy = end[0] - start[0], end[1] - start[1]
    dist = np.sqrt(dx**2 + dy**2)
    radius = dist * (1 + bulge**2) / (2 * bulge)
    
    # Middle point between start and end
    mid = [(start[0] + end[0]) / 2, (start[1] + end[1]) / 2]
    
    # Distance from midpoint to arc center
    sagitta = radius - dist / 2 * abs(bulge)
    angle = np.arctan2(dy, dx)
    
    # Determine center of the arc
    if bulge > 0:
        center = [mid[0] + sagitta * np.sin(angle), mid[1] - sagitta * np.cos(angle)]
    else:
        center = [mid[0] - sagitta * np.sin(angle), mid[1] + sagitta * np.cos(angle)]
    
    # Calculate start and end angles
    start_angle = np.degrees(np.arctan2(start[1] - center[1], start[0] - center[0]))
    end_angle = np.degrees(np.arctan2(end[1] - center[1], end[0] - center[0]))
    
    # Arc drawing
    if bulge < 0:
        if start_angle < end_angle:
            start_angle += 360
    else:
        if end_angle < start_angle:
            end_angle += 360

    arc = Arc(center, 2*radius, 2*radius, angle=0, theta1=start_angle, theta2=end_angle, color='black')
    ax.add_patch(arc)


def draw_lwpolyline(ax, entity):
    vertices = entity.get_points(format='xyb')
    for i in range(len(vertices) - 1):
        start, end = vertices[i], vertices[i + 1]
        bulge = start[2]
        if bulge == 0:
            ax.plot([start[0], end[0]], [start[1], end[1]], 'k-')
        else:
            add_arc(ax, start, end, bulge)




def draw_lwpolyline_o2(ax, entity):
    def add_arc(ax, start, end, bulge):
        # Calculate midpoint
        mid = ((start[0] + end[0]) / 2, (start[1] + end[1]) / 2)

        # Distance between start and end points
        dist = np.hypot(end[0] - start[0], end[1] - start[1])

        # Radius of the arc
        radius = dist / 2 * (1 + bulge**2) / (2 * bulge)

        # Angle of the line connecting start and end points
        angle = np.arctan2(end[1] - start[1], end[0] - start[0])

        # Distance from midpoint to arc center
        sagitta = radius - dist / 2 * bulge

        # Calculate center of the arc
        center_x = mid[0] + sagitta * np.sin(angle)
        center_y = mid[1] - sagitta * np.cos(angle)

        # Start and end angles
        start_angle = np.arctan2(start[1] - center_y, start[0] - center_x)
        end_angle = np.arctan2(end[1] - center_y, end[0] - center_x)

        # Correct angles for drawing
        if bulge < 0:
            start_angle, end_angle = end_angle, start_angle
        if end_angle < start_angle:
            end_angle += 2 * np.pi

        # Draw the arc
        arc = Arc((center_x, center_y), 2*radius, 2*radius, theta1=np.degrees(start_angle), theta2=np.degrees(end_angle),
                                      color='black', fill=False)
        ax.add_patch(arc)

    vertices = entity.get_points(format='xyb')  # x, y, bulge

    # Draw each segment
    for i in range(len(vertices) - 1):
        start, end = vertices[i], vertices[i + 1]
        bulge = start[2]
        if bulge == 0:
            ax.plot([start[0], end[0]], [start[1], end[1]], 'k-')
        else:
            add_arc(ax, start, end, bulge)

    # Close the polyline if it's closed, considering possible bulge in last segment
    if entity.closed:
        start, end = vertices[-1], vertices[0]
        bulge = start[2]
        if bulge == 0:
            ax.plot([start[0], end[0]], [start[1], end[1]], 'k-')
        else:
            add_arc(ax, start, end, bulge)



def draw_lwpolyline_o(ax, entity):
    vertices = entity.get_points(format='xyb')  # Get vertices and bulges
    for i in range(len(vertices) - 1):
        start, end = vertices[i], vertices[i + 1]
        bulge = start[2]
        if bulge == 0:
            # Draw straight line for segments with no bulge
            ax.plot([start[0], end[0]], [start[1], end[1]], 'k-')
        else:
            # Calculate arc for segments with bulge
            # Arc center, radius, start angle, and end angle calculation
            dx, dy = end[0] - start[0], end[1] - start[1]
            distance = np.hypot(dx, dy)
            radius = distance * (1 + bulge**2) / (4 * bulge)
            angle = np.arctan2(dy, dx)
            center = (start[0] + dx / 2 - radius * np.sin(angle),
                      start[1] + dy / 2 + radius * np.cos(angle))
            start_angle = np.degrees(np.arctan2(start[1] - center[1], start[0] - center[0]))
            end_angle = np.degrees(np.arctan2(end[1] - center[1], end[0] - center[0]))

            # Ensure the arc moves in the correct direction
            if bulge < 0:
                start_angle, end_angle = end_angle, start_angle
            # Correct the angles for drawing
            if end_angle <= start_angle:
                end_angle += 360

            # Draw the arc
            arc = Arc(center, 2*radius, 2*radius, angle=0, theta1=start_angle, theta2=end_angle, color='black')
            ax.add_patch(arc)

    # Close the polyline if it is closed
    if entity.closed:
        start, end = vertices[-1], vertices[0]
        bulge = vertices[-1][2]
        if bulge == 0:
            ax.plot([start[0], end[0]], [start[1], end[1]], 'k-')
        else:
            # Handle the last segment as an arc if needed, similar to above
            pass  # Implement arc drawing for the closing segment if needed
