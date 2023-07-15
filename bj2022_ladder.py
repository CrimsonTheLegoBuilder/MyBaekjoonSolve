import sys

x, y, c = map(float, sys.stdin.readline().strip().split())
right = min(x, y)
left = 0

while abs(right - left) > 0.000001:
    D = (left + right) / 2.0
    h1 = (x ** 2 - D ** 2) ** 0.5
    h2 = (y ** 2 - D ** 2) ** 0.5
    h = (h1 * h2) / (h1 + h2)
    if h > c:
        left = D
    else:
        right = D

sys.stdout.write(f'{right:.5}\n')
