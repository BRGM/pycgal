import pytest

import numpy as np

# We need to import Point_3 to display them cf (*) below
from pycgal.Epick import Point_3
from pycgal.Surface_mesh import Surface_mesh, Edge_index, Edges


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
    print("Before:", a)
    prop.set(edges, np.arange(edges.size, dtype="d"))
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
    flag.set(True)
    assert np.all(flag.copy_as_array())
    flag.fill(False)
    assert not np.any(flag.copy_as_array())
    print("f:removed:", mesh.face_property("f:removed").copy_as_array())
    for k, f in enumerate(mesh.faces()):
        if k % 2 == 0:
            print("Remmove", f)
            mesh.remove_face(f)
    print("f:removed:", mesh.face_property("f:removed").copy_as_array())

    assert "some_property" in mesh.edge_properties()
    mesh.remove_property(prop)
    assert "some_property" not in mesh.edge_properties()
