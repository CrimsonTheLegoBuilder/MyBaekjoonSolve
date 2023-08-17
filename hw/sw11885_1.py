for tc in range(1,int(input())+1):
 n,m=map(int,input().split())
 a=list(input().split())
 print(f'#{tc} {a[m%n]}')