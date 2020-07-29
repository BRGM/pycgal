# Define module elements here You will have to speficy a cpp file for each of
# the elements list
set(module_elements clip corefine isotropic_remeshing triangulate_faces
                    stitch_borders utilities
)

# string(JOIN " " module_elements ${module_elements})
pycgal_add_module(PMP ${module_elements})
