import sys

a, b = map(int, input().split())
dict_1 = {}
dict_2 = {}

for i in range(a):
    c = sys.stdin.readline()
    dict_1[i + 1] = c
    dict_2[c] = i + 1

for j in range(b):
    n = sys.stdin.readline()
    if 48 <= ord(n[0]) <= 57:
        print(dict_1.get(int(n)), end='')

    elif 65 <= ord(n[0]) <= 122:
        print(dict_2.get(n))
