set(module_classes
    Constrained_Delaunay_triangulation_2
    Delaunay_mesh_vertex_base_2
    Delaunay_mesh_face_base_2
    Triangulation_vertex_base_with_info_2
    Triangulation_data_structure_2
    Delaunay_mesh_adaptative_size_criteria_2
    Delaunay_mesher_2
    Delaunay_terrain
)

set(module_elements Mesh_optimization_return_code lloyd_optimize_mesh_2)

pycgal_add_core_module(Mesh_2 module_classes module_elements)
