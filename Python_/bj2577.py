import sys
A = int(sys.stdin.readline().strip())
B = int(sys.stdin.readline().strip())
C = int(sys.stdin.readline().strip())
D = A*B*C
ans = [0]*10
while D:
    ans[D % 10] += 1
    D //= 10
# for i in ans:
#     print(i)
