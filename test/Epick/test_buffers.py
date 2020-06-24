import pytest

import numpy as np

from pycgal.Epick import Point_3, Vector_3, as_array


@pytest.mark.parametrize("PointLike", [Point_3, Vector_3])
def test_buffers(PointLike):
    A = PointLike(0, 0, 0)
    B = PointLike(1, 1, 0)
    assert np.array(A).shape == (3,)
    assert as_array([A, B]).shape == (2, 3)


if __name__ == "__main__":
    test_buffers(Point_3)
    test_buffers(Vector_3)
