set(module_classes)

set(additional_elements
    border_halfedges
    clip
    connected_components
    corefine
    helpers
    isotropic_remeshing
    split
    stitch_borders
    triangulate_faces
)

pycgal_add_core_module(
  Polygon_mesh_processing module_classes additional_elements
)
