import sys


N = int(sys.stdin.readline().strip())
balloons = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
stack = [(balloons[0][0], (float(balloons[0][1])))]
ans = [stack[0][1]] + [0.0] * (N-1)
for i in range(1, N):
    r = float(balloons[i][1])
    while stack:
        d = balloons[i][0] - stack[-1][0]
        # print(d)
        if r < stack[-1][1]:
            if 2*(r * stack[-1][1])**.5 <= d:
                break
            else:
                r = d**2 / (4*stack[-1][1])
                break
        else:
            if 2*(r * stack[-1][1])**.5 <= d:
                stack.pop()
            else:
                r = d**2 / (4*stack[-1][1])
        # print(stack)
    stack.append((balloons[i][0], r))
    # print(stack)
    ans[i] = stack[-1][1]
for row in ans:
    print(row)

'''
3
0 9
8 1
13 7

2
8 1
13 7
'''