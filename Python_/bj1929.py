import sys

n, m = map(int, input().split())

for i in range(n, m + 1):
    flag = 0
    for j in range(2, int(i ** (1/2) + 1)):
        if i % j == 0:
            flag += 1
            break
        flag = 0

    if flag == 0:
        if i == 1:
            continue
        print(i)
