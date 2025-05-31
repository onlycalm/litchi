import sys
import json
import PrnTee
from datetime import datetime

InputParamNum = 4

def RecBegTm(RecTmPth):
    try:
        BegTm = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
        RecTmDat = {}
        RecTmDat["BegTm"] = BegTm
        RecTmFile = open(RecTmPth, 'w', encoding = 'utf-8')
        json.dump(RecTmDat, RecTmFile, ensure_ascii = False, indent = 4)
    except FileNotFoundError:
        print(f"Error: {RecTmPth} does not exist.")
    except Exception as e:
        print(f"Error: {e}")

def RecEndTm(RecTmPth):
    try:
        RecTmFile = open(RecTmPth, 'r', encoding = 'utf-8')
        RecTmDat = json.load(RecTmFile)
        EndTm = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
        RecTmDat["EndTm"] = EndTm
        RecTmFile = open(RecTmPth, 'w', encoding = 'utf-8')
        json.dump(RecTmDat, RecTmFile, ensure_ascii = False, indent = 4)
    except FileNotFoundError:
        print(f"Error: {RecTmPth} does not exist.")
    except Exception as e:
        print(f"Error: {e}")

def CalcDifTm(RecTmPth):
    try:
        RecTmFile = open(RecTmPth, 'r', encoding = 'utf-8')
        RecTmDat = json.load(RecTmFile)
        BegTm = datetime.strptime(RecTmDat["BegTm"], "%Y-%m-%d %H:%M:%S.%f")
        EndTm = datetime.strptime(RecTmDat["EndTm"], "%Y-%m-%d %H:%M:%S.%f")
        DifTm = EndTm - BegTm
        RecTmDat["DifTm"] = str(DifTm)
        RecTmFile = open(RecTmPth, 'w', encoding = 'utf-8')
        json.dump(RecTmDat, RecTmFile, ensure_ascii = False, indent = 4)
        print(f"Compilation takes time: {DifTm}")
    except FileNotFoundError:
        print(f"Error: {RecTmPth} does not exist.")
    except Exception as e:
        print(f"Error: {e}")

def main():
    if len(sys.argv) == InputParamNum:
        Stg = sys.argv[1]
        RecTmPth = sys.argv[2]
        BldLogPth = sys.argv[3]

        BldLog = open(BldLogPth, "a")
        sys.stdout = PrnTee.Tee(sys.stdout, BldLog)

        if Stg == "Begin":
            RecBegTm(RecTmPth)
        elif Stg == "End":
            RecEndTm(RecTmPth)
            CalcDifTm(RecTmPth)
        else:
            pass

if __name__ == "__main__":
    main()
