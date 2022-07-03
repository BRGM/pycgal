import numpy as np
from pycgal.core import Vector_of_size_t


def test_vector_of_size_t():

    v = Vector_of_size_t()
    assert v.empty()
    assert v.is_empty
    v.push_back(1)
    v.push_back(2)
    print(v)
    a = np.array(v, copy=False)
    assert a.shape == (v.size(),)
    assert a[0] == v[0]
    a[0] = 2
    assert a[0] == v[0]
    assert v[0] == 2
    v = Vector_of_size_t(list(range(10)))
    for k, i in enumerate(v):
        assert k == i
    v.extend(list(range(10)))
    for k in range(10):
        assert v[k] == v[k + 10] == k
    a = np.arange(2, dtype="i")
    v = Vector_of_size_t(a)
    print(v)
    v.extend(a)
    print(v)
