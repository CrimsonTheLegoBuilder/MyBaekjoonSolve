import sys
from math import log

_, a, b = map(int, sys.stdin.readline().strip().split())
print(int(log((a-1) ^ (b-1), 2))+1)

'''
@;1-\1-^
'''