from pycgal.core import Random, set_default_random


def test_set_default_random():
    random = Random(12345)
    for _ in range(3):
        print(random.get_double(0, 10))
    set_default_random(random)
