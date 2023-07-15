N, M = map(int, input().split())

ans = []


def n_and_m_3():
    if len(ans) == M:
        print(*ans)
        return
    # if len(ans) < M:
    for i in range(1, N + 1):
        ans.append(i)
        n_and_m_3()
        ans.pop()


n_and_m_3()
