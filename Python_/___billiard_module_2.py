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
    flag = dot(d1, d3, d3, d2) > 0
    return dist < D*2 and flag


def one_cushion(d1):
    pt = []
    # for h_ in HOLES:
    return


def hit_pos(d1):
    coord = []
    for h_ in HOLES:
        vx_, vy_ = make_vector(h_, d1)
        length = cal_length(d1, h_)
        x, y = d1
        x1 = x + vx_*(D/length)
        y1 = y + vy_*(D/length)
        flag = 1
        for l1, l2 in boundary:
            if cal_dist(l1, l2, d1) < D:
                flag = 0
        for i in range(1, 6):
            if dot((x1, y1), balls[i], balls[i], h_) > 0 and is_collide((x1, y1), h_, balls[i]) < D+0.01:
                flag = 0
        if flag:
            coord.append((x1, y1))
    for i in range(6):
        pass
    return coord


def is_cross(d1, d2, d3, d4):
    flag1 = cross(d1, d2, d2, d3) * cross(d2, d1, d1, d4)
    flag2 = cross(d3, d4, d4, d1) * cross(d4, d3, d3, d2)
    return flag1 > 0 and flag2 > 0


def crs_pos(d1, d2):
    d_ = d1
    for i in range(6):
        d3, d4 = boundary[i]
        if is_cross(d1, d2, d3, d4):
            if d3[0] - d4[0] == 0 and d1[1] - d2[1] == 0:
                x = d3[0]
                y = d1[1]
                d_ = x, y
            elif d1[0] - d2[0] == 0 and d3[1] - d4[1] == 0:
                x = d1[0]
                y = d3[1]
                d_ = x, y
            elif d3[0] - d4[0] == 0:
                x = d3[0]
                y = d1[1] + (d1[1] - d2[1]) * (d3[0] - d1[0]) / (d1[0] - d2[0])
                d_ = x, y
            elif d3[1] - d4[1] == 0:
                x = d1[0] + (d1[0] - d2[0]) * (d3[1] - d1[1]) / (d1[1] - d2[1])
                y = d3[1]
                d_ = x, y
            if dot(d1, d_, d_, d2) > 0:
                return d_
    return


def one_cushion_t2h(j):
    candidate = []
    d1 = balls[j]
    x, y = d1
    d2 = 508 - x - D, -y
    d_ = crs_pos(d1, d2)
    flag = 1
    if d_ is not None:
        for i in range(1, 6):
            if i != j:
                if is_collide(d1, d_, balls[i]) and is_collide((0, 0), d_, balls[i]):
                    flag = 0
    if flag:
        vx, vy = make_vector(d2, d1)
        l_ = cal_length(d2, d1) ** .5
        d__ = x + vx*(D/l_), y + vy*(D/l_)
        candidate.append(d__)
    d2 = -x, 254 - y - D
    d_ = crs_pos(d1, d2)
    flag = 1
    if d_ is not None:
        for i in range(1, 6):
            if i != j:
                if is_collide(d1, d_, balls[i]) and is_collide((0, 0), d_, balls[i]):
                    flag = 0
    if flag:
        vx, vy = make_vector(d2, d1)
        l_ = cal_length(d2, d1) ** .5
        d__ = x + vx*(D/l_), y + vy*(D/l_)
        candidate.append(d__)

    return


N = 6
HOLES = ((0, 0), (127, 0), (254, 0), (0, 127), (127, 127), (254, 127))
target = ((0,), (1, 3, 5), (2, 4, 5))
player = 0

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
            # print(h)
            vx2, vy2 = make_vector(t, h)
            l_ = cal_length(t, h)**.5
            x_, y_ = t
            x1_, y1_ = x_ - vx2*(D/l_), y_ - vy2*(D/l_)
            x1, y1 = x1_ - x, y1_ - y
            angle = degrees(atan2(y1, x1))
            t_ = x1, y1
            l = cal_length(s, t_)
            v = cross(s, t_, t_, h)
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
for i in target[player][1:]:
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
