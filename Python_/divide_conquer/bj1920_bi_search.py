import sys


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

    while (i < len(left)) & (j < len(right)):
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


def bi_search(arr, x):
    start = 0
    end = len(arr) - 1

    while start <= end:
        mid = (start + end) // 2
        if arr[mid] == x:
            return mid
        elif arr[mid] < x:
            start = mid + 1
        else:
            end = mid - 1

    return None


_ = sys.stdin.readline()
arr1 = list(map(int, sys.stdin.readline().strip().split()))
_ = sys.stdin.readline()
target = list(map(int, sys.stdin.readline().strip().split()))
arr_ans = merge_sort(arr1)

for k in target:
    if bi_search(arr_ans, k) is None:
        print(0)
    else:
        print(1)
