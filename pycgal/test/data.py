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
triangles = np.array([[0, 3, 4], [1, 0, 5], [3, 1, 6], [3, 2, 7],])
