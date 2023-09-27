import sys

n = int(input())

for _ in range(n):
    stack = 0
    temp = 0
    vps = sys.stdin.readline().strip()

    for i in vps:
        if i == '(':
            stack += 1
        else:
            if stack == 0:
                temp += 1
            else:
                stack -= 1

    if stack == 0 and temp == 0:
        print('YES')
    elif temp != 0:
        print('NO')
    else:
        print('NO')
