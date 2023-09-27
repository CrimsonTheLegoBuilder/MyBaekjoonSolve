import sys


def merge_sort(arr, x, y):
    if len(arr) <= 1:
        return arr

    mid = len(arr) // 2
    left = arr[:mid]
    right = arr[mid:]

    left_ = merge_sort(left, x, y)
    right_ = merge_sort(right, x, y)
    return merge(left_, right_, x, y)


def merge(left, right, x, y):
    i, j = 0, 0
    sorted_list = []
    while i < len(left) and j < len(right):
        if left[i][x] == right[j][x]:
            if left[i][y] <= right[j][y]:
                sorted_list.append(left[i])
                i += 1
            else:
                sorted_list.append(right[j])
                j += 1
        elif left[i][x] < right[j][x]:
            sorted_list.append(left[i])
            i += 1
        elif left[i][x] > right[j][x]:
            sorted_list.append(right[j])
            j += 1
    while i < len(left):
        sorted_list.append(left[i])
        i += 1
    while j < len(right):
        sorted_list.append(right[j])
        j += 1
    return sorted_list


a = int(sys.stdin.readline().strip())
dots = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(a)]
ans = 0

dots_x = merge_sort(dots, 0, 1)
dots_y = merge_sort(dots, 1, 0)
for k in range(0, a - 1, 2):
    ans += dots_x[k + 1][1] - dots_x[k][1]
    ans += dots_y[k + 1][0] - dots_y[k][0]

print(ans)
