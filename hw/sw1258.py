import sys
sys.stdin = open('input_magazine.txt', 'r')


def merge_sort(arr):  # 병합 정렬. 망할 sort() 함수 못 쓰게 해서 다 만들어야되잖아 씨부럴
    if len(arr) <= 1:
        return arr    # len() 함수까지 못 썼으면 진짜 X됐다
    mid = len(arr)//2
    left = []
    for o in range(0, mid):
        left.append(arr[o])
    right = []
    for o in range(mid, len(arr)):
        right.append(arr[o])
    left_ = merge_sort(left)
    right_ = merge_sort(right)
    return merge(left_, right_)


def merge(left, right):
    i, j = 0, 0
    sorted_list = []
    while i < len(left) and j < len(right):
        if left[i][0] == right[j][0]:
            if left[i][1] < right[j][1]:    # 넓이에 대해 정렬해보고 같으면 행으로 비교하랬음
                sorted_list.append(left[i])
                i += 1
            else:
                sorted_list.append(right[j])
                j += 1
        elif left[i][0] < right[j][0]:
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


def scan(r, c):  # 어차피 부분행렬은 0으로 구분되어있거나 벽에 붙어있으니까 무식하게 세면 됨
    row, col = 0, 0
    nr, nc = r, c
    while True:
        nr += 1
        row += 1
        if nr >= N or not magazine[nr][c]:  # 리스트를 벗어나거나 0이면 부분행렬 끝 지점임
            break
    while True:
        nc += 1
        col += 1
        if nc >= N or not magazine[r][nc]:
            break
    for x in range(r, nr):
        for y in range(c, nc):
            magazine[x][y] = 0  # 다 봤으면 없애버림
    return row*col, row, col    # 반환값은 넓이, 행, 열


for tc in range(1, int(input())+1):
    N = int(input())
    magazine = [list(map(int, input().split())) for _ in range(N)]
    sub_matrix = []
    for i in range(N):
        for j in range(N):
            if magazine[i][j] > 0:
                sub_matrix.append(scan(i, j))
    # print(sub_matrix)
    sorted_magazine = merge_sort(sub_matrix)
    ans = []
    for _, r, c in sorted_magazine:
        ans. append(r)
        ans. append(c)
    print(f'#{tc}', len(ans)//2, *ans)
