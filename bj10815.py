import sys

N_cur = int(input())
arr_cur = list(map(int, sys.stdin.readline().split()))
N_comp = int(input())
arr_comp = list(map(int, sys.stdin.readline().split()))
cnt = [0] * N_comp


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


arr = merge_sort(arr_cur)

for i in range(N_comp):
    if bi_search(arr_comp[i], arr) != None:
        cnt[i] += 1

print(*cnt)
