def f(n):return 1 if n==1 else f(n-1)*2+1-2*(n%2)
for t in range(1,int(input())+1):print(f'#{t}',f(int(input())//10))
# t=int(input());i=0
# while i<t:i+=1;print(f'#{i}',f(int(input())//10))
# def paper(n):
#     return 1 if n == 0 else paper(n - 1) * 2 + 1 - 2 * (n % 2)
# # 초항은 n = 0 일 때 1, 다음부터는 재귀 돌입, 삼항연산자로 줄인 재귀함수.
# # 홀수번 째 수는 전번째 수의 2배 -1, 짝수번 째 수는 전번째 수의 2배 +1이라는 규칙이 있음.
# # 이는 해당 칸의 패턴의 경우의 수에 1줄을 추가한 후 만들어 낼 수 있는 새로문 패턴의 경우의 수를 더한 것임.
#
#
# for tc in range(1, int(input()) + 1):
#     print(f'#{tc} {paper(int(input()) // 10)}')
#
# dp = [1] * 31  # 초항은 1
#
# for i in range(1, 31):
#     dp[i] = dp[i - 1] * 2 + 1 - 2 * (i % 2)
#
# # 동적 계획법으로도 풀이 가능
# # for tc in range(1, int(input()) + 1):
# #     print(f'#{tc} {dp[int(input()) // 10]}')
