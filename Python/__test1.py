import sys
INPUT = sys.stdin.readline

def inner_product(a, b):
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2]


def outer_product(a, b):
    return (a[1]*b[2]-a[2]*b[1], a[2]*b[0]-a[0]*b[2], a[0]*b[1]-a[1]*b[0])


def omnicircumnavigation():
    points = [tuple(map(int, INPUT().strip().split())) for _ in range(int(INPUT()))]
    p = []
    for i in range(len(points)):
        for j in range(i+1, len(points)):
            if outer_product(points[i], points[j]) == (0, 0, 0):  # colinear
                if inner_product(points[i], points[j]) < 0:  # angle between line [(0, 0), points[i]] and line [(0, 0), points[j]] is 180 degrees
                    return "YES"
                else:  # duplicated
                    break
        else:
            p.append(points[i])
    for i in range(len(p)):
        k = -1
        for j in range(len(p)):
            if j in (i, k):
                continue
            # rotate a plane with [(0, 0), p[i]] as the axis to cover each point,
            # if the points are inside the semi-sphere,
            # there should exist two plane boundaries and the angle between them is less than 180 degrees and all points are inside them
            if k == -1 or inner_product(outer_product(p[i], p[k]), p[j]) > 0:
                k = j  # find the leftmost point where the left plane boundary is
        for j in range(len(p)):
            if j in (i, k):
                continue
            coplanar = inner_product(outer_product(p[i], p[k]), p[j])
            if coplanar == 0:  # coplanar
                if inner_product(outer_product(p[i], p[k]), outer_product(p[i], p[j])) < 0:  # angle between plane [(0, 0), p[i], p[k]] and plane [(0, 0), p[i], p[k]] is 180 degrees
                    break
            elif coplanar > 0:  # the left plane boundary doesn't exist, thus the points are not inside the semisphere
                break
        else:
            return "NO"  # all points are inside the semisphere
    return "YES"


sys.stdin = open('ts2_input.txt', 'r')
sys.stdout = open('ts2_output.txt', 'w')
for case in range(int(INPUT())):
    print('Case #%d: %s' % (case+1, omnicircumnavigation()))