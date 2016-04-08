from distutils.core import setup
from Cython.Build import cythonize
import numpy

setup(name="parjulia",
      ext_modules=cythonize("parjulia.pyx"),
     include_dirs=[numpy.get_include()])