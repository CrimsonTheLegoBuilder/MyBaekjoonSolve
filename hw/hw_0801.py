# t = int(input())
# for tc in range(1, t + 1):
#     n = int(input())
#     station = [0] * 5001
#     for _ in range(n):
#         a1, b1 = map(int, input().split())
#         for i in range(a1, b1 + 1):
#             station[i] += 1
#     p = int(input())
#     ans = []
#     for _ in range(p):
#         ans.append(station[int(input())])
#     print(f'#{tc}', *ans)

# t = int(input())
# for tc in range(1, t + 1):
#     n = int(input())
#     cnt = [0] * 10
#     numbers = input()
#     for i in numbers:
#         cnt[int(i)] += 1
#     ans = [0, -1]
#     for i in range(10):
#         if cnt[i] > ans[1] or cnt[i] == ans[1] and i > ans[0]:
#             ans[0] = i
#             ans[1] = cnt[i]
#     print(f'#{tc}', *ans)

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
    boxes = merge_sort(boxes)
    total = 0
    for k in boxes:
        total += k
    mean = total // 100
    minn, maxx = 0, 0
    move_box_min = 0
    move_box_max = 0
    for k in range(100):
        if boxes[k] > mean:
            maxx += 1
            move_box_max += boxes[k]
        if boxes[k] < mean:
            minn += 1
            move_box_min += boxes[k]

    print(maxx, minn, move_box_max, move_box_min)
    H = (move_box_max - n) // maxx
    L = (move_box_min + n) // minn
    print(H, L)


    # for _ in range(n):
    #     boxes[0] += 1
    #     boxes[99] -= 1
    #     boxes = merge_sort(boxes)
    # ans = boxes[99] - boxes[0]
    # print(f'#{tc} {ans}')









