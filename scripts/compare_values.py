import re
import sys
import os
file0 = '/Users/danielwang/Local/Fourth Year/Capstone/implementation/out.txt'
file1 = '/Users/danielwang/Local/Fourth Year/Capstone/Capstone_Implementation/c_models/out.txt'

with open(file0, 'r') as infile, open('temp0.txt', 'w') as outfile:
    temp = infile.read().replace("(", "").replace(")", "").replace("[", "").replace("]","").replace(",", "").replace('tensor', "")
    outfile.write(temp)

with open(file1, 'r') as infile, open('temp1.txt', 'w') as outfile:
    temp = infile.read().replace("(", "").replace(")", "").replace("[", "").replace("]","").replace(",", "").replace('tensor', "")
    outfile.write(temp)


file0 = open('temp0.txt', 'r')
file0 = file0.read()

data0 = re.findall(r"[-+]?\d*\.\d+|\d+", file0)

file1 = open('temp1.txt', 'r')
file1 = file1.read()

data1 = re.findall(r"[-+]?\d*\.\d+|\d+", file1)

count = 0
print(len(data0))

print(len(data1))
while((count < len(data0)) and (count < len(data1))):
    print(data0[count] + " " + data1[count])
    if (float(data0[count]) != float(data1[count])):
        print("not equal at index " + str(count) + " value at data0 is " + data0[count] + " value at data1 is " + data1[count])
        break
    count += 1

print("traversed " + str(count) + " elements")
print("contents are equal")

os.remove('temp0.txt')
os.remove('temp1.txt')