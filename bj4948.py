import sys


def prime_list(n):
    arr = [1] * n * 2
    sr_n = int((n * 2) ** 0.5)
    arr[0] = 0
    arr[1] = 0

    for i in range(2, sr_n + 1):
        if arr[i] == 1:
            for j in range(i * 2, n * 2, i):
                arr[j] = 0

    return [i for i in range(n + 1, n * 2) if arr[i] == 1]


while True:
    n = int(sys.stdin.readline().strip())
    if n == 0:
        break
    elif n == 1:
        print(1)
    else:
        print(len(prime_list(n)))
