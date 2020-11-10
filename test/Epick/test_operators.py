def test_operators():
    from pycgal.Epick import Point_3, Vector_3

    A = Point_3(0, 0, 0)
    B = Point_3(1, 1, 0)
    V = B - A
    assert V.squared_length() == 2
    assert V == Vector_3(1, 1, 0)
    assert V * Vector_3(0, 0, 1) == 0
