import sys
sys.stdin = open('sample_code_2.txt', 'r')

ref = {
    (2, 1, 1): 0,
    (2, 2, 1): 1,
    (1, 2, 2): 2,
    (4, 1, 1): 3,
    (1, 3, 2): 4,
    (2, 3, 1): 5,
    (1, 1, 4): 6,
    (3, 1, 2): 7,
    (2, 1, 3): 8,
    (1, 1, 2): 9
}
h_to_b = {
    '0': '0000', '1': '0001', '2': '0010', '3': '0011', '4': '0100', '5': '0101', '6': '0110', '7': '0111',
    '8': '1000', '9': '1001', 'A': '1010', 'B': '1011', 'C': '1100', 'D': '1101', 'E': '1110', 'F': '1111'
}


def solve():
    global ans
    for x in range(1, N):
        y = 4*M - 1
        while y >= 0:
            y -= 1
            if conv[x-1][y] == '0' and conv[x][y] == '1':
                cnt = 0
                security = [0, 0]
                while cnt < 8:
                    cnt4, cnt3, cnt2 = 0, 0, 0
                    while conv[x][y] == '1':
                        cnt4 += 1
                        y -= 1
                    while conv[x][y] == '0':
                        cnt3 += 1
                        y -= 1
                    while conv[x][y] == '1':
                        cnt2 += 1
                        y -= 1
                    cnt += 1
                    while conv[x][y] == '0':
                        if cnt > 7:
                            break
                        y -= 1
                    m = min(cnt2, cnt3, cnt4)
                    security[cnt % 2] += ref[(cnt2//m, cnt3//m, cnt4//m)]
                # print(security)
                a, b = security
                sec_key = a*3 + b
                if not sec_key % 10:
                    ans += a+b


for tc in range(1, int(input())+1):
    N, M = map(int, input().strip().split())
    code = [list(map(str, input())) for _ in range(N)]
    conv = []
    for i in range(N):
        temp = ''
        for j in range(M):
            temp += h_to_b[code[i][j]]
        conv.append(temp)
    # print(conv)
    ans = 0
    solve()
    print(f'#{tc} {ans}')
