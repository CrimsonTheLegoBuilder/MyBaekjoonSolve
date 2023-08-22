for tc in range(1, int(input())+1):
    N = int(input())
    ans = ['_'] * (N+1)
    cards = list(input().split())+['0']
    for i in range((N+1)//2):
        ans[i*2] = cards[i]
        ans[i*2+1] = cards[(N+1)//2+i]
    ans.pop()
    print(f'#{tc}', *ans)

'''
4
6
A B C D E F
4
JACK QUEEN KING ACE
5
ALAKIR ALEXSTRASZA DR-BOOM LORD-JARAXXUS AVIANA
7
ALAKIR ALEXSTRASZA DR-BOOM LORD-JARAXXUS AVIANA FUCK SUCK
'''