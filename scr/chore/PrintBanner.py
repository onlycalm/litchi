import sys
import os
import json
import PrnTee

InputParamNum = 3

Logo = """
██████╗ ██████╗ ██████╗ ██████╗  ██████╗ ████████╗
██╔═══██╗██╔══██╗██╔══██╗██╔══██╗██╔═══██╗╚══██╔══╝
██║   ██║██████╔╝██║  ██║██████╔╝██║   ██║   ██║
██║   ██║██╔══██╗██║  ██║██╔══██╗██║   ██║   ██║
╚██████╔╝██║  ██║██████╔╝██████╔╝╚██████╔╝   ██║
 ╚═════╝ ╚═╝  ╚═╝╚═════╝ ╚═════╝  ╚═════╝    ╚═╝
"""

ClrRed = "\033[31m"
ClrYlw = "\033[33m"
ClrGrn = "\033[32m"
ClrRst = "\033[0m"

def PrintBnr(ErrCnt, WrnCnt):
    if ErrCnt > 0:
        Clr = ClrRed
    elif WrnCnt > 0:
        Clr = ClrYlw
    else:
        Clr = ClrGrn

    print(f"{Clr}{Logo}{ClrRst}")

def main():
    if len(sys.argv) == InputParamNum:
        AnlsBldLogJsonPth = sys.argv[1]
        BldLogPth = sys.argv[2]

        BldLog = open(BldLogPth, "a")
        sys.stdout = PrnTee.Tee(sys.stdout, BldLog)

        try:
            AnlsBldLogJson = open(AnlsBldLogJsonPth, 'r', encoding = 'utf-8')
            AnlsBldLogDat = json.load(AnlsBldLogJson)
            PrintBnr(AnlsBldLogDat["errors"], AnlsBldLogDat["warnings"])
        except FileNotFoundError:
            print(f"Error: {AnlsBldLogJsonPth} does not exist.")
        except json.JSONDecodeError:
            print(f"Error: {AnlsBldLogJsonPth} format error.")
    else:
        print(f"Error: parameter input error! The number of parameters passed "
              f"in is {len(sys.argv)}, but the expected number is {InputParamNum}.")

if __name__ == "__main__":
    main()
