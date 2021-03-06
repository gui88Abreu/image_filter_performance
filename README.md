## Description
The goal of this project is to compare the performance between multithread and multiprocess programs that apply blur filter on images. In order to accomplish this task, it was made 3 diferents functions that apply blur on image and it was counted the taken time in each case.The third case apply filter without parallelism. The functions were implemented in a way that they are fair enougth with each other, otherwise this project had no meaning.

## Compilation
In order to compile all files type **make** on command line. The command **make clean** remove all generated files.

## Execution
In order to execute the program you have to type the following command on command line: **./main IMAGE_NAME MODE**

- **IMAGE_NAME** must not include the file path, only the file name.
- **MODE** accept just three assigments:
  ```
  0 execute main with threads
  1 execute main with processes
  -1 execute main without parallelism
  ```
Also you are allowed to execute it on all images with **make test**. It will be generate a csv table with the taken time to execute blur filter in each image. The images must be in the directory named as **images**, the output images will be saved in the directory **filtered_images**.

Note: The program will run the maximum of threads available in your system. Make sure the all of them be active.

## Subsidiary Script
In order to compare the performance of the program you are allowed to execute the Subsidiary Script **generate_datas.py**.
The following command execute it: **python3 generate_datas.py**

- It will be generate 3 csv tables named as __Thread_Method.csv__, __Process_Method.csv__ and __Single_Processing__.
- Don't forget to compile the files before to execute it.
- The script allow you choose how many times you want to execute test.
