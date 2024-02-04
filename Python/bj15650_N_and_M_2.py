N, M = map(int, input().split())

ans = []


def n_and_m_2(n):
    if len(ans) == M:
        print(*ans)
    for i in range(n, N + 1):
    #    if i not in ans:
            ans.append(i)
            n_and_m_2(i + 1)
            ans.pop()


n_and_m_2(1)
