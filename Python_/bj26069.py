import sys

n = int(input())
dict_a = {}

for _ in range(n):
    a, b = sys.stdin.readline().strip().split()

    if a == 'ChongChong' or b == 'ChongChong':
        dict_a[a] = 1
        dict_a[b] = 1
    elif a in dict_a and dict_a[a] == 1:
        dict_a[b] = 1
    elif b in dict_a and dict_a[b] == 1:
        dict_a[a] = 1
    else:
        dict_a[a] = 0
        dict_a[b] = 0

cnt = 0

for i in dict_a:
    if dict_a[i] == 1:
        cnt += 1

print(cnt)
