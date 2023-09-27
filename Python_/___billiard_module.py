from math import atan, atan2, degrees


# def cross(d1, d2, d3, d4):
#     return (d2[0]-d1[0])*(d4[1]-d3[1]) - (d2[1]-d1[1])*(d4[0]-d3[0])


def dot(d1, d2, d3, d4):
    return (d2[0]-d1[0])*(d4[0]-d3[0]) + (d2[1]-d1[1])*(d4[1]-d3[1])


def make_vector(d1, d2):
    return d2[0]-d1[0], d2[1]-d1[1]


def cal_length(d1, d2):
    return (d2[0]-d1[0])**2 + (d2[1]-d1[1])**2


N = 6  # 모든 공의 수
HOLES = ((0, 0), (127, 0), (254, 0), (0, 127), (127, 127), (254, 127))
balls = ((127.0, 63.5), (127.0, 100.0))
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
            ans.append(angle)
print(*ans)

TABLE_WIDTH = 254
TABLE_HEIGHT = 127
NUMBER_OF_BALLS = 6
