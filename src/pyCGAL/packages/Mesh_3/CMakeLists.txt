set(module_classes # Mesh_triangulation_3
    Mesh_complex_3_in_triangulation_3 Mesh_criteria_3
    Polyhedral_complex_mesh_domain_3 Polyhedral_mesh_domain_with_features_3
    Polyhedron_3
)

set(module_elements make_mesh_3)

pycgal_add_core_module(Mesh_3 module_classes module_elements)
