import sys

n = int(sys.stdin.readline().strip())

arr = list(map(int, sys.stdin.readline().strip().split()))
mm = 0

for i in range(n - 1):
    dp1 = [0] * n
    dp1[0] = 1
    for j in range(i + 1):
        if arr[j] < arr[i + 1]:
            if dp1[i + 1] <= dp1[j]:
                dp1[i + 1] = dp1[j]
    dp1[i + 1] += 1
    for k in range(i, n - 1):
        for l in range(i, k + 1):
            if arr[l] > arr[k + 1]:
                if dp1[k + 1] <= dp1[l]:
                    dp1[k + 1] = dp1[l]
        dp1[k + 1] += 1
    if mm < max(dp1):
        mm = max(dp1)

print(mm)
