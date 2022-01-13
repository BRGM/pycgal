def test_vertex_base():
    import pycgal.Mesh_2 as Mesh_2

    vb = Mesh_2.Triangulation_vertex_base_with_info_2()
    assert vb.info is None
    vb.info = []
    vb.info.append(1)
    assert vb.info[0] == 1


def test_face_base():
    import pycgal.Mesh_2 as Mesh_2

    fb = Mesh_2.Triangulation_face_base_with_info_2()
    assert fb.info is None
    fb.info = "hello"
    assert fb.info.upper() == "HELLO"
    fb.info += " world"
    assert fb.info == "hello world"
