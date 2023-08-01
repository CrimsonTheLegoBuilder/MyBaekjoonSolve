def bubble_sort(arr):
    for i in range(98):
        for j in range(i, 99):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]


for tc in range(1, 11):
    n = int(input())
    boxes = list(map(int, input().split()))
    bubble_sort(boxes)
    total = 0
    for _ in range(n):
        boxes[0] += 1
        boxes[99] -= 1
        bubble_sort(boxes)
    print(f'#{tc} {boxes[99] - boxes[0]}')
