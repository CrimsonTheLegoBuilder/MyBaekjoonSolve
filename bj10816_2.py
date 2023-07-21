import sys

N = int(input())
arr_N = list(map(int, sys.stdin.readline().strip().split()))

M = int(input())
arr_M = list(map(int, sys.stdin.readline().strip().split()))
dict_M = dict.fromkeys(arr_M, 0)

# print(dict_M)


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


arr_s_M = sorted(arr_M)

for i in arr_N:
    if bi_search(i, arr_s_M):
        dict_M[i] = int(dict_M[i]) + 1

# print(dict_M)

for M in arr_M:
    print(dict_M[M], end=' ')
