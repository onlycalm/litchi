import sys
import os
import re
import json
import PrnTee

InputParamNum = 4

def AnlsBldLog(BldLogPth):
    ErrCnt = 0
    WrnCnt = 0

    LstErrPatn = [
        r'error:', r'Error:', r'fatal error:', r'错误:', r'error C\d+'
    ]
    LstWrnPatn = [
        r'warning:', r'Warning:', r'警告:', r'warning C\d+'
    ]

    ErrPatn = re.compile('|'.join(f'({pat})' for pat in LstErrPatn), re.IGNORECASE)
    WrnPatn = re.compile('|'.join(f'({pat})' for pat in LstWrnPatn), re.IGNORECASE)

    try:
        BldLog = open(BldLogPth, 'r', encoding = 'utf-8')

        for line in BldLog:
            if ErrPatn.search(line):
                ErrCnt += 1

            if WrnPatn.search(line):
                WrnCnt += 1
    except FileNotFoundError:
        print(f"Error: {BldLogPth} does not exist.")
    except Exception as e:
        print(f"Error: {e}")

    return ErrCnt, WrnCnt

def WrJson(AnlsBldLogDat, AnlsBldLogJsonPth):
    try:
        AnlsBldLogJson = open(AnlsBldLogJsonPth, 'w', encoding='utf-8')
        json.dump(AnlsBldLogDat, AnlsBldLogJson, ensure_ascii = False, indent = 4)
    except Exception as e:
        print(f"Error: Writing json file. {e}")

def main():
    if len(sys.argv) == InputParamNum:
        VerTyp = sys.argv[1]
        BldLogPth = sys.argv[2]
        AnlsBldLogJsonPth = sys.argv[3]

        BldLog = open(BldLogPth, "a")
        sys.stdout = PrnTee.Tee(sys.stdout, BldLog)

        ErrCnt, WrnCnt = AnlsBldLog(BldLogPth)
        AnlsBldLogDat = {"errors": ErrCnt, "warnings": WrnCnt}
        WrJson(AnlsBldLogDat, AnlsBldLogJsonPth)

        print(f"=== {VerTyp} build report ===")
        print(f"Errors: {ErrCnt}")
        print(f"Warnings: {WrnCnt}")

if __name__ == "__main__":
    main()
