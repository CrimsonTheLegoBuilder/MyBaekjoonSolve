import sys


def merge_sort(n):
    if len(n) <= 1:
        return n

    mid = (len(n) + 1) // 2
    left = n[:mid]
    right = n[mid:]

    left_ = merge_sort(left)
    right_ = merge_sort(right)
    return merge(left_, right_)


def merge(left, right):
    i, j = 0, 0
    sorted_list = []
    global cnt, answer

    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            sorted_list.append(left[i])
            cnt += 1
            if cnt == x:
                answer = left[i]
            i += 1
        else:
            sorted_list.append(right[j])
            cnt += 1
            if cnt == x:
                answer = right[j]
            j += 1
    while i < len(left):
        sorted_list.append(left[i])
        cnt += 1
        if cnt == x:
            answer = left[i]
        i += 1
    while j < len(right):
        sorted_list.append(right[j])
        cnt += 1
        if cnt == x:
            answer = right[j]
        j += 1

    return sorted_list


_, x = map(int, input().split())
arr = list(map(int, sys.stdin.readline().strip().split()))

cnt = 0
answer = -1
a = merge_sort(arr)
print(answer)
