import sys
sys.stdin = open('input_carp.txt','r')


for tc in range(1, int(input())+1):
    N, M, K = map(int, input().split())
    guest = list(map(int, input().split()))
    guest.sort()
    ans = 'Possible'
    for i in range(len(guest)):
        if not guest[i] or (guest[i] // M) * K < (i + 1):
            ans = 'Impossible'
    print(f'#{tc} {ans}')
    # carp = [0]*11112
    # wait = [0]*11112
    # cnt = 0
    # for i in range(1, 11112):
    #     if not i % M:
    #         cnt += K
    #     carp[i] += cnt
    # for p in guest:
    #     for i in range(p, 11112):
    #         wait[i] += 1
    # ans = 'Possible'
    # for i in range(11112):
    #     if carp[i] < wait[i]:
    #         ans = 'Impossible'
    # # print(carp)
    # # print(wait)
    # print(f'#{tc} {ans}')

