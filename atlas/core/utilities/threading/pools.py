import sys, os, csv
from atlas.core.settings import *
from atlas.core.logger import *
from atlas.core.logger import *
from atlas.core.utilities.threading import *
from atlas.ui.importer.ui_BatchImportDialog import Ui_BatchImportDialog
from pandas import DataFrame
import re
from difflib import SequenceMatcher

from PySide6.QtCore import QTimer, QRunnable, Slot, Signal, QObject, QThreadPool
from PySide6.QtWidgets import QTableWidgetItem, QComboBox

class ThreadPools(object):
    image_loaders = QThreadPool()
    image_importers = QThreadPool()
    importers = QThreadPool()
    pre_importers = QThreadPool()
    #If this is not called, the program will default to what it thinks is best
    def init():
        ThreadPools.image_loaders.setMaxThreadCount(20)
        ThreadPools.image_importers.setMaxThreadCount(20)
        ThreadPools.importers.setMaxThreadCount(20)
        ThreadPools.pre_importers.setMaxThreadCount(50)
