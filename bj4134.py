import sys

n = int(input())

for _ in range(n):
    a = int(sys.stdin.readline().strip())

    if a == 0 or a == 1: # a <= 1
        print(2)

    else:
        while True:
            flag = 0
            for i in range(2, int(a ** (1/2) + 1)):
                if a % i == 0:
                    a += 1
                    flag += 1
                    break
                flag = 0

            if flag == 0:
                print(a)
                break
