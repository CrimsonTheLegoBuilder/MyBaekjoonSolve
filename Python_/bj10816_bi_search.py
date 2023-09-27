import sys

N = int(input())
arr_N = list(map(int, sys.stdin.readline().strip().split()))

M = int(input())
arr_M = list(map(int, sys.stdin.readline().strip().split()))


def merge_sort(arr):
    if len(arr) <= 1:
        return arr

    mid = len(arr) // 2
    left = arr[:mid]
    right = arr[mid:]

    left_ = merge_sort(left)
    right_ = merge_sort(right)
    return merge(left_, right_)


def merge(left, right):
    i, j = 0, 0
    sorted_list = []

    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
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


def bi_search(target, data):
    start = 0
    end = len(data) - 1

    while start <= end:
        mid = (start + end) // 2

        if data[mid] == target:
            return mid
        elif data[mid] < target:
            start = mid + 1
        else:
            end = mid - 1

    return None


arr_s_M = merge_sort(arr_M)

cnt_arr = [0] * M

for k in arr_N:
    a = bi_search(k, arr_s_M)
    if a == None:
        continue
    cnt_arr[arr_M.index(arr_s_M[a])] += 1
#     print(arr_s_M.index(arr_s_M[a]))
#
# print(arr_s_M)
print(*cnt_arr)
