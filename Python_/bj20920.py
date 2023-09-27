import sys

n, m = map(int, sys.stdin.readline().strip().split())
dic = {}


def merge_len_sort(a):
    if len(a) <= 1:
        return a
    mid = len(a) // 2
    left = a[:mid]
    right = a[mid:]

    left_ = merge_len_sort(left)
    right_ = merge_len_sort(right)
    return merge_len(left_, right_)


def merge_len(left, right):
    i, j = 0, 0
    sorted_list = []

    while i < len(left) and j < len(right):
        if len(left[i]) >= len(right[j]):
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


for i in range(n):
    a = sys.stdin.readline().strip()
    if len(a) >= m:
        if a in dic:
            dic[a] += 1
        else:
            dic[a] = 1

answer = []

while True:
    if len(dic) == 0:
        break
    temp = []
    cnt = 0
    for i in dic:
        if dic[i] > cnt:
            temp.clear()
            temp.append(i)
            cnt = dic[i]
        elif dic[i] == cnt:
            temp.append(i)
        else:
            continue
    temp.sort()
    # 길이순 정렬
    temp_ = merge_len_sort(temp)

    answer = answer + temp_
    for i in temp:
        del dic[i]

for k in answer:
    print(k)
