pow3 = [3**i for i in range(40)]

for tc in range(1, int(input())+1):
    A = list(map(int, reversed(input())))
    B = list(map(int, input()))

    a = 0
    for i in range(len(A)):
        if A[i]:
            a |= 1 << i

    ans = 0
    for i in range(len(A)):
        a ^= 1 << i
        for j in range(len(B)):
            b1, b2 = 0, 0
            for k in range(len(B)):
                if j == k:
                    b1 = b1 * 3 + (B[k] + 1) % 3
                    b2 = b2 * 3 + (B[k] + 2) % 3
                else:
                    b1 = b1 * 3 + B[k]
                    b2 = b2 * 3 + B[k]
            if a == b1 or a == b2:
                ans = a
        a ^= 1 << i

    print(f'#{tc} {ans}')
