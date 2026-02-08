from pycgal.Surface_mesh import Surface_mesh


# checks that pycgal.Epick is available through pycgal.Surface_mesh
# it is mandatory to display pycgal.Epick.Point_3 objects used as vertices
def test_diplay_mesh_vertices(simple_mesh):

    mesh = Surface_mesh(simple_mesh.square_vertices, simple_mesh.square)
    for v in mesh.vertices():
        print(f"{v} at {mesh.point(v)}")
