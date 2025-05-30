# [Install]

# pip 설치 
# curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
# python get-pip.py
# Scripts 폴더에 설치됨.

# pip install xlrd
# pip install openpyxl
# pip install pandas

# [Run]
# 전체 xlsx 파일 변환
# python convert.py
# 특정 파일.xlsx 파일 변환
# python convert.py 특정 파일.xlsx 

import pandas as pd
import os
import sys

path = '.'
files = os.listdir(path)
 
# 특정 포맷 파일 리스트 담기.
file_list_py = [file for file in files if file.endswith(".xlsx")]

def convertToCsv(excel_src):
    xlsx = pd.read_excel(excel_src)
    csv = excel_src.replace(".xlsx",".csv")
    xlsx.to_csv(csv, index = None, encoding = 'utf-8-sig') 
	# columns = ['test', 'test_', 'test__'] : 특정 컬럼만 출력
	
    print(csv + " converted")

if len(sys.argv) == 2 : # 특정 파일.xlsx 인자가 있는 경우..
    convertToCsv(sys.argv[1])
else : # 인자가 업는 경우 전체 xlsx 파일 변환
    for f in file_list_py:
        convertToCsv(f)