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


for tc in range(1, 11):
    n = int(input())
    boxes = list(map(int, input().split()))
    # for i in range(99, 0, -1):
    #     for j in range(i):
    #         if boxes[j] > boxes[j + 1]:
    #             boxes[j], boxes[j + 1] = boxes[j + 1], boxes[j]
    boxes = merge_sort(boxes)
    # flag = 0
    M = 99
    m = 0
    for _ in range(n):
        boxes[m] += 1
        boxes[M] -= 1
        if boxes[M] < boxes[M - 1]:
            M -= 1
        if M != 99 and boxes[M] <= boxes[M + 1]:
            M = 99
        if boxes[m] > boxes[m + 1]:
            m += 1
        if m != 0 and boxes[m] >= boxes[m - 1]:
            m = 0
        if boxes[m] >= boxes[M]:
            # flag = 1
            break
    # if flag:
    #     print(f'#{tc} {1}')
    #     continue
    M = -1
    m = 101
    for i in range(100):
        if boxes[i] > M:
            M = boxes[i]
        if boxes[i] < m:
            m = boxes[i]
    print(f'#{tc} {M - m}')
