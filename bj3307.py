import sys


N = int(sys.stdin.readline().strip())
balloons = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
stack = [(balloons[0][0], (float(balloons[0][1])))]
ans = [stack[0][1]] + [0.0] * (N-1)
for i in range(1, N):
    r = float(balloons[i][1])
    while stack:
        d = balloons[i][0] - stack[-1][0]
        print(d)
        if r < stack[-1][1]:
            if 2*(balloons[i][1] * stack[-1][1])**.5 <= d:
                print(r)
                stack.append((balloons[i][0], r))
                break
            else:
                r = d**2 / (4*stack[-1][1])
                stack.append((balloons[i][0], r))
                break
        else:
            if 2*(balloons[i][1] * stack[-1][1])**.5 <= d:
                stack.pop()
            else:
                r = d**2 / (4*stack[-1][1])
                stack.pop()
        print(stack)
    print(stack)
    ans[i] = stack[-1][1]
print(*ans)
