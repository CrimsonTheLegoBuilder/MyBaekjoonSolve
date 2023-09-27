import sys

n = int(input())
arr = []

for _ in range(n):
    arr.append(int(sys.stdin.readline().strip()))

arr.sort()

art_mean = round(sum(arr) / n)
print(art_mean)

median = arr[n // 2]
print(median)

dic_a = {}
for i in arr:
    if i in dic_a:
        dic_a[i] += 1
    else:
        dic_a[i] = 1
mode_arr = []
cnt = 0
for j in dic_a:
    if dic_a[j] > cnt:
        mode_arr.clear()
        mode_arr.append(j)
        cnt = dic_a[j]
    elif dic_a[j] == cnt:
        mode_arr.append(j)
    else:
        continue

mode_arr.sort()
if len(mode_arr) == 1:
    print(mode_arr[0])
else:
    print(mode_arr[1])

range_arr = arr[-1] - arr[0]
print(range_arr)
