for tc in range(1, int(input())+1):
    card = list(map(int, input().split()))
    deq1 = [0] * 10
    deq2 = [0] * 10
    win1 = 0
    win2 = 0
    for j in range(12):
        deq1[card[j]] += (j % 2) ^ 1
        deq2[card[j]] += j % 2
        for i in range(10):
            if i < 8:
                if deq1[i] and deq1[i+1] and deq1[i+2]:
                    win1 = 1
                if deq2[i] and deq2[i+1] and deq2[i+2]:
                    win2 = 2
            if deq1[i] == 3:
                win1 = 1
            if deq2[i] == 3:
                win2 = 2
        if win1 or win2:
            ans = (win1 & 1) | (win2 & 2)
            print(f'#{tc} {ans}')
            break
    if not win1 and not win2:
        print(f'#{tc} 0')
