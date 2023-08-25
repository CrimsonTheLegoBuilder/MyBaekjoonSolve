from collections import deque


def scan(order, s, e):
    total = 0
    d = 1
    for x in range(e-1, s-1, -1):
        total += order[x]*d
        d *= 16
    return total


for tc in range(1, int(input())+1):
    N, K = map(int, input().split())
    lid = deque(list(map(lambda x: int(x, 16), input())))
    side = N//4
    ans = []
    for _ in range(side):  # side*(K//side > 0) + (K+1)*(K//side == 0) % side):
        # print(lid)
        for j in range(4):
            ans.append(scan(lid, j*side, (j+1)*side))
        lid.rotate()
    # print(ans)
    max_ = (sorted(list(set(ans)), reverse=True)[K-1])
    print(f'#{tc} {max_}')
