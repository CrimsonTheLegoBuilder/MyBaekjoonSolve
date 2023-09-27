import sys

N, M = map(int, sys.stdin.readline().split())
S = []
arr_M = []

for _ in range(N):
    S.append(sys.stdin.readline())

for _ in range(M):
    arr_M.append(sys.stdin.readline())

S.sort()


def bi_search(target, data):
    start = 0
    end = len(data) - 1

    while start <= end:
        mid = (start + end) // 2
        if data[mid] == target:
            return data[mid]
        elif data[mid] < target:
            start = mid + 1
        else:
            end = mid - 1
    return None


cnt = 0
for i in arr_M:
    if bi_search(i, S):
        cnt += 1

print(cnt)
