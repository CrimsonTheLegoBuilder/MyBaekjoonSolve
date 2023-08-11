def arr_rotate(arr):
    x = len(arr)
    new = [[''] * x for _ in range(x)]
    for y in range(n):
        for z in range(n):
            new[y][z] = arr[n-1-z][y]
    return new


def arr_merge(arr1, arr2, arr3):
    x = len(arr1)
    new = [[''] * x for _ in range(x)]
    for y in range(n):
        new[y][0] = ''.join(arr1[y])
        new[y][1] = ''.join(arr2[y])
        new[y][2] = ''.join(arr3[y])
    return new


for tc in range(1, int(input())+1):
    n = int(input())
    arr0 = [list(map(str, input().strip().split())) for _ in range(n)]
    arr90 = arr_rotate(arr0)
    arr180 = arr_rotate(arr90)
    arr270 = arr_rotate(arr180)
    # print(arr90)
    # print(arr180)
    # print(arr270)
    ans = arr_merge(arr90, arr180, arr270)
    print(f'#{tc}')
    for row in ans:
        print(*row)
