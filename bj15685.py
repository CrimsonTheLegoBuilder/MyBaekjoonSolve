import sys


def rotate_ccw(x, y, line):
    tmp = []
    for x1, y1 in reversed(line[:-1]):
        vx, vy = x - x1, y - y1
        x2, y2 = x - vy, y + vx
        tmp.append((x2, y2))
    x_, y_ = tmp[-1]
    line_ = line + tmp
    return x_, y_, line_


def make_curve(x, y, d, g):
    line = [(x, y)]
    for k in range(g+1):
        if not k:
            line += [(x+dxy[d][0], y+dxy[d][1])]
            x, y = line[-1]
        else:
            x, y, line = rotate_ccw(x, y, line)
    return line


N = int(sys.stdin.readline().strip())
info = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
dots = {}
dxy = ((1, 0), (0, -1), (-1, 0), (0, 1))
for order in info:
    xs, ys, ds, gs = order
    dragon = make_curve(xs, ys, ds, gs)
    print(dragon)
    for i, j in dragon:
        dots.setdefault((i, j), 1)
cnt = 0
for i, j in dots.keys():
    if dots.get((i+1, j)) and dots.get((i, j+1)) and dots.get((i+1, j+1)):
        cnt += 1
print(dots)
print(cnt)

