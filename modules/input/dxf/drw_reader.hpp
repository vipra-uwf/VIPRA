#pragma once

#include "drw_interface.h"

#include "vipra/geometry/polygon.hpp"

namespace VIPRA {
class DrwReader : public DRW_Interface {
 public:
  DrwReader(const DrwReader&) = default;
  DrwReader(DrwReader&&) = default;
  auto operator=(const DrwReader&) -> DrwReader& = default;
  auto operator=(DrwReader&&) -> DrwReader& = default;
  ~DrwReader() override = default;
  DrwReader() = default;

  static const int NUM_SUBDIVISIONS = 126;

  [[nodiscard]] auto get_vipra_polygons() const
      -> const std::vector<VIPRA::Geometry::Polygon>&
  {
    return this->_obstacles;
  };

  auto get_vipra_goals()
      -> const std::map<std::string, std::vector<VIPRA::Geometry::Polygon>>&
  {
    return this->_objectives;
  };

  auto get_vipra_pedestrians() -> const std::vector<VIPRA::Geometry::Polygon>&
  {
    return this->_spawns;
  };

  /** Called when header is parsed.  */
  void addHeader(const DRW_Header* data) override {

  };

  [[nodiscard]] auto get_obstacles() -> std::vector<VIPRA::Geometry::Polygon>
  {
    return _obstacles;
  }
  [[nodiscard]] auto get_objectives()
      -> std::map<std::string, std::vector<VIPRA::Geometry::Polygon>>
  {
    return _objectives;
  }
  [[nodiscard]] auto get_pedestrians() -> std::vector<VIPRA::Geometry::Polygon>
  {
    return _spawns;
  }
  [[nodiscard]] auto get_areas()
      -> std::map<std::string, VIPRA::Geometry::Polygon>
  {
    return _areas;
  }

  /** Called for every line Type.  */
  void addLType(const DRW_LType& data) override {};
  /** Called for every layer. */
  void addLayer(const DRW_Layer& data) override {};
  /** Called for every dim style. */
  void addDimStyle(const DRW_Dimstyle& data) override {};
  /** Called for every VPORT table. */
  void addVport(const DRW_Vport& data) override {};
  /** Called for every text style. */
  void addTextStyle(const DRW_Textstyle& data) override {};
  /** Called for every AppId entry. */
  void addAppId(const DRW_AppId& data) override {};

  /**
   * Called for every block. Note: all entities added after this
   * command go into this block until endBlock() is called.
   *
   * @see endBlock()
   */
  void addBlock(const DRW_Block& data) override {};

  /**
   * In DWG called when the following entities corresponding to a
   * block different from the current. Note: all entities added after this
   * command go into this block until setBlock() is called already.
   *
   * int handle are the value of DRW_Block::handleBlock added with addBlock()
   */
  void setBlock(const int handle) override {};

  /** Called to end the current block */
  void endBlock() override {};

  /** Called for every point */
  void addPoint(const DRW_Point& data) override;

  /** Called for every line */
  void addLine(const DRW_Line& data) override;

  /** Called for every ray */
  void addRay(const DRW_Ray& data) override;

  /** Called for every xline */
  void addXline(const DRW_Xline& data) override;

  /** Called for every arc */
  void addArc(const DRW_Arc& data) override;

  /** Called for every circle */
  void addCircle(const DRW_Circle& data) override;

  /** Called for every ellipse */
  void addEllipse(const DRW_Ellipse& data) override;

  /** Called for every lwpolyline */
  void addLWPolyline(const DRW_LWPolyline& data) override;

  /** Called for every polyline start */
  void addPolyline(const DRW_Polyline& data) override;

  /***************************
   * Unused, expand as needed.
   **************************/
  /** Called for every spline */
  void addSpline(const DRW_Spline* data) override {};
  /** Called for every spline knot value */
  void addKnot(const DRW_Entity& data) override {};
  /** Called for every insert. */
  void addInsert(const DRW_Insert& data) override {};
  /** Called for every trace start */
  void addTrace(const DRW_Trace& data) override {};
  /** Called for every 3dface start */
  void add3dFace(const DRW_3Dface& data) override {};
  /** Called for every solid start */
  void addSolid(const DRW_Solid& data) override {};
  /** Called for every Multi Text entity. */
  void addMText(const DRW_MText& data) override {};
  /** Called for every Text entity. */
  void addText(const DRW_Text& data) override {};
  /** Called for every aligned dimension entity. */
  void addDimAlign(const DRW_DimAligned* data) override {};
  /** Called for every linear or rotated dimension entity. */
  void addDimLinear(const DRW_DimLinear* data) override {};
  /** Called for every radial dimension entity. */
  void addDimRadial(const DRW_DimRadial* data) override {};
  /** Called for every diametric dimension entity. */
  void addDimDiametric(const DRW_DimDiametric* data) override {};
  /** Called for every angular dimension (2 lines version) entity. */
  void addDimAngular(const DRW_DimAngular* data) override {};
  /** Called for every angular dimension (3 points version) entity. */
  void addDimAngular3P(const DRW_DimAngular3p* data) override {};
  /** Called for every ordinate dimension entity. */
  void addDimOrdinate(const DRW_DimOrdinate* data) override {};
  /** Called for every leader start. */
  void addLeader(const DRW_Leader* data) override {};
  /** Called for every hatch entity. */
  void addHatch(const DRW_Hatch* data) override {};
  /** Called for every viewport entity. */
  void addViewport(const DRW_Viewport& data) override {};
  /** Called for every image entity. */
  void addImage(const DRW_Image* data) override {};
  /** Called for every image definition. */
  void linkImage(const DRW_ImageDef* data) override {};
  /** Called for every comment in the DXF file (code 999). */
  void addComment(const char* comment) override {};
  /** Called for PLOTSETTINGS object definition. */
  void addPlotSettings(const DRW_PlotSettings* data) override {};
  void writeHeader(DRW_Header& data) override {};
  void writeBlocks() override {};
  void writeBlockRecords() override {};
  void writeEntities() override {};
  void writeLTypes() override {};
  void writeLayers() override {};
  void writeTextstyles() override {};
  void writeVports() override {};
  void writeDimstyles() override {};
  void writeObjects() override {};
  void writeAppId() override {};
  /**************************/

 private:
  std::vector<VIPRA::Geometry::Polygon>                        _obstacles;
  std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> _objectives;
  std::vector<VIPRA::Geometry::Polygon>                        _spawns;
  std::map<std::string, VIPRA::Geometry::Polygon>              _areas;

  void add_obstacle(const std::vector<VIPRA::f3d>& points,
                    std::string                    object_type);

  [[nodiscard]] static auto get_polygon(
      std::vector<std::shared_ptr<DRW_Vertex>>& vertexList)
      -> std::vector<VIPRA::f3d>;

  [[nodiscard]] static auto get_2d_vector_polygon(
      std::vector<std::shared_ptr<DRW_Vertex2D>>& vertexList)
      -> std::vector<VIPRA::f3d>;

  [[nodiscard]] static auto get_arc_as_polygon(
      DRW_Coord center, double radius, double start_angle,
      double end_angle) -> std::vector<VIPRA::f3d>;
};

}  // namespace VIPRA