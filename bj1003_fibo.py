import sys

fibo = [0] * 41
fibo[0] = 1
fibo[1] = 1
for i in range(2, 41):
    fibo[i] = fibo[i - 1] + fibo[i - 2]

n = int(sys.stdin.readline().strip())
for _ in range(n):
    m = int(sys.stdin.readline().strip())
    if m == 0:
        print('1 0')
    elif m == 1:
        print('0 1')
    else:
        print(fibo[m - 2], fibo[m - 1])
