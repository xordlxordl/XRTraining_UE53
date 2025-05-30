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

def check_column_exists(file_path, column_name):
    # 엑셀 파일 읽기
    df = pd.read_excel(file_path)

    # 특정 컬럼이 있는지 확인
    if column_name in df.columns:
        return True
    else:
        return False        
    
def convertToCsv(excel_src, select_col):
    
    if check_column_exists(excel_src, select_col) == False : 
        return
    
    xlsx = pd.read_excel(excel_src)
   
    old_column_name = select_col
    new_column_name = 'SourceString'
    
    # 특정 컬럼 이름 변경
    if old_column_name in xlsx.columns:
        xlsx.rename(columns={old_column_name: new_column_name}, inplace=True)
           
    # 추출할 컬럼 지정
    columns_to_extract = ['Key', 'SourceString']  # 원하는 컬럼 이름으로 변경

    # 지정한 컬럼만 추출
    xlsx_selected = xlsx[columns_to_extract]
              
    csv = excel_src.replace(".xlsx",select_col)
    csv = csv + ".csv" # excel_src.replace(".xlsx","_en.csv")
    xlsx_selected.to_csv(csv, index = None, encoding = 'utf-8-sig') 
	
    print(csv + " converted")

if len(sys.argv) == 3 : # 특정 파일.xlsx 인자와 언어 컬럼 인지가 있는 경우..
    convertToCsv(sys.argv[1], sys.argv[1])
if len(sys.argv) == 2 : # 특정 파일.xlsx 인자가 있는 경우..
    convertToCsv(sys.argv[1], 'EN')
else : # 인자가 업는 경우 전체 xlsx 파일 변환
    for f in file_list_py:
        convertToCsv(f, 'KO')
    for f in file_list_py:
        convertToCsv(f, 'EN')
        
        
        