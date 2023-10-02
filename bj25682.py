import sys

n, m, k = map(int, sys.stdin.readline().strip().split())
grid = [str(sys.stdin.readline().strip()) for _ in range(n)]
m1 = [[0]*(m+1) for _ in range(n+1)]
m2 = [[0]*(m+1) for _ in range(n+1)]

for i in range(1, n+1):
    for j in range(1, m+1):
        if (i + j) % 2:
            if grid[i-1][j-1] == 'W':
                m1[i][j] = m1[i][j-1] + m1[i-1][j] - m1[i-1][j-1] + 0
                m2[i][j] = m2[i][j-1] + m2[i-1][j] - m2[i-1][j-1] + 1
            else:
                m1[i][j] = m1[i][j-1] + m1[i-1][j] - m1[i-1][j-1] + 1
                m2[i][j] = m2[i][j-1] + m2[i-1][j] - m2[i-1][j-1] + 0
        else:
            if grid[i-1][j-1] == 'B':
                m1[i][j] = m1[i][j-1] + m1[i-1][j] - m1[i-1][j-1] + 0
                m2[i][j] = m2[i][j-1] + m2[i-1][j] - m2[i-1][j-1] + 1
            else:
                m1[i][j] = m1[i][j-1] + m1[i-1][j] - m1[i-1][j-1] + 1
                m2[i][j] = m2[i][j-1] + m2[i-1][j] - m2[i-1][j-1] + 0
change = k**2
for i in range(n+1-k):
    for j in range(m+1-k):
        sum1 = m1[i+k][j+k] - m1[i][j+k] - m1[i+k][j] + m1[i][j]
        sum2 = m2[i+k][j+k] - m2[i][j+k] - m2[i+k][j] + m2[i][j]
        change = min(change, sum1, sum2)
print(change)
