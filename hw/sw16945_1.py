for tc in range(1, int(input())+1):
    a1, a2, a3, a4, a5, a6 = sorted(list(map(int, input())))
    flag = 0
    if a2 == a5 and a1+1 == a2 == a6-1:
        flag = 1
    if a3 == a4 and a1+2 == a2+1 == a3 == a5-1 == a6-2:
        flag = 1
    if a1 == a2 == a3 and a4 == a5 == a6:
        flag = 1
    if a1+2 == a2+1 == a3 and a4 == a5 == a6:
        flag = 1
    if a1 == a2 == a3 and a4+2 == a5+1 == a6:
        flag = 1
    if a1+2 == a2+1 == a3 and a4+2 == a5+1 == a6:
        flag = 1
    if a1+2 == a3+1 == a5 and a1 == a2 and a3 == a4 and a5 == a6:
        flag = 1

    print(f'#{tc} {flag}')
