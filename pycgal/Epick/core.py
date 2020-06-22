import numpy as np

from .Point_3 import Point_3
from .Vector_3 import Vector_3


def as_array(seq):
    """Converts a sequence of point like object into a numpy array."""
    return np.array([np.array(a, copy=False) for a in seq])
