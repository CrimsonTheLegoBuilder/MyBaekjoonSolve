for tc in range(1, int(input())+1):
    N, M = map(int, input().split())
    container = sorted(list(map(int, input().split())))
    truck = sorted(list(map(int, input().split())))
    ans = 0
    while container:
        p = container.pop()
        if truck and p <= truck[-1]:
            ans += p
            truck.pop()
    print(f'#{tc} {ans}')
