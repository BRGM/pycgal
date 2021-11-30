from collections import namedtuple

import numpy as np
import pytest

SimpleMesh = namedtuple(
    "SimpleMesh", ("all_vertices", "square", "square_vertices", "triangles")
)
MeshInfo = namedtuple("MeshInfo", ("vertices", "faces"))


@pytest.fixture
def simple_mesh():
    """
    A simple mesh...
    """
    all_vertices = np.array(
        [
            (-1, -1, 0),
            (1, -1, 0),
            (1, 1, 0),
            (-1, 1, 0),
            (-2, 0, 0),
            (0, -2, 0),
            (2, 0, 0),
            (0, 2, 0),
        ],
        dtype=np.double,
    )
    square = np.array([[0, 1, 2, 3]])
    square_vertices = all_vertices[np.unique(square)]
    triangles = np.array(
        [
            [5, 1, 0],
            [6, 2, 1],
            [7, 3, 2],
            [4, 0, 3],
        ]
    )
    return SimpleMesh(
        all_vertices=all_vertices,
        square=square,
        square_vertices=square_vertices,
        triangles=triangles,
    )


@pytest.fixture
def squares_2x2():
    vertices = np.array(
        [
            (-1, -1, 0),
            (-1, 0, 0),
            (-1, 1, 0),
            (0, -1, 0),
            (0, 0, 0),
            (0, 1, 0),
            (1, -1, 0),
            (1, 0, 0),
            (1, 1, 0),
        ],
        dtype=np.double,
    )
    faces = np.array(
        [
            [0, 1, 3, 2],
            [1, 2, 4, 3],
            [3, 4, 7, 6],
            [4, 5, 8, 7],
        ]
    )
    return MeshInfo(vertices, faces)
