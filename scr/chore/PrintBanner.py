import sys
import os
import json

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

def PrintBnr(ErrCnt, WrnCnt, LogFilePth):
    if ErrCnt > 0:
        Clr = ClrRed
    elif WrnCnt > 0:
        Clr = ClrYlw
    else:
        Clr = ClrGrn

    BnrClr = f"{Clr}{Logo}{ClrRst}"
    print(BnrClr)

    try:
        with open(LogFilePth, "a") as file:
            file.write(BnrClr)
    except Exception as e:
        print(f"Error: Fail to write to file.", file=sys.stderr)

def main():
    if len(sys.argv) == InputParamNum:
        AnlsBldLogJsonPth = sys.argv[1]
        BldLogPth = sys.argv[2]

        try:
            AnlsBldLogJson = open(AnlsBldLogJsonPth, 'r', encoding = 'utf-8')
            AnlsBldLogDat = json.load(AnlsBldLogJson)
            PrintBnr(AnlsBldLogDat["errors"], AnlsBldLogDat["warnings"], BldLogPth)
        except FileNotFoundError:
            print(f"Error: {AnlsBldLogJsonPth} does not exist.")
        except json.JSONDecodeError:
            print(f"Error: {AnlsBldLogJsonPth} format error.")
    else:
        print(f"Error: parameter input error! The number of parameters passed "
              f"in is {len(sys.argv)}, but the expected number is {InputParamNum}.")

if __name__ == "__main__":
    main()
