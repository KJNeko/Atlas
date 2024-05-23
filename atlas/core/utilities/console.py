from colorama import Fore, Style
from datetime import datetime

class console(object):
    def output(str):
        str = str.split('||')
        now = datetime.now().strftime("%d/%m/%Y %H:%M:%S")
        if len(str) > 1:
            if 'info' in str[0]:
                print(Fore.WHITE + Style.NORMAL + now + ": " + str[1])
            if 'debug' in str[0]:
                print(Fore.LIGHTBLUE_EX + Style.DIM + now + ": " + str[1])
            if 'warn' in str[0]:
                print(Fore.YELLOW + Style.NORMAL + now + ": " + str[1])
            if 'error' in str[0]:
                print(Fore.RED + Style.NORMAL + now + ": " + str[1])
            #dp something
        else:
            print(Fore.WHITE + Style.NORMAL + now + ": " + str[0])
