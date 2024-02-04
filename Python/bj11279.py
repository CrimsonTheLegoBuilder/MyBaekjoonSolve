import sys


def h_push(o):
    global bottom
    bottom += 1
    pq[bottom] = o
    c = bottom
    p = c >> 1
    while p:
        if pq[p] < pq[c]:
            pq[p], pq[c] = pq[c], pq[p]
        else:
            break
        p >>= 1
        c >>= 1


def h_pop():
    global bottom
    if not bottom:
        return 0
    tmp = pq[1]
    pq[1] = pq[bottom]
    pq[bottom] = 0
    bottom -= 1
    p = 1
    while p <= bottom:
        m = p
        l, r = m << 1, (m << 1) | 1
        if l <= bottom and pq[m] < pq[l]:
            m = l
        if r <= bottom and pq[m] < pq[r]:
            m = r
        if p ^ m:
            pq[p], pq[m] = pq[m], pq[p]
            p = m
        else:
            break
    return tmp


N = int(sys.stdin.readline().strip())
bottom = 0
pq = [0]*100_001
for _ in range(N):
    x = int(sys.stdin.readline().strip())
    if x:
        h_push(x)
    else:
        print(h_pop())
