# import sys
#
# for i in range(1, int(sys.stdin.readline().strip())+1):
#     F, N = map(float, sys.stdin.readline().strip().split())
#     print((1.0+F/2)**N)


for i in range(int(input())):F,N=input().split();print(f'Case #{i+1}: {(1+float(F)/2)**int(N)}')
