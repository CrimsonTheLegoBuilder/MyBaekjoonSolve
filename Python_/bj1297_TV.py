import sys

D, H, W = map(int, sys.stdin.readline().strip().split())

ratio = D / (H ** 2 + W ** 2) ** 0.5

print(int(H * ratio), int(W * ratio))
