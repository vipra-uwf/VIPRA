
#include "drw_reader.hpp"

namespace VIPRA {
/**
 * @brief Takes in a list of vectors in the form of DRW_Vertex and returns a set of f3d points usable by VIPRA
 *
 * @param dxfPolygonVertices - A list of vectors in the form of DRW_Vertex
 * @return std::vector<VIPRA::f3d>
 */
auto DrwReader::get_polygon(std::vector<std::shared_ptr<DRW_Vertex>>&
                                vertexList) -> std::vector<VIPRA::f3d>
{
  std::vector<VIPRA::f3d> points;
  for ( std::shared_ptr<DRW_Vertex> const& vertex : vertexList ) {
    DRW_Vertex currVertex = *vertex;

    points.emplace_back(currVertex.basePoint.x, currVertex.basePoint.y, 0);
  }

  return points;
}

/**
 * @brief Takes in a list of vectors in the form of DRW_Vertex and returns a set of f3d points usable by VIPRA
 *
 * @param dxfPolygonVertices - A list of vectors in the form of DRW_Vertex
 * @return std::vector<VIPRA::f3d>
 */
auto DrwReader::get_2d_vector_polygon(
    std::vector<std::shared_ptr<DRW_Vertex2D>>& vertexList)
    -> std::vector<VIPRA::f3d>
{
  std::vector<VIPRA::f3d> points;
  for ( std::shared_ptr<DRW_Vertex2D> const& vertex : vertexList ) {
    DRW_Vertex2D currVertex = *vertex;

    points.emplace_back(currVertex.x, currVertex.y, 0);
  }

  return points;
}

/**
 * @brief Takes in a list of vectors in the form of DRW_Vertex and returns a set of f3d points usable by VIPRA
 *
 * @param dxfPolygonVertices - A list of vectors in the form of DRW_Vertex
 * @return std::vector<VIPRA::f3d>
 */
auto DrwReader::get_arc_as_polygon(DRW_Coord center, double radius,
                                   double start_angle,
                                   double end_angle) -> std::vector<VIPRA::f3d>
{
  std::vector<VIPRA::f3d> points;
  double                  centerX = center.x;
  double                  centerY = center.y;

  // Increments by steps to create points around a center at the specified radius.
  // For a circle, start point would be 0.00 and end point would be 2*pi
  // Steps are defined as the angle resulting from subdividing 2*pi
  double stepSize = (2 * M_PI) / NUM_SUBDIVISIONS;
  // TODO(tyler): consider tolerance with end state. Could create two points very close together.
  for ( double step = start_angle; step < end_angle; step += stepSize ) {
    double x = centerX + radius * cos(step);
    double y = centerY + radius * sin(step);

    points.emplace_back(x, y, 0);
  }

  // Should add point on last point if not circle. This closes
  // the shape if the arc is attached to another shape
  if ( std::abs(end_angle - (2 * M_PI)) > 0.01 ) {
    points.emplace_back(centerX + (radius * cos(end_angle)),
                        centerY + (radius * sin(end_angle)), 0);
  }

  return points;
}

/**
 * @brief Uses the layer name to identify which vector this should go in based on the layer.
 *
 * @param points - A vector of VIPRA::f3d points that represent a geometric shape.
 * @param object_type - Either "PEDESTRIANS", "GOALS", or "OBSTACLES". "OBSTACLES" by default.
 * @return std::vector<VIPRA::Geometry::Polygon>
 */
void DrwReader::add_obstacle(std::vector<VIPRA::f3d> const& points,
                             std::string                    object_type)
{
  std::string objectTypeCaseless = object_type;
  transform(object_type.begin(), object_type.end(), object_type.begin(),
            ::toupper);

  VIPRA::Geometry::Polygon shape(points);

  if ( object_type == "OBSTACLES" ) {
    _obstacles.emplace_back(shape);
  }
  else if ( object_type == "SPAWNS" ) {
    // _spawns.insert(_spawns.end(), points.begin(), points.end());
    _spawns.emplace_back(shape);
  }
  else if ( object_type.find("AREAS") != std::string::npos ) {
    _areas[object_type] = shape;
  }
  else {
    if ( _objectives[objectTypeCaseless].empty() ) {
      _objectives[objectTypeCaseless] = {shape};
    }
    else {
      _objectives[objectTypeCaseless].emplace_back(shape);
    }
  }
}

void DrwReader::addPoint(const DRW_Point& data)
{
  DRW_Coord dataCpy = data.basePoint;

  // Even if it's only 1 point, we still want to organize it as a vector of points.
  std::vector<VIPRA::f3d> points;

  points.emplace_back(dataCpy.x, dataCpy.y, 0);

  std::string layer = data.layer;
  add_obstacle(points, layer);
}

/** Called for every line */
void DrwReader::addLine(const DRW_Line& data)
{
  DRW_Line dataCpy = data;

  // Even if it's only 2 points, we still want to organize it as a vector of points.
  std::vector<VIPRA::f3d> points;

  points.emplace_back(dataCpy.basePoint.x, dataCpy.basePoint.y,
                      0);  // Start point
  points.emplace_back(dataCpy.secPoint.x, dataCpy.secPoint.y,
                      0);  // End point

  std::string layer = data.layer;
  add_obstacle(points, layer);
}

/** Called for every ray */
void DrwReader::addRay(const DRW_Ray& data)
{
  DRW_Line dataCpy = data;

  // Even if it's only 2 points, we still want to organize it as a vector of points.
  std::vector<VIPRA::f3d> points;

  points.emplace_back(dataCpy.basePoint.x, dataCpy.basePoint.y,
                      0);  // Start point
  points.emplace_back(dataCpy.secPoint.x, dataCpy.secPoint.y,
                      0);  // End point

  std::string layer = data.layer;
  add_obstacle(points, layer);
}

/** Called for every xline */
void DrwReader::addXline(const DRW_Xline& data)
{
  DRW_Line dataCpy = data;

  // Even if it's only 2 points, we still want to organize it as a vector of points.
  std::vector<VIPRA::f3d> points;

  points.emplace_back(dataCpy.basePoint.x, dataCpy.basePoint.y,
                      0);  // Start point
  points.emplace_back(dataCpy.secPoint.x, dataCpy.secPoint.y,
                      0);  // End point

  std::string layer = data.layer;
  add_obstacle(points, layer);
}

/** Called for every arc */
void DrwReader::addArc(const DRW_Arc& data)
{
  DRW_Arc dataCpy = data;

  double    radius = dataCpy.radius();
  DRW_Coord center = dataCpy.basePoint;

  auto points = DrwReader::get_arc_as_polygon(
      center, radius, data.staangle, data.endangle);

  std::string layer = data.layer;

  // We add these as individidual lines to prevent the arc from closing at both ends. 
  for (int i = 0; i < points.size()-1; i++) {
    std::vector<VIPRA::f3d> line = {points[i], points[i+1]};

    add_obstacle(line, layer);
  }
}

/** Called for every circle */
void DrwReader::addCircle(const DRW_Circle& data)
{
  DRW_Circle dataCpy = data;

  double    radius = dataCpy.radious;
  DRW_Coord center = dataCpy.basePoint;

  auto vipraPolyline =
      DrwReader::get_arc_as_polygon(center, radius, 0.00, (2 * M_PI));

  std::string layer = data.layer;
  add_obstacle(vipraPolyline, layer);
}

/** Called for every ellipse */
void DrwReader::addEllipse(const DRW_Ellipse& data)
{
  // Get ellipse into polyline
  DRW_Polyline ellipseLine = DRW_Polyline();
  DRW_Ellipse  dataCpy = data;
  dataCpy.toPolyline(&ellipseLine, NUM_SUBDIVISIONS);

  // Extract points from polyline
  std::vector<std::shared_ptr<DRW_Vertex>> vertexList = ellipseLine.vertlist;
  auto vipraPolyline = DrwReader::get_polygon(vertexList);

  std::string layer = data.layer;
  add_obstacle(vipraPolyline, layer);
}

/** Called for every lwpolyline */
void DrwReader::addLWPolyline(const DRW_LWPolyline& data)
{
  // Extract points from polyline
  std::vector<std::shared_ptr<DRW_Vertex2D>> vertexList = data.vertlist;
  auto points = DrwReader::get_2d_vector_polygon(vertexList);

  std::string layer = data.layer;

  // If the last point is the same as the first point, close it off. 
  if (points[0].x == points.back().x && points[0].y == points.back().y) {
    add_obstacle(points, layer);
    return;
  }

  // We add these as individidual lines to prevent the arc from closing at both ends. 
  for (int i = 0; i < points.size()-1; i++) {
    std::vector<VIPRA::f3d> line = {points[i], points[i+1]};

    add_obstacle(line, layer);
  }
}

/** Called for every polyline start */
void DrwReader::addPolyline(const DRW_Polyline& data)
{
  // Extract points from polyline
  std::vector<std::shared_ptr<DRW_Vertex>> vertexList = data.vertlist;
  auto points = DrwReader::get_polygon(vertexList);

  std::string layer = data.layer;

  // If the last point is the same as the first point, close it off. 
  if (points[0].x == points.back().x && points[0].y == points.back().y) {
    add_obstacle(points, layer);
    return;
  }

  // We add these as individidual lines to prevent the arc from closing at both ends. 
  for (int i = 0; i < points.size()-1; i++) {
    std::vector<VIPRA::f3d> line = {points[i], points[i+1]};
    add_obstacle(line, layer);
  }
}
}  // namespace VIPRA