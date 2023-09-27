import sys

n = int(sys.stdin.readline().strip())
triangle = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]

for i in range(1, n):
    for j in range(i + 1):
        if j == 0:
            triangle[i][j] = triangle[i][j] + triangle[i - 1][0]
        elif j == i:
            triangle[i][j] = triangle[i][j] + triangle[i - 1][j - 1]
        else:
            triangle[i][j] = triangle[i][j] + max(triangle[i - 1][j - 1], triangle[i - 1][j])

# print(triangle)
print(max(triangle[n - 1]))
