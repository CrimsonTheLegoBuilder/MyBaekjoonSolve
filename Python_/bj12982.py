import sys

K = int(sys.stdin.readline().strip())
arr = list(map(int, sys.stdin.readline().strip().split()))
ans = 0
m = int(12e12)

for i in range(K):
    ans += arr[i] // K
    arr[i] %= K

# arr.sort(key=lambda x: -x)
arr.sort(reverse=True)
for _ in range(K):
    if arr[-1]:
        break
    arr.pop()

if not arr:
    print(ans)
    exit(0)
# print(ans)
# print(arr)
m = min(m, len(arr), arr[0])
for i in range(1, len(arr)):
    m = min(m, i + arr[i])
print(ans + m)
