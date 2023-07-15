import sys

n = int(input())
arr = []

for _ in range(n):
    arr.append(int(sys.stdin.readline().strip()))

arr_n = []

for i in range(len(arr) - 1):
    arr_n.append(arr[i + 1] - arr[i])

cnt = 10 ** 10

for m in range(len(arr_n) - 1):
    a, b = arr_n[m], arr_n[m + 1]
    while b:
        a, b = b, a % b
        if b == 0:
            if a < cnt:
                cnt = a

m = (max(arr) - min(arr)) // cnt
# print(cnt)
# print(max(arr))
# print(m)
print(m - n + 1)
