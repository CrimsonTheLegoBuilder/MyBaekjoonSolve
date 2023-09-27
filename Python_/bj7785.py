import sys
n = int(input())
dict_1 = {}

for _ in range(n):
    a, b = sys.stdin.readline().split()
    dict_1[a] = b
    if dict_1.get(a) == 'leave':
        del dict_1[a]

dict_2 = sorted(dict_1, reverse=True)
print(*dict_2)
