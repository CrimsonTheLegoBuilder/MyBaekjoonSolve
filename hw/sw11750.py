for tc in range(1, int(input())+1):
    N, num = input().split()
    N = int(N)
    arr = ['____'] * 71
    arr[48], arr[49], arr[50], arr[51], arr[52], arr[53], arr[54], arr[55], arr[56], arr[57], arr[65], arr[66], arr[67], arr[68], arr[69], arr[70] = \
        '0000', '0001', '0010', '0011', '0100', '0101', '0110', '0111', '1000', '1001', '1010', '1011', '1100', '1101', '1110', '1111'
    print(f'#{tc}', end=' ')
    for o in num:
        print(arr[ord(o)], end='')
    print()
