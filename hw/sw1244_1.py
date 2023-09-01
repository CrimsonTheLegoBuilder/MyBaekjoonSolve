def swap(prize, i, j):
    prize_arr = [0] * prize_len
    for k in range(prize_len-1, -1, -1):
        prize_arr[k] = prize % 10
        prize //= 10
    prize_arr[i], prize_arr[j] = prize_arr[j], prize_arr[i]
    result = 0
    for k in range(prize_len):
        result *= 10
        result += prize_arr[k]
    return result


def find_max(n, k, prize):
    global ans
    for i in range(facto[prize_len]):
        if not memo[k][i]:
            memo[k][i] = prize
            break
        elif memo[k][i] == prize:
            return
    if n == k:
        ans = max(ans, prize)
    else:
        for i in range(prize_len-1):  # nCm, m=2
            for j in range(i+1, prize_len):
                find_max(n, k+1, swap(prize, i, j))


for tc in range(int(input())):
    prize, N = map(int, input().split())
    prize_len = 0
    t = prize
    while t:
        t //= 10
        prize_len += 1
    facto = (1, 1, 2, 6, 24, 120, 720)
    memo = [[0] * facto[prize_len] for _ in range(N+1)]  # 720
    ans = 0
    find_max(N, 0, prize)
    print(f'#{tc+1} {ans}')
