import sys

n, m = map(int, input().split())

a = set(sys.stdin.readline().strip().split())

b = set(sys.stdin.readline().strip().split())

c = (a | b) - (a & b)

# print(c)
print(len(c))

