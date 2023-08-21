def max_(a, b): return a if a >= b else b  # 급조한 max() 함수


for tc in range(1, int(input())+1):
    N = int(input())
    room = [0]*200  # 방은 1번부터 400번까지 있음. 좌표 압축!
    for _ in range(N):
        s, e = map(int, input().split())  # 술 처먹고 출발할 방, 가야할 방
        if s > e:
            s, e = e, s
        for i in range((s-1)//2, (e-1)//2+1):
            room[i] += 1  # 거쳐가는 길들에 +1
    ans = 0
    for i in room:
        ans = max_(ans, i)  # 수집
    print(f'#{tc} {ans}')
