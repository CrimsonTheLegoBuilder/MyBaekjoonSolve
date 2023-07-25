import sys

n = int(sys.stdin.readline().strip())
x = int(sys.stdin.readline().strip())
arr = [[0 for _ in range(n)] for _ in range(n)]
dr = 0
lu = n - 1
cnt = n**2
ans = None
for _ in range(n**2):
    for j in range(dr, lu + 1):
        arr[j][dr] = cnt
        if cnt == x:
            ans = j + 1, dr + 1
        cnt -= 1
    for j in range(dr + 1, lu + 1):
        arr[lu][j] = cnt
        if cnt == x:
            ans = lu + 1, j + 1
        cnt -= 1
    for j in range(lu - 1, dr - 1, -1):
        arr[j][lu] = cnt
        if cnt == x:
            ans = j + 1, lu + 1
        cnt -= 1
    for j in range(lu - 1, dr, -1):
        arr[dr][j] = cnt
        if cnt == x:
            ans = dr + 1, j + 1
        cnt -= 1
    dr += 1
    lu -= 1

for y in arr:
    print(*y)
print(*ans)
