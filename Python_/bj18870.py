import sys

N = int(input())
arr = list(sys.stdin.readline().split())
arrcnt = []

for s in range(N):
    arrcnt.append([int(arr[s]), s])


def merge_sort(n, x):
    if len(n) <= 1:
        return n

    mid = len(n) // 2
    left = n[:mid]
    right = n[mid:]

    left_ = merge_sort(left, x)
    right_ = merge_sort(right, x)
    return merge(left_, right_, x)


def merge(left, right, x):
    i, j = 0, 0
    sorted_list = []

    while i < len(left) and j < len(right):
        if left[i][x] <= right[j][x]:
            sorted_list.append(left[i])
            i += 1
        else:
            sorted_list.append(right[j])
            j += 1
    while i < len(left):
        sorted_list.append(left[i])
        i += 1
    while j < len(right):
        sorted_list.append(right[j])
        j += 1
    return sorted_list


a = merge_sort(arrcnt, 0)

cur_x = a[0][0]
pos_x = 0

for q in range(N):
    if a[q][0] != cur_x:
        cur_x = a[q][0]
        pos_x += 1
    a[q][0] = pos_x

b = merge_sort(a, 1)
for c in range(N):
    sys.stdout.write(str(b[c][0]))
    sys.stdout.write(' ')
