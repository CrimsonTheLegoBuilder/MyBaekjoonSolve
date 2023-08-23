for tc in range(1, int(input())+1):
    num = float(input())
    ans = [0] * 12
    d = 1
    for i in range(12):
        d <<= 1
        if num - d**-1 >= 0.0:
            num -= d**-1
            ans[i] = 1
    if num > 0.0000000000001:
        print(f'#{tc} overflow')
    else:
        while not ans[-1]:
            ans.pop()
        ans = ''.join(map(str, ans))
        print(f'#{tc} {ans}')

'''
3
0.625
0.1
0.125
'''