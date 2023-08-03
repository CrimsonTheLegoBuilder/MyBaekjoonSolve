# import sys
# sys.stdin = open('input (8).txt')
#


t = int(input())
for tc in range(1, t+1):
    n = int(input())
    arr = list(map(int, input().split()))
    for j in range(n - 1, - 1, -1):
        for k in range(n - 1):
            if arr[k] < arr[k + 1]:
                arr[k], arr[k + 1] = arr[k + 1], arr[k]
    for l in range(1, n):
        for m in range((n - l) // 2):
            arr[l + m], arr[n - 1 - m] = arr[n - 1 - m], arr[l + m]
    if n <= 10:
        print(f'#{tc}', *arr)
    else:
        ans = []
        for x in range(10):
            ans.append(arr[x])
        print(f'#{tc}', *ans)
