import sys
sys.stdin = open('input_hw081104.txt', 'r')

for tc in range(1, int(input())+1):
    n = int(input())
    arr = list(map(int, input().split()))
    # stack = []
    profit = 0
    cur = arr[n-1]
    for i in range(n-2, -1, -1):
        if cur > arr[i]:
            profit += cur - arr[i]
        if cur < arr[i]:
            cur = arr[i]

    print(f'#{tc} {profit}')
