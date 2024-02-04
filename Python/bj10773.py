import sys

n = int(input())
stack = []

for _ in range(n):
    a = int(sys.stdin.readline().strip())
    if a != 0:
        stack.append(a)
    else:
        del stack[-1]

print(sum(stack))
