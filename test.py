'''Test Script '''

from os import system
import sys

if len(sys.argv) != 2:
  quit()  

images_directory = "images"
program_target = sys.argv[1]
temp_file = "temp"

system('ls ' + images_directory + '>' + temp_file)

fl = open(temp_file, 'r')
test_list = fl.readlines()
fl.close

if len(test_list) == 0:
  quit('There is no test files')

for test in test_list:
  print('Test Image: ' + test)
  system('time ./'+program_target + ' ' + test)

system('rm -f ' + temp_file)
