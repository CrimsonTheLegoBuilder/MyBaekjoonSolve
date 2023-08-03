def sub_set_sum(n, k):
    global ans
    for i in range(1 << 12):
        total = 0
        cnt = 0
        for j in range(12):
            if i & (1 << j):
                total += arr[j]
                cnt += 1
        if cnt == n and total == k:
            ans += 1
    return


t = int(input())
for tc in range(1, t+1):
    N, K = map(int, input().split())
    arr = tuple(range(1, 13))
    ans = 0
    sub_set_sum(N, K)
    print(f'#{tc} {ans}')
