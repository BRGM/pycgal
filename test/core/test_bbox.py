from pycgal.core import Bbox_3
from pycgal.core import do_overlap


def test_bbox():
    box = Bbox_3(0, 0, 0, 1, 1, 1)
    assert do_overlap(box, box)
    box += Bbox_3(1, 1, 1, 2, 2, 2)
    assert box.xmin() == 0
    assert box.ymin() == 0
    assert box.zmin() == 0
    assert box.xmax() == 2
    assert box.ymax() == 2
    assert box.zmax() == 2
