import sys


def comp(d1, d2):
    return d1[0] > d2[0] or (d1[0] == d2[0] and d1[1] > d2[1])


def h_pop():
    global bottom
    if not bottom:
        return 0, 0
    tmp = pq[1]
    # print(tmp, 'pop')
    root = pq[bottom]
    bottom -= 1
    pq[1] = root
    p = 1
    while p <= bottom:
        m = p
        l, r = m << 1, (m << 1) | 1
        if l <= bottom and comp(pq[m], pq[l]):
            m = l
        if r <= bottom and comp(pq[m], pq[r]):
            m = r
        if m ^ p:
            pq[p], pq[m] = pq[m], pq[p]
            p = m
        else:
            break
    return tmp


def h_push(o):
    global bottom
    bottom += 1
    pq[bottom] = o
    c = bottom
    p = c >> 1
    while p:
        if comp(pq[p], pq[c]):
            pq[p], pq[c] = pq[c], pq[p]
        else:
            break
        c >>= 1
        p >>= 1


N = int(sys.stdin.readline().strip())
pq = [(0, 0) for _ in range(100_001)]
bottom = 0
for _ in range(N):
    x = int(sys.stdin.readline().strip())
    if x:
        h_push((abs(x), x))
    else:
        print(h_pop()[1])
    # print(pq[:bottom])
    # print(pq[1], 1)
    # print(pq[bottom], bottom)
    # print()
