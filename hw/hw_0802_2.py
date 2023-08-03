t = int(input())
for tc in range(1, 1 + t):
    n, m = map(int, input().split())
    arr = [tuple(map(int, input().split())) for _ in range(n)]
    ans = 0
    for i in range(n - m + 1):
        for j in range(n - m + 1):
            total = 0
            for k in range(m):
                for l in range(m):
                    total += arr[i + k][j + l]
            if ans < total:
                ans = total
    print(f'#{tc} {ans}')

