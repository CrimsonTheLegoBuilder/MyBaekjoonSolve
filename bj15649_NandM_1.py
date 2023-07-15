N, M = map(int, input().split())

ans = []


def n_and_m_1():
    if len(ans) == M:
        print(*ans)
    for i in range(1, N + 1):
        if i not in ans:
            ans.append(i)
            n_and_m_1()
            ans.pop()


n_and_m_1()
