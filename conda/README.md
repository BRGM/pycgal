Conda recipes are not compliant with Win64.
In conda-forge CGAL relies on mpifr which may work or may not.

To build an environment where to build pycgal
(it is assumed that CGAL repository has been cloned in $CGAL_DIR):
```
conda env create -f pycgal.yml
```

You may replace conda with mamba for faster depencies resolution.
