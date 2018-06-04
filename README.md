## Description
The goal of this project is to compare the performance between multithread and multiprocess programs that apply blur filter on images.

## Execution
In order to execute the program you have to type the following command on command line: ./main FILE_NAME MODE

- **FILE_NAME** must not include the file path, only the file name
- **MODE** accept just two assigments:
  __0__ execute main with threads
  __1__ execute main with processes

Also you are allowed to execute it on all images with **make test**. It will be generate a csv table with the taken time to execute blur filter in each image.

## Compilation
In order to compile files type **make** on command line

## Subsidiary Script
In order to compare the performance of the program you are allowed to execute the Subsidiary Script **generate_datas.py**.
The following command execute it: python3 generate_datas.py main

- It will be generate 2 csv tables named as Thread_Method.csv and Process_Method.csv
- Don't forget to compile the files before to execute it.
- The script allow you choose how many times you want to execute test.
