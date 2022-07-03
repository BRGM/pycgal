import numpy as np
from pycgal.Epick import Point_2, Vector_of_Point_2
from pycgal.Epick import Point_3, Vector_of_Point_3


def test_vector_of_point_2():

    v = Vector_of_Point_2()
    assert v.empty()
    assert v.is_empty
    v.push_back(Point_2(0, 1))
    v.push_back(Point_2(0, 2))
    print(v[0])
    v[0] = Point_2(10, 10)
    print(v[0])
    v.insert(1, Point_2(11, 11))
    v.erase(0)
    print(v)
    print(f"size: {v.size()} capacity: {v.capacity()}")
    v.shrink_to_fit()
    print(f"size: {v.size()} capacity: {v.capacity()}")
    v.reserve(100)
    print(f"size: {v.size()} capacity: {v.capacity()}")
    a = np.array(v, copy=False)
    assert a.shape == (v.size(), 2)


def test_vector_of_point_3():

    v = Vector_of_Point_3()
    assert v.empty()
    assert v.is_empty
    v.push_back(Point_3(0, 0, 1))
    v.push_back(Point_3(0, 0, 2))
    print(v)
    v.push_back(Point_3(0, 0, 3))
    v.push_back(Point_3(0, 0, 4))
    v.push_back(Point_3(0, 0, 5))
    v.push_back(Point_3(0, 0, 6))
    print(v)
    print(v[0])
    v[0] = Point_3(10, 10, 10)
    print(v[0])
    v.insert(1, Point_3(11, 11, 11))
    v.erase(5)
    a = np.array(v, copy=False)
    assert a.shape == (v.size(), 3)
    for P in v:
        print(P)
    print(f"size: {v.size()} capacity: {v.capacity()}")
    v.shrink_to_fit()
    print(f"size: {v.size()} capacity: {v.capacity()}")
    v.reserve(100)
    print(f"size: {v.size()} capacity: {v.capacity()}")
    n = v.size()
    l = list(v)
    v.clear()
    assert v.is_empty
    assert len(l) == n
    for P in l:
        print(P)
    a = np.arange(6, dtype="i")
    a.shape = -1, 3
    v = Vector_of_Point_3(a)
    print(v)
    v.extend(a)
    print(v)
