#!python
import sys
if(len(sys.argv)!=3):
    exit()
l = list(range(int(sys.argv[1])))
step = int(sys.argv[2])
i = 0
while len(l)>1:
    i += step
    while i>=len(l):
        i %= len(l)
    print(' ', l.pop(i), sep='') #can see the intermediate output
print(l)