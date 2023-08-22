import sys

N = int(sys.stdin.readline().strip())
stack = []
ans = [0] * N
towers = tuple(map(int, sys.stdin.readline().strip().split()))
for i in range(N-1, -1, -1):
    height = towers[i]
    while stack and stack[-1][0] < height:
        h, idx = stack.pop()
        ans[idx] = i+1
    stack.append((height, i))
    # print(ans)
    # print(stack)
print(*ans)

'''
6
9 4 6 3 7 8  # 0 1 1 3 2 2
'''