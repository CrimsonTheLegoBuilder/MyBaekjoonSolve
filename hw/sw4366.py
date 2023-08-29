pow2 = [1 << i for i in range(40)]
pow3 = [3**i for i in range(40)]

for tc in range(1, int(input())+1):
    A = list(map(int, reversed(input())))
    B = list(map(int, reversed(input())))

    a = 0
    for i in range(len(A)):
        a += pow2[i] * A[i]
    b = 0
    for i in range(len(B)):
        b += pow3[i] * B[i]
    # print(a, b)
    ans = 0

    for i in range(len(A)):
        temp_a = a ^ pow2[i]
        for j in range(len(B)):
            temp_b = b
            if not B[j]:
                temp_b += pow3[j]
                if temp_a == temp_b:
                    ans = temp_a
                temp_b += pow3[j]
                if temp_a == temp_b:
                    ans = temp_a
            elif B[j] == 1:
                temp_b -= pow3[j]
                if temp_a == temp_b:
                    ans = temp_a
                temp_b += 2*pow3[j]
                if temp_a == temp_b:
                    ans = temp_a
            elif B[j] == 2:
                temp_b -= pow3[j]
                if temp_a == temp_b:
                    ans = temp_a
                temp_b -= pow3[j]
                if temp_a == temp_b:
                    ans = temp_a
    print(f'#{tc} {ans}')
