
def convert(x, samjunsu):
    leetoten = int(x, 2)

    lst3 = []
    while True:
        lst3.append(str(leetoten % 3))
        leetoten //= 3
        if leetoten == 0 or leetoten == 1 or leetoten == 2:
            lst3.append(str(leetoten))
            break

    lst3.reverse()
    while len(lst3) != len(samjunsu):
        lst3.insert(0, '0')

    cnt = 0
    for j in range(len(lst3)):
        if lst3[j] != samjunsu[j]:
            cnt += 1

    if cnt == 1:
        return ''.join(lst3)

T = int(input())
for tc in range(1, T + 1):
    leejunsu = list(input())
    samjunsu = list(input())

    result = 0
    for i in range(len(leejunsu)):
        if leejunsu[i] == '1':
            leejunsu[i] = '0'
            a = convert(''.join(leejunsu), samjunsu)
            if a:
                result = int(a, 3)
            leejunsu[i] = '1'
        else:
            leejunsu[i] = '1'
            a = convert(''.join(leejunsu), samjunsu)
            if a:
                result = int(a, 3)
            leejunsu[i] = '0'

    print(f'#{tc} {result}')