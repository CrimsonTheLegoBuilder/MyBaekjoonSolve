import sys
sys.stdin = open('input_code.txt', 'r')


def scan(r, c):
    idx = 55
    for k in range(c, c-56, -1):
        target[idx] = code[r][k]
        idx -= 1
    return


def find_code():
    for r in range(N):
        for c in range(M-1, -1, -1):
            if code[r][c]:
                return r, c


def solve():
    for c in range(0, 56, 7):
        for x in range(10):
            flag = 1
            for y in range(7):
                if ref[x][y] != target[c + y]:
                    flag = 0
                    break
            if flag:
                verify[c % 2] += x
                break


for tc in range(1, int(input())+1):
    N, M = map(int, input().split())
    code = [list(map(int, list(input()))) for _ in range(N)]
    target = [0] * 56
    verify = [0, 0]
    ref = (
        (0, 0, 0, 1, 1, 0, 1),
        (0, 0, 1, 1, 0, 0, 1),
        (0, 0, 1, 0, 0, 1, 1),
        (0, 1, 1, 1, 1, 0, 1),
        (0, 1, 0, 0, 0, 1, 1),
        (0, 1, 1, 0, 0, 0, 1),
        (0, 1, 0, 1, 1, 1, 1),
        (0, 1, 1, 1, 0, 1, 1),
        (0, 1, 1, 0, 1, 1, 1),
        (0, 0, 0, 1, 0, 1, 1),
    )
    i, j = find_code()
    scan(i, j)
    # print(target)
    # print(verify)
    solve()
    # print(verify)
    a, b = verify
    if not (a*3 + b) % 10:
        print(f'#{tc} {a+b}')
    else:
        print(f'#{tc} 0')
