for tc in range(1, int(input())+1):
    card = list(map(int, input().split()))
    deq1 = [0] * 10
    deq2 = [0] * 10
    for i in range(3):
        deq1[card[2*i]] += 1
        deq2[card[2*i+1]] += 1
    win1 = 0
    win2 = 0
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
        ans = ((win1 & 1) | (win2 & 2)) % 3
        print(f'#{tc} {ans}')
        continue
    deq1[card[6]] += 1
    deq2[card[7]] += 1
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
        ans = ((win1 & 1) | (win2 & 2)) % 3
        print(f'#{tc} {ans}')
        continue
    deq1[card[8]] += 1
    deq2[card[9]] += 1
    for i in range(10):
        if i < 8:
            if deq1[i] and deq1[i + 1] and deq1[i + 2]:
                win1 = 1
            if deq2[i] and deq2[i + 1] and deq2[i + 2]:
                win2 = 2
        if deq1[i] == 3:
            win1 = 1
        if deq2[i] == 3:
            win2 = 2
    if win1 or win2:
        ans = ((win1 & 1) | (win2 & 2)) % 3
        print(f'#{tc} {ans}')
        continue
    deq1[card[10]] += 1
    deq2[card[11]] += 1
    for i in range(10):
        if i < 8:
            if deq1[i] and deq1[i + 1] and deq1[i + 2]:
                win1 = 1
            if deq2[i] and deq2[i + 1] and deq2[i + 2]:
                win2 = 2
        if deq1[i] == 3:
            win1 = 1
        if deq2[i] == 3:
            win2 = 2
    if win1 or win2:
        ans = ((win1 & 1) | (win2 & 2)) % 3
        print(f'#{tc} {ans}')
        continue
    if not win1 and not win2:
        print(f'#{tc} 0')
