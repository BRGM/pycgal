def test_planes():
    from pycgal.Epick import Plane_3
    from pycgal.Epick import Point_3
    from pycgal.Epick import Vector_3

    P1 = Plane_3(Point_3(0, 0, 0), Point_3(1, 0, 0), Point_3(0, 1, 0))
    P2 = Plane_3(Point_3(0, 0, 0), Vector_3(0, 0, 1))
    assert P1 == P2
    P3 = Plane_3(Point_3(0, 0, 0), Vector_3(1, 0, 0))
    assert P1 != P3
