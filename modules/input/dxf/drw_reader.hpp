#pragma once

#include <initializer_list>
#include "drw_base.h"
#include "drw_entities.h"
#include "drw_interface.h"

#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/polygon.hpp"
#include "vipra/logging/logging.hpp"

namespace VIPRA {
struct DrwReader : public DRW_Interface {
  static constexpr int                                         NUM_SUBDIVISIONS = 126;
  std::vector<VIPRA::Geometry::Polygon>                        obstacles;
  std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> objectives;
  std::vector<VIPRA::Geometry::Polygon>                        spawns;
  std::map<std::string, VIPRA::Geometry::Polygon>              areas;

  void addLWPolyline(const DRW_LWPolyline& data) override
  {
    bool isClosed = (data.flags & 0x01U) != 0U;

    if ( isClosed ) {
      VIPRA::f3dVec points;
      for ( auto const& vertex : data.vertlist ) {
        points.emplace_back(vertex->x, vertex->y);
      }

      add_obstacle(points, data.layer);

      return;
    }

    add_polyline_path(data);
  }

  void addLine(const DRW_Line& data) override
  {
    add_obstacle({VIPRA::f3d{data.basePoint.x, data.basePoint.y},
                  VIPRA::f3d{data.secPoint.x, data.secPoint.y}},
                 data.layer);
  }

  void addPoint(const DRW_Point& data) override
  {
    add_obstacle({VIPRA::f3d{data.basePoint.x, data.basePoint.y}}, data.layer);
  }

  void add_polyline_path(const DRW_LWPolyline& data)
  {
    // We add these as individidual lines to prevent the arc from closing at both ends.
    for ( int i = 0; i < data.vertexnum - 1; i++ ) {
      std::shared_ptr<DRW_Vertex2D> const& vertex = data.vertlist[i];
      std::shared_ptr<DRW_Vertex2D> const& nextVertex = data.vertlist[i];
      std::vector<VIPRA::f3d>              line = {VIPRA::f3d{vertex->x, vertex->y},
                                                   VIPRA::f3d{nextVertex->x, nextVertex->y}};

      add_obstacle(line, data.layer);
    }
  }

  void add_obstacle(std::initializer_list<VIPRA::f3d> const& points,
                    std::string                              object_type)
  {
    std::string objectTypeCaseless = object_type;
    transform(object_type.begin(), object_type.end(), object_type.begin(), ::toupper);

    if ( object_type == "OBSTACLES" ) {
      obstacles.emplace_back(points);
    }
    else if ( object_type == "SPAWNS" ) {
      spawns.emplace_back(points);
    }
    else if ( object_type.find("AREAS") != std::string::npos ) {
      areas[object_type] = Geometry::Polygon(points);
    }
    else {
      if ( objectives[objectTypeCaseless].empty() ) {
        objectives[objectTypeCaseless] = {Geometry::Polygon{points}};
      }
      else {
        objectives[objectTypeCaseless].emplace_back(points);
      }
    }
  }

  void add_obstacle(std::vector<VIPRA::f3d> const& points, std::string object_type)
  {
    std::string objectTypeCaseless = object_type;
    transform(object_type.begin(), object_type.end(), object_type.begin(), ::toupper);

    if ( object_type == "OBSTACLES" ) {
      obstacles.emplace_back(points);
    }
    else if ( object_type == "SPAWNS" ) {
      spawns.emplace_back(points);
    }
    else if ( object_type.find("AREAS") != std::string::npos ) {
      areas[object_type] = Geometry::Polygon(points);
    }
    else {
      if ( objectives[objectTypeCaseless].empty() ) {
        objectives[objectTypeCaseless] = {Geometry::Polygon{points}};
      }
      else {
        objectives[objectTypeCaseless].emplace_back(points);
      }
    }
  }

  void addRay(const DRW_Ray& data) override
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

  void addXline(const DRW_Xline& data) override
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

  void addArc(const DRW_Arc& data) override
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

  void addCircle(const DRW_Circle& data) override
  {
    // VIPRA::Log::warn("DXF Loading for Circle not impelmented");

    DRW_Circle dataCpy = data;

    double    radius = dataCpy.radious;
    DRW_Coord center = dataCpy.basePoint;

    auto vipraPolyline = DrwReader::get_arc_as_polygon(center, radius, 0.00, (2 * M_PI));

    std::string layer = data.layer;
    add_obstacle(vipraPolyline, layer);
  }

  void addEllipse(const DRW_Ellipse& data) override
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

  void addPolyline(const DRW_Polyline& data) override
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

  void addSpline(const DRW_Spline* /*data*/) override
  {
    VIPRA::Log::warn("DXF Loading for Spline not impelmented");
  }
  void addKnot(const DRW_Entity& /*data*/) override
  {
    VIPRA::Log::warn("DXF Loading for Knot not impelmented");
  }
  void addInsert(const DRW_Insert& /*data*/) override
  {
    VIPRA::Log::warn("DXF Loading for Insert not impelmented");
  }
  void addTrace(const DRW_Trace& /*data*/) override
  {
    VIPRA::Log::warn("DXF Loading for Trace not impelmented");
  }
  void add3dFace(const DRW_3Dface& /*data*/) override
  {
    VIPRA::Log::warn("DXF Loading for 3dFace not impelmented");
  }
  void addSolid(const DRW_Solid& /*data*/) override
  {
    VIPRA::Log::warn("DXF Loading for Solid not impelmented");
  }
  void addHeader(const DRW_Header* data) override {}
  void addLType(const DRW_LType& data) override {}
  void addLayer(const DRW_Layer& data) override {}
  void addDimStyle(const DRW_Dimstyle& data) override {}
  void addVport(const DRW_Vport& data) override {}
  void addTextStyle(const DRW_Textstyle& data) override {}
  void addAppId(const DRW_AppId& data) override {}
  void addBlock(const DRW_Block& data) override {}
  void setBlock(const int handle) override {}
  void endBlock() override {}
  void addMText(const DRW_MText& data) override {}
  void addText(const DRW_Text& data) override {}
  void addDimAlign(const DRW_DimAligned* data) override {}
  void addDimLinear(const DRW_DimLinear* data) override {}
  void addDimRadial(const DRW_DimRadial* data) override {}
  void addDimDiametric(const DRW_DimDiametric* data) override {}
  void addDimAngular(const DRW_DimAngular* data) override {}
  void addDimAngular3P(const DRW_DimAngular3p* data) override {}
  void addDimOrdinate(const DRW_DimOrdinate* data) override {}
  void addLeader(const DRW_Leader* data) override {}
  void addHatch(const DRW_Hatch* data) override {}
  void addViewport(const DRW_Viewport& data) override {}
  void addImage(const DRW_Image* data) override {}
  void linkImage(const DRW_ImageDef* data) override {}
  void addComment(const char* comment) override {}
  void addPlotSettings(const DRW_PlotSettings* data) override {}
  void writeHeader(DRW_Header& data) override {}
  void writeBlocks() override {}
  void writeBlockRecords() override {}
  void writeEntities() override {}
  void writeLTypes() override {}
  void writeLayers() override {}
  void writeTextstyles() override {}
  void writeVports() override {}
  void writeDimstyles() override {}
  void writeObjects() override {}
  void writeAppId() override {}

  /**
   * @brief Takes in a list of vectors in the form of DRW_Vertex and returns a set of f3d points usable by VIPRA
   *
   * @param dxfPolygonVertices - A list of vectors in the form of DRW_Vertex
   * @return std::vector<VIPRA::f3d>
   */
  auto get_arc_as_polygon(DRW_Coord center, double radius,
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
   * @brief Takes in a list of vectors in the form of DRW_Vertex and returns a set of f3d points usable by VIPRA
   *
   * @param dxfPolygonVertices - A list of vectors in the form of DRW_Vertex
   * @return std::vector<VIPRA::f3d>
   */
  auto get_polygon(std::vector<std::shared_ptr<DRW_Vertex>>&
                                  vertexList) -> std::vector<VIPRA::f3d>
  {
    std::vector<VIPRA::f3d> points;
    for ( std::shared_ptr<DRW_Vertex> const& vertex : vertexList ) {
      DRW_Vertex currVertex = *vertex;

      points.emplace_back(currVertex.basePoint.x, currVertex.basePoint.y, 0);
    }

    return points;
  }

};

}  // namespace VIPRA