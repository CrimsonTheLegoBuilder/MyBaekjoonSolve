import sys

N = int(sys.stdin.readline().strip())
K, C = map(int, sys.stdin.readline().strip().split())
cost = tuple(map(int, sys.stdin.readline().strip().split()))
dp = [0] * (N+1)  # dp는 한 칸 늘려서 받는 게 유리할 때가 많다.
k = [0] * (N+1)   # K개를 한꺼번에 가져가는 시점들을 기록할 배열
for i in range(1, K):
    dp[i] = cost[i-1] + dp[i-1]  # K개를 못 가져가는 시점까지는 누적합
for i in range(K, N+1):
    a = dp[i-1] + cost[i-1]  # 이전까지의 최소 비용과 지금 바나나 하나를 가져가는 비용
    b = dp[i-K] + C          # 지금 고른 바나나 포함 K개를 가져가는 비용
    if a > b:                # 지금 하나 가져 가는 것보다 K개 가져가는 게 무조건 이득일 때:
        dp[i] = b            # 전에 K개 가지고 갔어야 이득
        k[i] = k[i-K] + 1    # K개 가져가는 시점이 발생
    else:
        dp[i] = a            # 자금 하나 가져가는 게 이득
        k[i] = k[i-1]        # K개 가져가는 일은 일어나지 않았음

print(dp[-1])  # 일단 최소 비용 출력
cnt, idx = k[-1], k[-1]    # K개 가져가는 이벤트 발생 횟수
print(cnt)     # 출력

ans = [0] * N       # 각각의 이벤트가 일어난 시점을 담을 리스트
while N >= 0 and cnt:
    if k[N] == cnt and k[N-K] == cnt - 1 and dp[N] - dp[N-K] == C:
        # 만일 지금 보고 있는 시점이 K개 가져간 이벤트가 발생한 시점보다 앞인 건 확인했고
        # K개 앞을 보니 1 차이 나서 더 확실한 것 같은데
        # dp도 K개 앞을 보니 누적 비용이 C 차이 나기 까지 한다면?
        ans[cnt] = N - K + 1  # 확실히 답이다.
        N -= K-1           # 건너 뛰어 더 앞을 보자.
        cnt -= 1           # 하나 찾음
    N -= 1                 # 앞으로 간다
for i in range(1, idx):
    print(ans[i], end=' ')
print(ans[idx])


# ans = []       # 각각의 이벤트가 일어난 시점을 담을 리스트
# while N >= -1 and cnt:
#     if k[N] == cnt and k[N-K] == cnt - 1 and dp[N] - dp[N-K] == C:
#         # 만일 지금 보고 있는 시점이 K개 가져간 이벤트가 발생한 시점보다 앞인 건 확인했고
#         # K개 앞을 보니 1 차이 나서 더 확실한 것 같은데
#         # dp도 K개 앞을 보니 누적 비용이 C 차이 나기 까지 한다면?
#         ans.append(N-K+1)  # 확실히 답이다.
#         N -= K-1           # 건너 뛰어 더 앞을 보자.
#         cnt -= 1           # 하나 찾음
#     N -= 1                 # 앞으로 간다
# print(*sorted(ans))

'''
5
3 9
5 3 7 1 8

6
3 9
5 3 7 1 8 2

7
3 9
10 10 10 3 3 3 3
'''