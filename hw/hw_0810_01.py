dp=[1]*31
for i in range(1,31):dp[i]=dp[i-1]*2+1-2*(i%2)
for tc in range(1, int(input())+1):print(f'#{tc} {dp[int(input())//10]}')
