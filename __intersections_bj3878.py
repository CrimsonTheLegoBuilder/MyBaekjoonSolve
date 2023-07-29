import sys


def cross_3(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def cross_4(a1, a2, a3, a4):  # cross product / CCW / for 4 dots
    v1 = (a2[0] - a1[0], a2[1] - a1[1])
    v2 = (a4[0] - a3[0], a4[1] - a3[1])
    return v1[0] * v2[1] - v1[1] * v2[0]


def intersection_dots(hull1, hull2):
    cross_dots = []
    for i in range(len(hull1)):
        for j in range(len(hull2)):
            if (hull2[j - 1] == hull1[i] or hull2[j - 1] == hull1[i - 1]) | \
                (cross_3(hull1[i - 1], hull1[i], hull2[j - 1]) == 0 and
                    sorted([hull1[i - 1], hull1[i], hull2[j - 1]], key=lambda o: (o[0], o[1]))[1] == hull2[j - 1]):
                d1 = hull2[j - 1]
                cross_dots.append(d1)
            if (hull1[i - 1] == hull2[j] or hull1[i - 1] == hull2[j - 1]) | \
                    (cross_3(hull2[j - 1], hull2[j], hull1[i - 1]) == 0 and
                     sorted([hull2[j - 1], hull2[j], hull1[i - 1]], key=lambda o: (o[0], o[1]))[1] == hull1[
                         i - 1]):
                d1 = hull1[i - 1]
                cross_dots.append(d1)
            if cross_3(hull1[i - 1], hull1[i], hull2[j - 1]) * cross_3(hull1[i], hull1[i - 1], hull2[j]) > 0 and \
                    cross_3(hull2[j - 1], hull2[j], hull1[i - 1]) * cross_3(hull2[j], hull2[j - 1], hull1[i]) > 0:
                # prevent ZeroDivisionError
                if hull1[i - 1][0] - hull1[i][0] == 0 and hull2[j - 1][1] - hull2[j][1] == 0:
                    x = hull1[i - 1][0]
                    y = hull2[j - 1][1]
                elif hull2[j - 1][0] - hull2[j][0] == 0 and hull1[i - 1][1] - hull1[i][1] == 0:
                    x = hull2[j - 1][0]
                    y = hull1[i - 1][1]
                elif hull1[i - 1][0] - hull1[i][0] == 0:
                    x = hull1[i - 1][0]
                    y = hull2[j - 1][1] + (hull2[j - 1][1] - hull2[j][1]) * (hull1[i - 1][0] - hull2[j - 1][0]) / (
                                hull2[j - 1][0] - hull2[j][0])
                elif hull1[i - 1][1] - hull1[i][1] == 0:
                    x = hull2[j - 1][0] + (hull2[j - 1][0] - hull2[j][0]) * (hull1[i - 1][1] - hull2[j - 1][1]) / (
                                hull2[j - 1][1] - hull2[j][1])
                    y = hull1[i - 1][1]
                elif hull2[j - 1][0] - hull2[j][0] == 0:
                    x = hull2[j - 1][0]
                    y = hull1[i - 1][1] + (hull1[i - 1][1] - hull1[i][1]) * (hull2[j - 1][0] - hull1[i - 1][0]) / (
                                hull1[i - 1][0] - hull1[i][0])
                elif hull2[j - 1][1] - hull2[j][1] == 0:
                    x = hull1[i - 1][0] + (hull1[i - 1][0] - hull1[i][0]) * (hull2[j - 1][1] - hull1[i - 1][1]) / (
                                hull1[i - 1][1] - hull1[i][1])
                    y = hull2[j - 1][1]
                else:
                    a = (hull1[i - 1][1] - hull1[i][1]) / (hull1[i - 1][0] - hull1[i][0])
                    b = (hull2[j - 1][1] - hull2[j][1]) / (hull2[j - 1][0] - hull2[j][0])
                    c = (hull1[i - 1][0] - hull1[i][0]) / (hull1[i - 1][1] - hull1[i][1])
                    d = (hull2[j - 1][0] - hull2[j][0]) / (hull2[j - 1][1] - hull2[j][1])

                    x = (hull2[j - 1][1] - hull1[i - 1][1] + a * hull1[i - 1][0] - b * hull2[j - 1][0]) / (a - b)
                    y = (hull2[j - 1][0] - hull1[i - 1][0] + c * hull1[i - 1][1] - d * hull2[j - 1][1]) / (c - d)
                cross_dots.append((x, y))
    cross_dots = list(set(cross_dots))
    return cross_dots


def get_inner(arr1, arr2):
    hull_candidate = []
    len_1, len_2 = len(arr1), len(arr2)
    for i in arr1:
        flag = True
        for j in range(len_2):
            if cross_3(i, arr2[j], arr2[(j + 1) % len_2]) * cross_3(i, arr2[(j + 1) % len_2], arr2[(j + 2) % len_2]) <= 0:
                flag = False
                break
        if flag:
            hull_candidate.append(i)
    for j in arr2:
        flag = True
        for i in range(len_1):
            if cross_3(j, arr1[i], arr1[(i + 1) % len_1]) * cross_3(j, arr1[(i + 1) % len_1], arr1[(i + 2) % len_1]) <= 0:
                flag = False
                break
        if flag:
            hull_candidate.append(j)
    return hull_candidate


def monotone_chain(arr):  # get hull
    arr.sort(key=lambda x: (x[0], x[1]))
    if len(arr) <= 2:
        return arr
    lower = []
    for dl in arr:
        while len(lower) > 1 and cross_3(lower[-2], lower[-1], dl) <= 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(arr):
        while len(upper) > 1 and cross_3(upper[-2], upper[-1], du) <= 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


t = int(sys.stdin.readline().strip())
for _ in range(t):
    n, m = map(int, sys.stdin.readline().strip().split())
    arr_a = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
    arr_b = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(m)]
    if n == 1 and m == 1:
        print('YES')
    elif n == 2 and m == 1:
        if cross_3(arr_a[0], arr_a[1], arr_b[0]) == 0 and \
                sorted([arr_a[0], arr_a[1], arr_b[0]], key=lambda x: (x[0], x[1]))[1] == arr_b[0]:
            print('NO')
        else:
            print('YES')
    elif n == 1 and m == 2:
        if cross_3(arr_b[0], arr_b[1], arr_a[0]) == 0 and \
                sorted([arr_b[0], arr_b[1], arr_a[0]], key=lambda x: (x[0], x[1]))[1] == arr_a[0]:
            print('NO')
        else:
            print('YES')
    elif n == m == 2:
        if (cross_3(arr_a[0], arr_a[1], arr_b[0]) * cross_3(arr_a[1], arr_a[0], arr_b[1]) > 0 and
                cross_3(arr_b[0], arr_b[1], arr_a[0]) * cross_3(arr_b[1], arr_b[0], arr_a[1]) > 0) | \
            (cross_3(arr_a[0], arr_a[1], arr_b[0]) == 0 and
             sorted([arr_a[0], arr_a[1], arr_b[0]], key=lambda x: (x[0], x[1]))[1] == arr_b[0]) | \
            (cross_3(arr_a[0], arr_a[1], arr_b[1]) == 0 and
             sorted([arr_a[0], arr_a[1], arr_b[1]], key=lambda x: (x[0], x[1]))[1] == arr_b[1]) | \
            (cross_3(arr_b[0], arr_b[1], arr_a[0]) == 0 and
             sorted([arr_b[0], arr_b[1], arr_a[0]], key=lambda x: (x[0], x[1]))[1] == arr_a[0]) | \
            (cross_3(arr_b[0], arr_b[1], arr_a[1]) == 0 and
             sorted([arr_b[0], arr_b[1], arr_a[1]], key=lambda x: (x[0], x[1]))[1] == arr_a[1]):
            print('NO')
        else:
            print('YES')
    # elif m == 1:
    #     a = monotone_chain(arr_a)
    #     across = intersection_dots(arr_b, a)
    #     if len(across) < 1:
    #         print('YES')
    #     else:
    #         print('NO')
    else:
        a = monotone_chain(arr_a)
        b = monotone_chain(arr_b)
        across = get_inner(a, b) + intersection_dots(a, b)
        # print(get_inner(a, b))
        # print(intersection_dots(a, b))
        if len(across) < 1:
            print('YES')
        else:
            print('NO')
