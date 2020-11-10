set(module_classes)

set(additional_elements corefine isotropic_remeshing clip helpers
                        stitch_borders triangulate_faces
)

pycgal_add_core_module(
  Polygon_mesh_processing module_classes additional_elements
)
