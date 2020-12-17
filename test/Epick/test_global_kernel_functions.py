def test_gkf_approwimate_angle():
    from pycgal.Epick import Point_3
    from pycgal.Epick import approximate_angle

    A = Point_3(0, 0, 0)
    B = Point_3(1, 0, 0)
    C = Point_3(1, 1, 0)
    D = Point_3(1, -1, 0)
    E = Point_3(2, 0, 0)
    print(approximate_angle(A, B, C))
    print(approximate_angle(A, B, D))
    print(approximate_angle(A, B, E))
    print(approximate_angle(C, A, D))
