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

  void addRay(const DRW_Ray& /*data*/) override
  {
    VIPRA::Log::warn("DXF Loading for Ray not impelmented");
  }
  void addXline(const DRW_Xline& /*data*/) override
  {
    VIPRA::Log::warn("DXF Loading for Xline not impelmented");
  }
  void addArc(const DRW_Arc& /*data*/) override
  {
    VIPRA::Log::warn("DXF Loading for Arc not impelmented");
  }
  void addCircle(const DRW_Circle& /*data*/) override
  {
    VIPRA::Log::warn("DXF Loading for Circle not impelmented");
  }
  void addEllipse(const DRW_Ellipse& /*data*/) override
  {
    VIPRA::Log::warn("DXF Loading for Ellipse not impelmented");
  }
  void addPolyline(const DRW_Polyline& /*data*/) override
  {
    VIPRA::Log::warn("DXF Loading for Polyline not impelmented");
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
};

}  // namespace VIPRA