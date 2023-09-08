from math import atan, atan2, degrees


def cross(d1, d2, d3, d4):
    return (d2[0]-d1[0])*(d4[1]-d3[1]) - (d2[1]-d1[1])*(d4[0]-d3[0])


def dot(d1, d2, d3, d4):
    return (d2[0]-d1[0])*(d4[0]-d3[0]) + (d2[1]-d1[1])*(d4[1]-d3[1])


def make_vector(d1, d2):
    return d2[0]-d1[0], d2[1]-d1[1]


def cal_length(d1, d2):
    return (d2[0]-d1[0])**2 + (d2[1]-d1[1])**2


def cal_dist(d1, d2, d3):
    torque = abs(cross(d1, d2, d2, d3))
    length = cal_length(d1, d2)**.5
    return torque / length


def is_collide(d1, d2, d3):
    dist = cal_dist(d1, d2, d3)
    return dist < D*2


def one_bounce(d1):
    pt = []
    # for h_ in HOLES:


N = 6
HOLES = ((0, 0), (127, 0), (254, 0), (0, 127), (127, 127), (254, 127))
target = ((0,), (1, 3, 5), (2, 4, 5))

r = 2.865
boundary = (((5.73, 2.865), (121.27, 2.865)), ((132.73, 2.865), (248.27, 2.865)), ((251.135, 5.73), (251.135, 121.27)),
            ((5.73, 121.27), (121.27, 121.27)), ((132.73, 121.27), (248.27, 121.27)), ((2.865, 5.73), (2.865, 121.27)))
balls = ((127.0, 63.5), (180.0, 100.0), (127.0, 100.0), (100.0, 50.0), (50.0, 25.0), (95.0, 30.0))
D = 5.73
s = balls[0]
x, y = s
ans = []
for i in range(1, len(balls)):
    t = balls[i]
    for h in HOLES:
        if dot(s, t, t, h) > 0:
            print(h)
            vx2, vy2 = make_vector(t, h)
            l_ = cal_length(t, h)**.5
            x_, y_ = t
            x1_, y1_ = x_ - vx2*(D/l_), y_ - vy2*(D/l_)
            x1, y1 = x1_ - x, y1_ - y
            angle = degrees(atan2(y1, x1))
            l = cal_length(s, t)
            v = cross(s, t, t, h)
            theta = abs(atan(v/l))
            ans.append((theta, angle))

ans.sort()
rt = ans[0][1]
print(*ans)
print(rt)

# ==================================================================================================================== #

s = balls[0]
x, y = s
player = 0
for i in target[player][:2]:
    t = balls[i]
    for h in HOLES:
        if dot(s, t, t, h) > 0:
            print(h)
            vx2, vy2 = make_vector(t, h)
            l_ = cal_length(t, h)**.5
            x_, y_ = t
            x1_, y1_ = x_ - vx2*(D/l_), y_ - vy2*(D/l_)
            x1, y1 = x1_ - x, y1_ - y
            angle = degrees(atan2(y1, x1))
            l = cal_length(s, t)
            v = cross(s, t, t, h)
            theta = abs(atan(v/l))
            ans.append((theta, angle))


TABLE_WIDTH = 254
TABLE_HEIGHT = 127
NUMBER_OF_BALLS = 6
