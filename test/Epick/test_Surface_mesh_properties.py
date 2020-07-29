import pytest

import numpy as np

# We need to import Point_3 to display them cf (*) below
from pycgal.Epick import Surface_mesh, Point_3
from pycgal.test.data import all_vertices, triangles, square


def test_properties():
    def list_properties(mesh):
        for loc in ["vertex", "halfedge", "edge", "face"]:
            print(getattr(mesh, loc + "_properties")())

    mesh = Surface_mesh(all_vertices, [square, triangles])
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
    # a = np.array(flag) : this will throw a C++ exception (TODO: convert this to python)
    # because of underlyning storage
    # TODO: iterate correctly over boolean array
    # TODO: add as_array() member to generate a copy

    assert "some_property" in mesh.edge_properties()
    mesh.remove_property(prop)
    assert "some_property" not in mesh.edge_properties()


if __name__ == "__main__":
    test_properties()
