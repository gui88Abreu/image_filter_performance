'''Test Script '''

from os import system
import sys

import re

def yield_datas(line):
  user, sys, real, cpu, garbage = line.split(' ', 4)
  garbage = None

  regex = re.compile('[0-9]+[.][0-9]+')
  user = regex.findall(user)
  sys = regex.findall(sys)
  real = regex.findall(real)

  regex = re.compile('[0-9]+')
  cpu = regex.findall(cpu)

  return (real[0], user[0], sys[0], cpu[0])

if len(sys.argv) != 2:
  quit()  

images_directory = "images"
program_target = sys.argv[1]
temp_file = "temp"

system('ls ' + images_directory + '>' + temp_file)

fl = open(temp_file, 'r')
test_list = fl.readlines()
fl.close
system('rm -f ' + temp_file)

if len(test_list) == 0:
  quit('There is no test files')

log_file = "log.lg"
for i in range(len(test_list)):
  test_list[i] = test_list[i].replace('\n', '')
  print(60*'-')
  print('Test Image: ' + test_list[i])
  system('/usr/bin/time -o '+ log_file + ' --append ./'+program_target + ' ' + test_list[i])
print(60*'-')
file = open(log_file, 'r')

lines = []
line = file.readline()
while line:
  lines.append(line)
  line = file.readline()
file.close()

system('rm -f ' + log_file)

planilha = open('threading.csv', 'w')
planilha.write('Test,User [s],Real [s],System [s],CPU\n')
j = 0
for i in range(0, len(lines), 2):
  datas = yield_datas(lines[i])
  planilha.write(test_list[j] + ',' + datas[0] + ',' + datas[1] + ',' + datas[2] + ',' + datas[3] + '%\n')
  j += 1
planilha.close()
