import numpy as np
import pytest

from pycgal.Surface_mesh import Edges
from pycgal.Surface_mesh import Surface_mesh


def test_properties(simple_mesh):
    def list_properties(mesh):
        for loc in ["vertex", "halfedge", "edge", "face"]:
            print(getattr(mesh, loc + "_properties")())

    mesh = Surface_mesh(
        simple_mesh.all_vertices, [simple_mesh.square, simple_mesh.triangles]
    )
    list_properties(mesh)

    prop, created = mesh.add_edge_property("some_property", dtype="d")
    assert created
    for e in mesh.edges():
        print(f"edge {e} has value: {prop[e]}")
    a = np.array(prop, copy=False)
    print(a)
    a[::2] = 1
    for e in mesh.edges():
        print(f"edge {e} has value: {prop[e]}")
    for k, e in enumerate(mesh.edges()):
        if k % 3 == 0:
            prop[e] = prop[e] + 0.5
    print(a)
    edges = Edges(mesh.edges())
    with pytest.raises(RuntimeError):
        prop.is_set(edges[0])
    print("Before:", a)
    prop.set(edges, np.arange(edges.size, dtype="d"))
    prop.set([e for e in edges], np.arange(edges.size, dtype="d"))
    print("After:", a)
    fconnectivity = mesh.face_property("f:connectivity")
    print(
        "some_property type:", prop.property_type()
    )  # mangled named... TODO:replace mangled names using detail::TName
    assert (
        fconnectivity is None
    )  # connectivity type does not belong to variant alternatives yet

    # retrieve previously stored property
    prop = mesh.edge_property("some_property")
    print(
        "some_property type:", prop.property_type()
    )  # mangled named... TODO:replace mangled names using detail::TName
    flag, created = mesh.add_edge_property("e:flag", dtype="b")
    assert created
    for e in mesh.edges():
        assert flag[e] == flag.is_set(e)
        assert not flag.is_set(e)  # booleans default to false
    # a = np.array(flag) : this will throw a C++ exception (TODO: convert the exception to python)
    # because of underlyning storage
    flag_copy = flag.copy_as_array()
    assert np.all(np.logical_not(flag_copy))
    print(flag_copy)
    # TODO: iterate correctly over boolean array
    edges = Edges(mesh.edges())
    flag.set(edges, True)
    flag_copy = flag.copy_as_array()
    assert np.all(flag_copy)
    print(flag_copy)
    one_of_two = Edges()
    for e in list(mesh.edges())[::2]:
        one_of_two.append(e)
    print("One out of two:", list(one_of_two))
    flag.set(one_of_two, False)
    print(flag.copy_as_array())
    flag.flip()
    for e in edges:
        assert flag[e] == flag.is_set(e)
        if e in one_of_two:
            assert flag.is_set(e)
        else:
            assert not flag.is_set(e)
    flag.set(True)
    assert np.all(flag.copy_as_array())
    flag.fill(False)
    assert not np.any(flag.copy_as_array())
    flag.flip()
    assert np.all(flag.copy_as_array())
    print("f:removed:", mesh.face_property("f:removed").copy_as_array())
    for k, f in enumerate(mesh.faces()):
        if k % 2 == 0:
            print("Remmove", f)
            mesh.remove_face(f)
    print("f:removed:", mesh.face_property("f:removed").copy_as_array())

    assert "some_property" in mesh.edge_properties()
    mesh.remove_property(prop)
    assert "some_property" not in mesh.edge_properties()

    prop, created = mesh.add_vertex_property("addresses", dtype="p")
    assert created
    print(
        "addresses type:", prop.property_type()
    )  # mangled named... TODO:replace mangled names using detail::TName
    n = mesh.number_of_vertices()
    some_integers = list(range(n))
    for v, i in zip(mesh.vertices(), some_integers):
        prop[v] = id(i)
    for v, i in zip(mesh.vertices(), some_integers):
        assert prop[v] == id(i)

    from itertools import cycle

    message = "Hello world!"
    prop, created = mesh.add_edge_property("characters", dtype="c")
    assert created
    print(
        "characters type:", prop.property_type()
    )  # mangled named... TODO:replace mangled names using detail::TName

    for e, c in zip(mesh.edges(), cycle(message)):
        prop[e] = c
    retrieved = "".join(prop[e] for e in mesh.edges())
    print(retrieved)
    n = min(len(message), len(retrieved))
    assert message[:n] == retrieved[:n]


def test_logical_properties(simple_mesh):
    mesh = Surface_mesh(
        simple_mesh.all_vertices, [simple_mesh.square, simple_mesh.triangles]
    )
    ne = mesh.number_of_edges()

    def generate_edge_selection():
        selection = np.unique(np.random.randint(0, ne, ne // 2))
        some_edges = []
        for k, e in enumerate(mesh.edges()):
            if k in selection:
                some_edges.append(e)
        return selection, some_edges

    f1, created = mesh.add_edge_property("e:f1", dtype="b", value=True)
    assert np.all(f1.copy_as_array())
    assert created
    f2, created = mesh.add_edge_property("e:f2", dtype="b")
    assert created
    assert not np.any(f2.copy_as_array())
    ne = mesh.number_of_edges()
    selection, some_edges = generate_edge_selection()
    f2.set(some_edges, True)
    check2 = np.zeros(ne, dtype="b")
    check2[selection] = True
    assert np.all(f2.copy_as_array() == check2)
    f2.flip()
    assert np.all(f2.copy_as_array() == np.logical_not(check2))
    selection, some_edges = generate_edge_selection()

    def reset_f1():
        f1.set(True)
        f1.set(some_edges, False)

    reset_f1()
    check1 = np.ones(ne, dtype="b")
    check1[selection] = False
    assert np.all(f1.copy_as_array() == check1)
    f2.flip()
    assert np.all(f2.copy_as_array() == check2)
    f1.logical_or(f2)
    assert np.all(f1.copy_as_array() == np.logical_or(check1, check2))
    reset_f1()
    f1.logical_and(f2)
    assert np.all(f1.copy_as_array() == np.logical_and(check1, check2))
    reset_f1()
    f1.logical_xor(f2)
    assert np.all(f1.copy_as_array() == np.logical_xor(check1, check2))


def test_property_selection(simple_mesh):
    mesh = Surface_mesh(
        simple_mesh.all_vertices, [simple_mesh.square, simple_mesh.triangles]
    )
    value, created = mesh.add_edge_property("e:value", dtype="i")
    assert created
    for k, e in enumerate(mesh.edges()):
        value[e] = k % 2
    selection = mesh.select(value, 1)
    assert all([value[e] == 1 for e in selection])
    assert len(selection) == mesh.number_of_edges() / 2
    n = len(selection)
    for k, e in enumerate(selection):
        value[e] = k % 2
    mesh.keep(selection, value, 1)
    assert all([value[e] == 1 for e in selection])
    assert len(selection) == n / 2
    value, created = mesh.add_vertex_property("v:value", dtype="i")
    assert created
    for k, v in enumerate(mesh.vertices()):
        value[v] = k % 2
    selection = mesh.select_edges(value, 0)
    print("All edges")
    for e in mesh.edges():
        va, vb = mesh.vertex(e, 0), mesh.vertex(e, 1)
        print(va, "<->", vb, ":", value[va], value[vb])
    print("Selection")
    for e in selection:
        print(mesh.vertex(e, 0), "<->", mesh.vertex(e, 1))


def test_properties_copy(triangle):

    mesh = Surface_mesh(triangle.vertices, triangle.faces)
    p, created = mesh.add_face_property("f:p", dtype="i", value=0)
    assert created
    mesh2 = Surface_mesh(mesh)
    p2 = mesh2.face_property("f:p")
    faces = mesh.faces()
    assert len(faces) == 1
    f0 = list(faces)[0]
    assert p[f0] == p2[f0]
    p[f0] = 1
    assert p[f0] != p2[f0]
