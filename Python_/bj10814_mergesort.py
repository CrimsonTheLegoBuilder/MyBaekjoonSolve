import sys

N = int(input())
arr = []

for _ in range(N):
    a, b = sys.stdin.readline().split()
    arr.append((int(a), b))


def merge_sort(n):
    if len(n) <= 1:
        return n

    mid = len(n) // 2
    left = n[:mid]
    right = n[mid:]

    left_ = merge_sort(left)
    right_ = merge_sort(right)
    return merge(left_, right_)


def merge(left, right):
    i, j = 0, 0
    sorted_list = []

    while i < len(left) and j < len(right):
        if left[i][0] <= right[j][0]:
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


a = merge_sort(arr)
for k in a:
    print(*k)
