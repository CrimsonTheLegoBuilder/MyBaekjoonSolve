N, M = map(int, input().split())

ans = []


def n_and_m_4(n):
    if len(ans) == M:
        print(*ans)
        return
    # if len(ans) < M:
    for i in range(n, N + 1):
        ans.append(i)
        n_and_m_4(i)
        ans.pop()


n_and_m_4(1)
