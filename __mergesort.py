# 길이 - 알파벳 순 정렬 프로그램

import sys

N = int(input())
arr = []

for _ in range(N):
    arr.append(sys.stdin.readline().strip())


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
        if len(left[i]) == len(right[j]):
            for k in range(len(left[i])):
                if ord(left[i][k]) < ord(right[j][k]):
                    sorted_list.append(left[i])
                    i += 1
                    break
                elif ord(left[i][k]) > ord(right[j][k]):
                    sorted_list.append(right[j])
                    j += 1
                    break
                else:
                    if k == len(left[i]) - 1 and ord(left[i][k]) == ord(right[j][k]):
                        sorted_list.append(left[i])
                        i += 1
                        # sorted_list.append(right[j]) 중복 제거
                        j += 1  # 중복을 제거하더라도 횟수에는 포함되어야함
                        break
        elif len(left[i]) < len(right[j]):
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


arr_1 = merge_sort(arr)
for l in arr_1:
    print(l)
