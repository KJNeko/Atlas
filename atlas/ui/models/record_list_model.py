from PySide6.QtCore import (Qt, QAbstractTableModel, QModelIndex)
from atlas.core.database.record.game.game import *

class RecordListModel(QAbstractTableModel):

    m_records = [] #dict

    def __init__(self, parent=None):
        super(RecordListModel, self).__init__(parent)
        #all data is loaded through set_records

    def addRecord(self, records: list[Game]):
        self.m_records = records

    def rowCount(self, index=QModelIndex()):
        """ Returns the number of rows the model holds. """
        return len(self.m_records)

    def columnCount(self, index=QModelIndex()):
    #    """ Returns the number of columns the model holds. """
        return 1

    def data(self, index, role=Qt.DisplayRole):
        """ Depending on the index and role given, return data. If not 
            returning data, return None (PySide equivalent of QT's 
            "invalid QVariant").
        """
        if not index.isValid():
            return None

        if not 0 <= index.row() < len(self.m_records):
            return None

        if role == Qt.DisplayRole:
            title = self.m_records[index.row()]["title"]           
            return title       
        return None