import sys

N, M = map(int, sys.stdin.readline().strip().split())
arr_a = [list(map(int, list(str(sys.stdin.readline().strip())))) for _ in range(N)]
arr_b = [list(map(int, list(str(sys.stdin.readline().strip())))) for _ in range(N)]
cnt = 0
for i in range(N-2):
    for j in range(M-2):
        if arr_a[i][j] != arr_b[i][j]:
            cnt += 1
            for k in range(3):
                for l in range(3):
                    if arr_a[i+k][j+l]:
                        arr_a[i + k][j + l] -= 1
                    else:
                        arr_a[i + k][j + l] += 1
# print(arr_a)
# print(arr_b)
for i in range(N):
    for j in range(M):
        if arr_a[i][j] != arr_b[i][j]:
            cnt = -1
print(cnt)

