set(module_classes)

set(additional_elements
    border
    clip
    connected_components
    corefine
    helpers
    remesh
    repair
    split
    stitch_borders
    surface_intersection
    triangulate_faces
)

pycgal_add_core_module(
  Polygon_mesh_processing module_classes additional_elements
)
