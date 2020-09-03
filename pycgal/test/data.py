import numpy as np

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
triangles = np.array([[5, 1, 0], [6, 2, 1], [7, 3, 2], [4, 0, 3],])
