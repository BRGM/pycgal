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
