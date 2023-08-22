for t in range(1,int(input())+1):
 N=int(input())
 ans=[0]*8
 m=50000
 d=5,2
 for i in range(8):
  ans[i]=N//m
  N%=m
  m//=d[i%2]
 print(f'#{t}')
 print(*ans)
