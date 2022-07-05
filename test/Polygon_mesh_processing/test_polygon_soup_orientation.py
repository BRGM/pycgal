import numpy as np
from pycgal.Polygon_mesh_processing import Polygon_soup
from pycgal.Polygon_mesh_processing import repair_polygon_soup
from pycgal.Polygon_mesh_processing import orient_polygon_soup
from pycgal.Polygon_mesh_processing import polygon_soup_to_polygon_mesh


def test_polygon_soup_orientation():

    soup = Polygon_soup()
    soup.points.extend(
        np.array(
            [
                (0, 0, 0),
                (1, 0, 0),
                (1, 1, 0),
                (0, 1, 0),
                (1, 0, 0),
                (1, 1, 0),
                (2, 0, 0),
                (2, 1, 0),
                (2, 1.5, 0),
                (10, 10, 10),  # isolated vertex
            ]
        )
    )
    soup.polygons.extend([[0, 1, 2], [3, 4, 5], [4, 5, 7, 6], [6, 7, 8], [7, 6, 8]])
    print(
        f"Before reparation soup has {len(soup.points)} points and {len(soup.polygons)} polygons."
    )
    repair_polygon_soup(soup)
    print(
        f"After reparation soup has {len(soup.points)} points and {len(soup.polygons)} polygons."
    )
    print(soup.points)
    print(soup.polygons)
    orient_polygon_soup(soup)
    mesh = polygon_soup_to_polygon_mesh(soup)
    print(
        f"Result mesh has {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces."
    )
    assert soup.points.size() == mesh.number_of_vertices()
    assert soup.polygons.size() == mesh.number_of_faces()
