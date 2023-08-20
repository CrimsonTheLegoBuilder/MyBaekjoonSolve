import sys

N = int(sys.stdin.readline().strip())
stack = []
ans = [0] * N
towers = tuple(map(int, sys.stdin.readline().strip().split()))
for i in range(N-1, 0, -1):
    temp = towers[i]
    while stack and stack[-1][0] < temp:
        h, idx = stack.pop()
        ans[idx] = i+1
    stack.append((temp, i))
    print(ans)
    print(stack)
print(ans)
