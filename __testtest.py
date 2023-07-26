import sys


def cross_4(a1, a2, a3, a4):  # cross product / CCW / line-segment intersection / for 4 dots
    v1 = (a2[0] - a1[0], a2[1] - a1[1])
    v2 = (a4[0] - a3[0], a4[1] - a3[1])
    return v1[0] * v2[1] - v1[1] * v2[0]


def cross_3(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def across_dots(hull1, hull2):
    cross_dots = []
    if hull1[0] == hull2[0] or hull1[0] == hull2[1]:
        d1 = hull1[0]
        cross_dots.append(d1)
        if hull1[1] == hull2[0] or hull1[1] == hull2[1]:
            d2 = hull1[1]
            cross_dots.append(d2)
        return cross_dots
    if cross_3(hull1[0], hull1[1], hull2[0]) * cross_3(hull1[1], hull1[0], hull2[1]) > 0 and \
            cross_3(hull2[0], hull2[1], hull1[0]) * cross_3(hull2[1], hull2[0], hull1[1]) > 0:
        if hull1[0][0] - hull1[1][0] == 0 and hull2[0][1] - hull2[1][1] == 0:  # prevent ZeroDivisionError
            x = hull1[0][0]
            y = hull2[0][1]
        elif hull2[0][0] - hull2[1][0] == 0 and hull1[0][1] - hull1[1][1] == 0:
            x = hull2[0][0]
            y = hull1[0][1]
        elif hull1[0][0] - hull1[1][0] == 0:
            x = hull1[0][0]
            y = hull2[0][1] + (hull1[0][0] - hull2[0][0]) * (hull2[0][1] - hull2[1][1]) / (hull2[0][0] - hull2[1][0])
        elif hull1[0][1] - hull1[1][1] == 0:
            x = hull2[0][0] + (hull1[0][1] - hull2[0][1]) * (hull2[0][0] - hull2[0][0]) / (hull2[0][1] - hull2[1][1])
            y = hull1[0][1]
        elif hull2[0][0] - hull2[1][0] == 0:
            x = hull2[0][0]
            y = hull1[0][1] + (hull2[0][0] - hull1[0][0]) * (hull1[0][1] - hull1[1][1]) / (hull1[0][0] - hull1[1][0])
        elif hull2[0][1] - hull2[1][1] == 0:
            x = hull1[0][0] + (hull2[0][1] - hull1[0][1]) * (hull1[0][0] - hull1[1][0]) / (hull1[0][1] - hull1[1][1])
            y = hull2[0][1]
        else:
            a = (hull1[0][1] - hull1[1][1]) / (hull1[0][0] - hull1[1][0])
            b = (hull2[0][1] - hull2[1][1]) / (hull2[0][0] - hull2[1][0])
            c = (hull1[0][0] - hull1[1][0]) / (hull1[0][1] - hull1[1][1])
            d = (hull2[0][0] - hull2[1][0]) / (hull2[0][1] - hull2[1][1])

            x = (a * hull1[0][0] - b * hull2[0][0] - hull1[0][1] + hull2[0][1]) / (a - b)
            y = (hull2[0][0] - hull1[0][0] - d * hull2[0][1] + c * hull1[0][1]) / (c - d)
        return x, y
    return None


print(across_dots(((1, 2), (5, 4)), ((5, 5), (1, 1))))
