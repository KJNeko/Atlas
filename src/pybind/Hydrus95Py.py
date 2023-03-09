import sys
import os
from ctypes import cdll

dirname = os.path.dirname(__file__)
filename = os.path.join(dirname, "libH95.so")

lib = cdll.LoadLibrary(filename)

def init():
    lib.init()

if __name__ == "__main__":
    init()




