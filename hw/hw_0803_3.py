def bi_search(target):
    cnt = 0
    start, end = 1, s
    while start <= end:
        cnt += 1
        mid = (start + end) // 2
        if mid == target:
            return cnt
        elif mid > target:
            end = mid
        else:
            start = mid


t = int(input())
for tc in range(1, t+1):
    s, A, B = map(int, input().split())
    a = bi_search(A)
    b = bi_search(B)
    ans = 0
    # if a > b:
    #     ans = 'B'
    # elif a < b:
    #     ans = 'A'
    print(f'#{tc} {a, b}')
