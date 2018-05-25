'''Test Script '''

from os import system
import sys

def yield_table(time_file, Name):
  file = open(time_file, 'r')

  line = file.readlines()

  system('rm -f ' + time_file)

  planilha = open('Test_Result.csv', 'a')
  planilha.write(Name + '\n')
  planilha.write('Test,Usr time[s],Real time[s]\n')
  j = 0
  for i in range(2, len(line), 3):
    planilha.write(test_list[j] + ',' + line[i])
    j += 1
  planilha.write('\n')
  planilha.close()

def make_test(test_list, program_target, mode, time_file):
  for i in range(len(test_list)):
    test_list[i] = test_list[i].replace('\n', '')
    print(60*'-')
    print('Test Image: ' + test_list[i])
    system('./'+program_target + ' ' + test_list[i] + ' ' + mode + ' >>' + time_file)
  print(60*'-')

if len(sys.argv) != 2:
  quit()  

images_directory = "images"
program_target = sys.argv[1]
temp_file = "temp"
time_file = "time"

system('ls ' + images_directory + '>' + temp_file)

fl = open(temp_file, 'r')
test_list = fl.readlines()
fl.close
system('rm -f ' + temp_file)

if len(test_list) == 0:
  quit('There is no test files')

make_test(test_list, program_target, '0', time_file)
yield_table(time_file, 'Thread')

#When the part of the program that apply blur with process be ready, descomment the code below
'''
make_test(test_list, program_target, '1', time_file)
yield_table(time_file, 'Process')
'''