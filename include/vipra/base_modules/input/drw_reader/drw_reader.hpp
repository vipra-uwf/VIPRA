#pragma once

#include "vipra.hpp"

#include "drw_interface.h"

#include "vipra/geometry/polygon.hpp"

namespace VIPRA {
class DrwReader : public DRW_Interface {
 public:
  static const int NUM_SUBDIVISIONS = 126;

  ~DrwReader() override = default;

  [[nodiscard]] auto get_vipra_polygons() const -> const std::vector<VIPRA::Geometry::Polygon>&
  {
    return this->_obstacles;
  };

  auto get_vipra_goals() -> const std::map<std::string, std::vector<VIPRA::Geometry::Polygon>>&
  {
    return this->_objectives;
  };

  auto get_vipra_pedestrians() -> const std::vector<VIPRA::Geometry::Polygon>& { return this->_spawns; };

  /** Called when header is parsed.  */
  void addHeader(const DRW_Header* data) override {
    
  };

  std::vector<VIPRA::Geometry::Polygon>                        getObstacles() { return _obstacles; }
  std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> getObjectives() { return _objectives; }
  std::vector<VIPRA::Geometry::Polygon>                        getPedestrians() { return _spawns; }
  std::map<std::string, VIPRA::Geometry::Polygon>              getAreas() { return _areas; }

  /** Called for every line Type.  */
  void addLType(const DRW_LType& data) {};
  /** Called for every layer. */
  void addLayer(const DRW_Layer& data) {};
  /** Called for every dim style. */
  void addDimStyle(const DRW_Dimstyle& data) {};
  /** Called for every VPORT table. */
  void addVport(const DRW_Vport& data) {};
  /** Called for every text style. */
  void addTextStyle(const DRW_Textstyle& data) {};
  /** Called for every AppId entry. */
  void addAppId(const DRW_AppId& data) {};

  /**
   * Called for every block. Note: all entities added after this
   * command go into this block until endBlock() is called.
   *
   * @see endBlock()
   */
  void addBlock(const DRW_Block& data) {};

  /**
   * In DWG called when the following entities corresponding to a
   * block different from the current. Note: all entities added after this
   * command go into this block until setBlock() is called already.
   *
   * int handle are the value of DRW_Block::handleBlock added with addBlock()
   */
  void setBlock(const int handle) {};

  /** Called to end the current block */
  void endBlock() {};

  /** Called for every point */
  void addPoint(const DRW_Point& data)
  {
    DRW_Coord data_cpy = data.basePoint;

    // Even if it's only 1 point, we still want to organize it as a vector of points.
    std::vector<VIPRA::f3d> points;

    points.emplace_back(data_cpy.x, data_cpy.y, 0);

    _vipraPolygons.emplace_back(points);

    std::string layer = data.layer;
    add_obstacle(points, layer);
  };

  /** Called for every line */
  void addLine(const DRW_Line& data)
  {
    DRW_Line data_cpy = data;

    // Even if it's only 2 points, we still want to organize it as a vector of points.
    std::vector<VIPRA::f3d> points;

    points.emplace_back(data_cpy.basePoint.x, data_cpy.basePoint.y, 0);  // Start point
    points.emplace_back(data_cpy.secPoint.x, data_cpy.secPoint.y, 0);    // End point

    _vipraPolygons.emplace_back(points);

    std::string layer = data.layer;
    add_obstacle(points, layer);
  };

  /** Called for every ray */
  void addRay(const DRW_Ray& data)
  {
    DRW_Line data_cpy = data;

    // Even if it's only 2 points, we still want to organize it as a vector of points.
    std::vector<VIPRA::f3d> points;

    points.emplace_back(data_cpy.basePoint.x, data_cpy.basePoint.y, 0);  // Start point
    points.emplace_back(data_cpy.secPoint.x, data_cpy.secPoint.y, 0);    // End point

    _vipraPolygons.emplace_back(points);

    std::string layer = data.layer;
    add_obstacle(points, layer);
  };

  /** Called for every xline */
  void addXline(const DRW_Xline& data)
  {
    DRW_Line data_cpy = data;

    // Even if it's only 2 points, we still want to organize it as a vector of points.
    std::vector<VIPRA::f3d> points;

    points.emplace_back(data_cpy.basePoint.x, data_cpy.basePoint.y, 0);  // Start point
    points.emplace_back(data_cpy.secPoint.x, data_cpy.secPoint.y, 0);    // End point

    _vipraPolygons.emplace_back(points);

    std::string layer = data.layer;
    add_obstacle(points, layer);
  };

  /** Called for every arc */
  void addArc(const DRW_Arc& data)
  {
    DRW_Arc data_cpy = data;

    double    radius = data_cpy.radius();
    DRW_Coord center = data_cpy.basePoint;

    auto vipra_polyline = DrwReader::get_arc_as_polygon(center, radius, data.staangle, data.endangle);

    _vipraPolygons.emplace_back(vipra_polyline);

    std::string layer = data.layer;
    add_obstacle(vipra_polyline, layer);
  };

  /** Called for every circle */
  void addCircle(const DRW_Circle& data)
  {
    DRW_Circle data_cpy = data;

    double    radius = data_cpy.radious;
    DRW_Coord center = data_cpy.basePoint;

    auto vipra_polyline = DrwReader::get_arc_as_polygon(center, radius, 0.00, (2 * M_PI));

    _vipraPolygons.emplace_back(vipra_polyline);

    std::string layer = data.layer;
    add_obstacle(vipra_polyline, layer);
  };

  /** Called for every ellipse */
  void addEllipse(const DRW_Ellipse& data)
  {
    // Get ellipse into polyline
    DRW_Polyline ellipse_line = DRW_Polyline();
    DRW_Ellipse  data_cpy = data;
    data_cpy.toPolyline(&ellipse_line, NUM_SUBDIVISIONS);

    // Extract points from polyline
    std::vector<std::shared_ptr<DRW_Vertex>> vertex_list = ellipse_line.vertlist;
    auto                                     vipra_polyline = DrwReader::get_polygon(vertex_list);

    _vipraPolygons.emplace_back(vipra_polyline);

    std::string layer = data.layer;
    add_obstacle(vipra_polyline, layer);
  };

  /** Called for every lwpolyline */
  void addLWPolyline(const DRW_LWPolyline& data)
  {
    // Extract points from polyline
    std::vector<std::shared_ptr<DRW_Vertex2D>> vertex_list = data.vertlist;
    auto                                       vipra_polyline = DrwReader::get_2D_vector_polygon(vertex_list);

    _vipraPolygons.emplace_back(vipra_polyline);

    std::string layer = data.layer;
    add_obstacle(vipra_polyline, layer);
  };

  /** Called for every polyline start */
  void addPolyline(const DRW_Polyline& data)
  {
    // Extract points from polyline
    std::vector<std::shared_ptr<DRW_Vertex>> vertex_list = data.vertlist;
    auto                                     vipra_polyline = DrwReader::get_polygon(vertex_list);

    _vipraPolygons.emplace_back(vipra_polyline);

    std::string layer = data.layer;
    add_obstacle(vipra_polyline, layer);
  };

  /***************************
   * Unused, expand as needed.
   **************************/
  /** Called for every spline */
  void addSpline(const DRW_Spline* data) {};
  /** Called for every spline knot value */
  void addKnot(const DRW_Entity& data) {};
  /** Called for every insert. */
  void addInsert(const DRW_Insert& data) {};
  /** Called for every trace start */
  void addTrace(const DRW_Trace& data) {};
  /** Called for every 3dface start */
  void add3dFace(const DRW_3Dface& data) {};
  /** Called for every solid start */
  void addSolid(const DRW_Solid& data) {};
  /** Called for every Multi Text entity. */
  void addMText(const DRW_MText& data) {};
  /** Called for every Text entity. */
  void addText(const DRW_Text& data) {};
  /** Called for every aligned dimension entity. */
  void addDimAlign(const DRW_DimAligned* data) {};
  /** Called for every linear or rotated dimension entity. */
  void addDimLinear(const DRW_DimLinear* data) {};
  /** Called for every radial dimension entity. */
  void addDimRadial(const DRW_DimRadial* data) {};
  /** Called for every diametric dimension entity. */
  void addDimDiametric(const DRW_DimDiametric* data) {};
  /** Called for every angular dimension (2 lines version) entity. */
  void addDimAngular(const DRW_DimAngular* data) {};
  /** Called for every angular dimension (3 points version) entity. */
  void addDimAngular3P(const DRW_DimAngular3p* data) {};
  /** Called for every ordinate dimension entity. */
  void addDimOrdinate(const DRW_DimOrdinate* data) {};
  /** Called for every leader start. */
  void addLeader(const DRW_Leader* data) {};
  /** Called for every hatch entity. */
  void addHatch(const DRW_Hatch* data) {};
  /** Called for every viewport entity. */
  void addViewport(const DRW_Viewport& data) {};
  /** Called for every image entity. */
  void addImage(const DRW_Image* data) {};
  /** Called for every image definition. */
  void linkImage(const DRW_ImageDef* data) {};
  /** Called for every comment in the DXF file (code 999). */
  void addComment(const char* comment) {};
  /** Called for PLOTSETTINGS object definition. */
  void addPlotSettings(const DRW_PlotSettings* data) {};
  void writeHeader(DRW_Header& data) {};
  void writeBlocks() {};
  void writeBlockRecords() {};
  void writeEntities() {};
  void writeLTypes() {};
  void writeLayers() {};
  void writeTextstyles() {};
  void writeVports() {};
  void writeDimstyles() {};
  void writeObjects() {};
  void writeAppId() {};
  /**************************/

 private:
  std::vector<VIPRA::Geometry::Polygon> _vipraPolygons;

  std::vector<VIPRA::Geometry::Polygon>                        _obstacles;
  std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> _objectives;
  std::vector<VIPRA::Geometry::Polygon>                        _spawns;
  std::map<std::string, VIPRA::Geometry::Polygon>              _areas;

  void add_obstacle(const std::vector<VIPRA::f3d>& points, std::string object_type);

  [[nodiscard]] static auto get_polygon(std::vector<std::shared_ptr<DRW_Vertex>>& vertexList)
      -> std::vector<VIPRA::f3d>;

  [[nodiscard]] static auto get_2D_vector_polygon(std::vector<std::shared_ptr<DRW_Vertex2D>>& vertexList)
      -> std::vector<VIPRA::f3d>;

  [[nodiscard]] static auto get_arc_as_polygon(DRW_Coord center, double radius, double start_angle,
                                               double end_angle) -> std::vector<VIPRA::f3d>;
};

/**
 * @brief Takes in a list of vectors in the form of DRW_Vertex and returns a set of f3d points usable by VIPRA
 *
 * @param dxf_polygon_vertices - A list of vectors in the form of DRW_Vertex
 * @return std::vector<VIPRA::f3d>
 */
inline auto DrwReader::get_polygon(std::vector<std::shared_ptr<DRW_Vertex>>& dxf_polygon_vertices)
    -> std::vector<VIPRA::f3d>
{
  std::vector<VIPRA::f3d> points;
  for ( std::shared_ptr<DRW_Vertex> const& vertex : dxf_polygon_vertices ) {
    DRW_Vertex curr_vertex = *vertex;

    points.emplace_back(curr_vertex.basePoint.x, curr_vertex.basePoint.y, 0);
  }

  return points;
}

/**
 * @brief Takes in a list of vectors in the form of DRW_Vertex and returns a set of f3d points usable by VIPRA
 *
 * @param dxf_polygon_vertices - A list of vectors in the form of DRW_Vertex
 * @return std::vector<VIPRA::f3d>
 */
inline auto DrwReader::get_2D_vector_polygon(std::vector<std::shared_ptr<DRW_Vertex2D>>& dxf_polygon_vertices)
    -> std::vector<VIPRA::f3d>
{
  std::vector<VIPRA::f3d> points;
  for ( std::shared_ptr<DRW_Vertex2D> const& vertex : dxf_polygon_vertices ) {
    DRW_Vertex2D curr_vertex = *vertex;

    points.emplace_back(curr_vertex.x, curr_vertex.y, 0);
  }

  return points;
}

/**
 * @brief Takes in a list of vectors in the form of DRW_Vertex and returns a set of f3d points usable by VIPRA
 *
 * @param dxf_polygon_vertices - A list of vectors in the form of DRW_Vertex
 * @return std::vector<VIPRA::f3d>
 */
inline auto DrwReader::get_arc_as_polygon(DRW_Coord center, double radius, double start_angle,
                                          double end_angle) -> std::vector<VIPRA::f3d>
{
  std::vector<VIPRA::f3d> points;
  double                  center_x = center.x;
  double                  center_y = center.y;

  // Increments by steps to create points around a center at the specified radius.
  // For a circle, start point would be 0.00 and end point would be 2*pi
  // Steps are defined as the angle resulting from subdividing 2*pi
  double step_size = (2 * M_PI) / NUM_SUBDIVISIONS;
  // TODO: consider tolerance with end state. Could create two points very close together.
  for ( double step = start_angle; step < end_angle; step += step_size ) {
    double x = center_x + radius * cos(step);
    double y = center_y + radius * sin(step);

    points.emplace_back(x, y, 0);
  }

  // Should add point on last point if not circle. This closes
  // the shape if the arc is attached to another shape
  if ( std::abs(end_angle - (2 * M_PI)) > 0.01 ) {
    points.emplace_back(center_x + (radius * cos(end_angle)), center_y + (radius * sin(end_angle)), 0);
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
inline void DrwReader::add_obstacle(std::vector<VIPRA::f3d> const& points, std::string object_type)
{
  transform(object_type.begin(), object_type.end(), object_type.begin(), ::toupper);

  VIPRA::Geometry::Polygon shape(points);

  if ( object_type == "OBSTACLES" ) {
    _obstacles.emplace_back(shape);
  }
  else if ( object_type == "PEDESTRIANS" ) {
    // _spawns.insert(_spawns.end(), points.begin(), points.end());
    _spawns.emplace_back(shape);
  }
  else if ( object_type.find("AREAS") != std::string::npos ) {
    _areas[object_type] = shape;
  }
  else {
    if ( _objectives[object_type].empty() ) {
      _objectives[object_type] = {shape};
    }
    else {
      _objectives[object_type].emplace_back(shape);
    }
  }
};
}  // namespace VIPRA
