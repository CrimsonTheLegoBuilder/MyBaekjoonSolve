deq = (7, 11, 13, 19, 21, 25, 35, 37, 41, 49)

for tc in range(1, int(input())+1):
    card = list(map(int, input()))
    ans = 0
    for i in deq:
        deq1 = []
        deq2 = []
        for j in range(6):
            if i & 1 << j:
                deq1.append(card[j])
            else:
                deq2.append(card[j])
        # print(deq1)
        # print(deq2)
        a1, a2, a3 = sorted(deq1)
        b1, b2, b3 = sorted(deq2)
        flag1, flag2 = 0, 0
        if a1 == a2 == a3 or a1+1 == a2 == a3-1:
            flag1 = 1
        if b1 == b2 == b3 or b1+1 == b2 == b3-1:
            flag2 = 1
        if flag1 & flag2:
            ans |= 1
    print(f'#{tc} {ans}')
