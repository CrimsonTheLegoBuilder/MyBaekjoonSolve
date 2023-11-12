import sys

n = int(sys.stdin.readline().strip())
arr = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(6)]
# direction = [4, 2, 3, 1]
ans = int()

for i in range(6):
    if ((arr[i][0] == 1) & (arr[i - 1][0] == 4)) | ((arr[i][0] == 4) & (arr[i - 1][0] == 2)) | \
            ((arr[i][0] == 2) & (arr[i - 1][0] == 3)) | ((arr[i][0] == 3) & (arr[i - 1][0] == 1)):
        area2 = arr[i][1] * arr[i - 1][1]
        idx1 = (i + 2) % 6
        idx2 = (i + 3) % 6
        area1 = arr[idx1][1] * arr[idx2][1]
        ans = n * (area1 - area2)

print(ans)

'''
1
2 2
3 2
1 1
4 1
1 1
4 1

1
4 2
2 1
3 1
2 1
3 1
1 2

1
3 2
1 2
4 1
2 1
4 1
2 1

1
4 2
2 2
3 1
1 1
3 1
1 1



'''