import sys

t = int(sys.stdin.readline().strip())
for _ in range(t):
    x, y = map(int, sys.stdin.readline().strip().split())
    d = y - x
    b = int((d-1) ** 0.5)

    if d <= (b**2 + (b+1)**2)//2:
        print(b * 2)
    else:
        print(b * 2 + 1)

    c = 0
    while True:
        if d <= c * (c + 1):
            break
        c += 1

    if d <= c ** 2:
        print(c * 2 - 1)
    else:
        print(c * 2)

# powered = [0]
# t = 1
# for i in range(1, 30):
#     for _ in range(i):
#         powered.append(t)
#     t += 1
#     for _ in range(i):
#         powered.append(t)
#     t += 1
# print(powered[y - x])
