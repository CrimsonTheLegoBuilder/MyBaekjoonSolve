def divide(s, e):
    if e == s:
        return s
    if e - s == 1:
        return game(s, e)
    mid = (s + e) // 2
    s_ = divide(s, mid)
    e_ = divide(mid+1, e)
    return game(s_, e_)


def game(s, e):
    if s_w[arr[e]] == arr[s] or arr[s] == arr[e]:
        return s
    return e


for tc in range(1, int(input())+1):
    N = int(input())
    arr = list(map(int, input().split()))
    s_w = (0, 2, 3, 1)
    winner = divide(0, N-1) + 1

    print(f'#{tc} {winner}')
