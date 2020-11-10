import numpy as np


def as_array(seq):
    """Converts a sequence of point like object into a numpy array."""
    return np.array([np.array(a, copy=False) for a in seq])
