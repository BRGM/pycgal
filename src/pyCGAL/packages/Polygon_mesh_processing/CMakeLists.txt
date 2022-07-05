set(module_classes Polygon_soup)

set(additional_elements
    border
    clip
    connected_components
    corefine
    extrude_mesh
    helpers
    orient_polygon_soup
    polygon_soup_conversions
    remesh
    repair
    repair_polygon_soup
    split
    stitch_borders
    surface_intersection
    triangulate_faces
)

pycgal_add_core_module(
  Polygon_mesh_processing module_classes additional_elements
)
