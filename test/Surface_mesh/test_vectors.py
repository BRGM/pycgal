import pytest
import numpy as np

from pycgal.Surface_mesh import Vertices, Edges, Vertex_index


def test_surface_mesh_vertices():
    with pytest.raises(TypeError):
        # Creation from numpy array is deactivated to avoid mixing indices
        v = Vertices(np.arange(5))
    with pytest.raises(TypeError):
        # Creation from iterables is deactivated to avoid mixing indices
        v = Vertices(list(range(5)))
    # WARNING
    # You can still explicitely create a list from integers
    # but it's likely that you will run into problems
    # some indices have a special interpretation
    # that does dot correspond to their value
    v = Vertices.create(np.arange(5))
    v.extend(list(range(5)))
    print(list(v))
    v.remove_duplicates()
    print(list(v))
    v.extend(np.arange(5))
    print(list(v))
    v.remove_duplicates()
    print(list(v))
    print(v[0])
    with pytest.raises(TypeError):
        v[0] = 1
    with pytest.raises(TypeError):
        v.append(1)
    # Same as above... be cautious
    v[0] = Vertex_index.create(1)
    e = Edges.create(np.arange(5))
    print(e[0])
    with pytest.raises(TypeError):
        Edges(v)
    with pytest.raises(TypeError):
        v[0] = e[0]
