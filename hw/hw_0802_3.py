# t = int(input())
for tc in range(1, 11):
    _ = int(input())
    arr = [tuple(map(int, input().split())) for _ in range(100)]
    ans = 0
    for i in range(100):
        total = 0
        for j in range(100):
            total += arr[i][j]
        if ans < total:
            ans = total

    for j in range(100):
        total = 0
        for i in range(100):
            total += arr[i][j]
        if ans < total:
            ans = total

    total = 0
    for i in range(100):
        total += arr[i][i]
    if ans < total:
        ans = total

    total = 0
    for i in range(100):
        total += arr[i][99 - i]
    if ans < total:
        ans = total

    print(f'#{tc} {ans}')
