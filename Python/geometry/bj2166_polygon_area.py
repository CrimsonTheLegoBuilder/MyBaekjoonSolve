import sys

n = int(sys.stdin.readline().strip())
dot = [list(map(float, sys.stdin.readline().strip().split())) for _ in range(n)]
pivot = [0, 0]
area = 0


def cross(arr1, arr2, x):
    ans = (arr1[0] - x[0]) * (arr2[1] - arr1[1]) - (arr2[0] - arr1[0]) * (arr1[1] - x[1])
    return ans


for i in range(n):
    area += cross(dot[i], dot[i - 1], pivot)

print(abs(area / 2))
