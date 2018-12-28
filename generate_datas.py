#!/usr/bin/env python3

#*********************************************************************************************************#
                                              #***import librarys***#

from os import system #execute commands on command line
import sys #catch arguments from the command line that runs this script
import re #RegEx

#*********************************************************************************************************#
                                              #***function statements***#

def yield_table(time_file, Name, test_list, mode, test):
  ''' 
  Yield a csv table with datas that were colected and stored on time_file.
  String *Name must have something about the datas, example: Thread Method or just Thread.
  '''
  #Store on a list all datas yielded previously in make_test
  file = open(time_file, 'r')
  line = file.readlines()
  file.close()

  #Erase time_file
  system('rm -f ' + time_file)

  # Actually yield a csv table
  planilha = open(Name+'.csv', mode)
  planilha.write('Test,Width x Height,Real time[s]\n')
  
  regex = re.compile("[0-9]+[.][0-9]+")

  j = 1
  for l in line:
    result = regex.findall(l)
    if len(result) == 1:
      planilha.write('Test '+ str(j) + ',' + l)
      j += 1
  planilha.close()

def make_test(test_list, program_target, mode, N_blur, cpu, time_file, test, N):
  '''
  It does test program_target with the test_list and store the results on time_file.
  *mode must be assigned with '0' to execute program with threads or with '1' to execute with processes.
  '''
  i = 1
  file = test_list[test].replace('\n', '')
  print(60*'-')
  while i <= N:
    print(60*'-')
    print('Executing Test '+ str(i) + '...')
    system('./'+program_target + ' ' + file + ' ' + mode + ' ' + N_blur + ' ' + cpu + ' >>' + time_file)
    i+=1
  print(60*'-')

#*********************************************************************************************************#
                                              #***main***#

#File Names
images_directory = "images"
program_target = "main"
temp_file = "temp"
time_file = "time"
N_blur = '10'

#Catch the number of available threads of the processor
system('grep -c cpu[0-9] /proc/stat >>' + temp_file)
fl = open(temp_file,'r')
cpu = fl.readline().replace('\n','')
fl.close()

#Catch all file names in image_directory and store on temp_file
system('ls ' + images_directory + '>' + temp_file)

#Store all names found in image_directory on a list
fl = open(temp_file, 'r')
test_list = fl.readlines()
fl.close

#Erase temp_file
system('rm -f ' + temp_file)

if len(test_list) == 0:
  quit('There is no test files')

i = 0
print('Choose one file typing its ID\n')
for line in test_list:
  print(i,line, end ="")
  i+=1
print()

choice = int(input('type here your choice: '))

if choice < 0 or choice >= len(test_list):
  quit('Invalid Choice')

amount = int(input('type here how many times you want it be tested: '))

if amount < 1:
  quit('Invalid Amount')

print('\nThread Test')
make_test(test_list, program_target, '0', N_blur, cpu, time_file, choice, amount)
yield_table(time_file, 'Thread_Method', test_list, 'w', choice)

print('Process Test')
make_test(test_list, program_target, '1', N_blur, cpu,time_file, choice, amount)
yield_table(time_file, 'Process_Method', test_list, 'w', choice)

print('Single Processing Test')
make_test(test_list, program_target, '-1', N_blur, cpu, time_file, choice, amount)
yield_table(time_file, 'Single_Processing', test_list, 'w', choice)