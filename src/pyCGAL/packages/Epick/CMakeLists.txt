set(module_classes
    Point_2
    Vector_2
    Segment_2
    Point_3
    Vector_of_Point_2
    Vector_of_Point_3
    Vector_3
    Plane_3
    Segment_3
    Line_3
    Ray_3
)

set(additional_elements global_kernel_functions)

pycgal_add_core_module(
  Epick module_classes additional_elements LinearGeometryKernel
)
