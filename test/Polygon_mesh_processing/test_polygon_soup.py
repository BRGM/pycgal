import numpy as np
from pycgal.Polygon_mesh_processing import Polygon_soup
from pycgal.Polygon_mesh_processing import repair_polygon_soup


def test_polygon_soup():

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
            ]
        )
    )
    polygons = soup.polygons
    polygons.push_back([0, 1, 2])
    polygons.push_back([3, 4, 5])
    polygons.extend([[4, 5, 7, 6], [6, 7, 8]])
    polygons.extend([[7, 6, 8], soup.Polygon([8, 6, 7])])
    print(f"We have {len(soup.points)} points and {len(soup.polygons)} polygons.")
    print(soup.points)
    print(soup.polygons)


def test_repair_polygon_soup():

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
