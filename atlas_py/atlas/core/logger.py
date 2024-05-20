import os
from .utilities.multicast import *
from .utilities.console import*

class logger(object):
    #This class allows for outputing to a console window or via UDP over port 5555. This will be enabled initally. 
    #We will have a setting to change this at a later time


    is_console = False #Output to console
    is_enabled = True #Disable output. For Terminal Program

    def init():
        os.system('cls')

    def info(_str):
        message = "info||" + str(_str)
        if logger.is_enabled:
            if logger.is_console:
                console.output(message)
            multicast.send(message)

    def error(_str):
        message = "error||" + str(_str)
        if logger.is_enabled:
            if logger.is_console:
                console.output(message)
            multicast.send(message)

    def debug(_str):
        message ="debug||" + str(_str)
        if logger.is_enabled:
            if logger.is_console:
                console.output(message)
            multicast.send(message)

    def warn(_str):
        message = "warn||" + str(_str)
        if logger.is_enabled:
            if logger.is_console:
                console.output(message)
            multicast.send(message)


