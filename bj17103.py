import sys

a = int(input())

arr = [1] * 1_000_000
arr[0] = 0
arr[1] = 0
m = int(1_000_000 ** 1/2)

for i in range(2, m):
    if arr[i] == 1:
        for j in range(i + i, 1_000_000, i):
            arr[j] = 0

prime_list = [i for i in range(2, 1_000_000) if arr[i] == 1]


for _ in range(a):
    n = int(sys.stdin.readline().strip())
    cnt = 0

    for k in prime_list:
        if n // 2 < k:
            break
        if arr[n - k] == 1:
            cnt += 1

    print(cnt)
    # if cnt % 2 == 1:
    #     print(cnt // 2 + 1)
    # else:
    #     print(cnt // 2)
