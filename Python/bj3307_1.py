import sys


N = int(sys.stdin.readline().strip())
balloons = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
stack = [(balloons[0][0], (float(balloons[0][1])))]
ans = [stack[0][1]] + [0.0] * (N-1)
for i in range(1, N):
    r = float(balloons[i][1])
    # while stack:
    while stack:
        d = balloons[i][0] - stack[-1][0]
        # print(d)
        if r < stack[-1][1]:  # 최후에 불었던 풍선보다 지금 부는 풍선의 반지름이 작을 때
            if 2*(r * stack[-1][1])**.5 <= d:  # 불어봤는데 다 불 수 있는 경우:
                break
            else:
                r = d**2 / (4*stack[-1][1])  # 불어봤는데 부딪히면 이게 최대 크기임
                break

        else:    # 최후에 불었던 풍선보다 지금 부는 풍선의 반지름이 클 때
            if 2*(r * stack[-1][1])**.5 <= d:  # 불어봤는데 다 불 수 있는 경우:
                stack.pop()
            else:
                r = d**2 / (4*stack[-1][1])    # 불어봤는데 부딪히면 이게 최대 크기임. 더 작을 수도 있고 더 클 수도 있음.
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