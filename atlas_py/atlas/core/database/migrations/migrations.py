import sqlite3
import os
from pandas import DataFrame
from .tables import *
from ...logger import *


class migrations(object):

    def run_up(database_path: str) -> str:
        logger.info("Running database migrations")
       
        con = sqlite3.connect(database_path)
        cur = con.cursor()
        __tables = tables.table_list()
        #print(__tables)
        for table in __tables:
            logger.info(f'Running migration for {table}')
            cur.execute(__tables[table][0])
            con.commit()
        con.close()