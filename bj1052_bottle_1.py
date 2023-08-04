n, k = map(int, input().split())
cur, ans = 1, 0
while True:
    temp, cnt = n, 0
    while temp > 0:
        if temp & 1: cnt += 1
        temp >>= 1
    if cnt <= k: print(ans); exit(0)
    if n & cur: n += cur; ans += cur
    cur <<= 1
    