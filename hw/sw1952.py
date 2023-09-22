def dfs(depth, t):
    global total
    if depth >= 12:
        total = min(total, t)
        return
    if t > total:
        return
    dfs(depth+1, t + M[depth] * P[0])
    dfs(depth+1, t + P[1])
    dfs(depth+3, t + P[2])
    # dfs(depth+12, t + P[3])


for tc in range(int(input())):
    P = tuple(map(int, input().split()))
    M = tuple(map(int, input().split()))
    total = P[3]
    dfs(0, 0)
    print(f"#{tc+1} {total}")
