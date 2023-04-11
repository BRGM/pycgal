import numpy as np
from pycgal.Surface_mesh import Surface_mesh
from pycgal.Surface_soup import Surface_soup
from pycgal.utils import surface_mesh_to_vtp


def test_surface_soup():

    tm1 = Surface_mesh(
        [
            (-0.5, 0.0, -0.5),
            (0.5, 0.0, -0.5),
            (0.0, 0.0, 0.5),
        ],
        [[0, 1, 2]],
    )
    tm2 = Surface_mesh(
        [
            (0.0, -0.5, -0.5),
            (0.0, 0.5, -0.5),
            (0.0, 0.0, 0.5),
        ],
        [[0, 1, 2]],
    )
    soup = Surface_soup([tm1, tm2])
    # FIXME: tm1 and tm2 should be moved into soup
    # assert tm1.number_of_vertices()==0
    # assert tm2.number_of_vertices()==0
    for i, mesh in enumerate(soup.meshes):
        print(
            "mesh",
            i,
            "with",
            mesh.number_of_vertices(),
            "vertices and",
            mesh.number_of_faces(),
            "faces",
        )
        surface_mesh_to_vtp(mesh, f"mesh_{i:04d}")
    intersections = soup.collect_intersections()
    print(intersections)
    assert len(intersections) == 1
