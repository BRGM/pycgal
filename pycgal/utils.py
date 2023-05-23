import numpy as np
import vtkwriters as vtkw


def as_array(seq):
    """Converts a sequence of point like object into a numpy array."""
    return np.array([np.array(a, copy=False) for a in seq])


def _filter_boolean_tables(d):
    if d is not None:
        return {
            name: a if a.dtype != bool else a.astype(np.int8) for name, a in d.items()
        }


def surface_mesh_to_vtp(
    mesh, filename, export_vertices_properties=True, export_faces_properties=True
):
    vertices, faces = mesh.as_lists()
    pointdata = (
        mesh.collect_vertices_properties() if export_vertices_properties else None
    )
    celldata = mesh.collect_faces_properties() if export_faces_properties else None
    vtkw.write_vtp(
        vtkw.vtp_doc(
            vertices,
            faces,
            pointdata=_filter_boolean_tables(pointdata),
            celldata=_filter_boolean_tables(celldata),
        ),
        filename,
    )


def surface_mesh_edges_to_vtu(
    mesh,
    filename,
    export_vertices_properties=True,
    export_edges_properties=True,
    export_vertices_id=False,
    export_edges_id=False,
):
    vertices, edges = mesh.edges_as_lists()
    pointdata = mesh.collect_vertices_properties() if export_vertices_properties else {}
    if export_vertices_id:
        assert "vertex_id" not in pointdata
        pointdata["vertex_id"] = mesh.vertices().as_array()
    celldata = mesh.collect_edges_properties() if export_edges_properties else {}
    if export_vertices_id:
        assert "edges_id" not in celldata
        celldata["edges_id"] = mesh.edges().as_array()
        for e, f in zip(mesh.edges(), celldata["edges_id"]):
            assert e.idx() == f
    vtkw.write_vtu(
        vtkw.vtu_doc(
            vertices,
            edges,
            pointdata=_filter_boolean_tables(pointdata),
            celldata=_filter_boolean_tables(celldata),
        ),
        filename,
    )


def c3t3_to_vtu(
    c3t3,
    basename,
    *,
    with_corner_index=False,
    with_curve_index=False,
    with_facet_index=False,
    with_subdomain_index=False,
    with_connected_components=False,
):
    vertices, corners, edges, facets, tets, *indices = c3t3.as_arrays(
        return_corner_index=with_corner_index,
        return_curve_index=with_curve_index,
        return_facet_index=with_facet_index,
        return_subdomain_index=with_subdomain_index,
    )
    fetched_indices = []

    def fetch_indices(available):
        data = {}
        if available:
            data = {"index": indices[len(fetched_indices)]}
            fetched_indices.append(indices[len(fetched_indices)])
        return data

    def point_indices(available):
        return {"pointdata": fetch_indices(available)}

    def cell_indices(available):
        return {"celldata": fetch_indices(available)}

    if len(corners) > 0:
        vtkw.write_vtu(
            vtkw.points_as_vtu_doc(
                vertices[corners], **point_indices(with_corner_index)
            ),
            f"{basename}-corners",
        )
    if len(edges) > 0:
        kept, clean = np.unique(edges, return_inverse=True)
        clean.shape = -1, 2
        vtkw.write_vtu(
            vtkw.vtu_doc(vertices[kept], clean, **cell_indices(with_curve_index)),
            f"{basename}-edges",
        )
    if len(facets) > 0:
        kept, clean = np.unique(facets, return_inverse=True)
        clean.shape = -1, 3
        vtkw.write_vtu(
            vtkw.vtu_doc(vertices[kept], clean, **cell_indices(with_facet_index)),
            f"{basename}-facets",
        )
    celldata = fetch_indices(with_subdomain_index)
    if with_connected_components:
        nc, component = c3t3.connected_components()
        # print(f"found {nc} connected components")
        celldata["component"] = component
    vtkw.write_vtu(
        vtkw.vtu_doc(vertices, tets, celldata=celldata),
        f"{basename}-tets",
    )
