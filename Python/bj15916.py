import sys

n = int(sys.stdin.readline())
arr = [0] + list(map(int, sys.stdin.readline().strip().split()))
k = int(sys.stdin.readline())
flag = 0

if arr[1] == k:
    print('T')
    flag += 1
else:
    for i in range(n):
        if ((arr[i] < i * k) & (arr[i + 1] >= (i + 1) * k)) | ((arr[i] > i * k) & (arr[i + 1] <= (i + 1) * k)) | (arr[1] == k):
            print('T')
            flag += 1
            break
if flag == 0:
    print('F')
#
# i = 0
# while True:
#     if ((arr[i] < i * k) & (arr[i + 1] >= (i + 1) * k)) | ((arr[i] > i * k) & (arr[i + 1] <= (i + 1) * k)):
#         print('T')
#         break
#     if i == n:
#         print('F')
#         break
#     i += 1

