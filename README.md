## Description
The goal of this project is to compare the performance between multithread and multiprocess programs that apply blur filter on images. In order to accomplish this task, it was made 2 diferents functions that apply blur on image and it was counted the taken time in each case. The functions were implemented in a way that they are fair enougth with each other, otherwise this project had no meaning.

## Execution
In order to execute the program you have to type the following command on command line: **./main FILE_NAME MODE**

- **FILE_NAME** must not include the file path, only the file name
- **MODE** accept just two assigments:
  ```
  __0__ execute main with threads
  __1__ execute main with processes
  ```

Also you are allowed to execute it on all images with **make test**. It will be generate a csv table with the taken time to execute blur filter in each image.

## Compilation
In order to compile all files type **make** on command line

## Subsidiary Script
In order to compare the performance of the program you are allowed to execute the Subsidiary Script **generate_datas.py**.
The following command execute it: **python3 generate_datas.py main**

- It will be generate 2 csv tables named as __Thread_Method.csv__ and __Process_Method.csv__
- Don't forget to compile the files before to execute it.
- The script allow you choose how many times you want to execute test.
