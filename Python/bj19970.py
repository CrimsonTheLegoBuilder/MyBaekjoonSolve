import sys


def dots(order):
    line = []
    i = 0
    while i < len(order)-1:
        if 'A' <= order[i] <= 'D':
            if order[i+1] == '1':
                line.append(L[order[i:i+2]])
                i += 2
            else:
                line.append(L[order[i]])
                i += 1
    if 'A' <= order[-1] <= 'D':
        line.append(L[order[i]])
    return line


def get_mid_p(s):
    d = ((s[0][0] + s[1][0]) // 2, (s[0][1] + s[1][1]) // 2, (s[0][2] + s[1][2]) // 2)
    return d


L = {
    "A": (0, 2, 0),
    "B": (0, 0, 0),
    "C": (2, 0, 0),
    "D": (2, 2, 0),
    "A1": (0, 2, 2),
    "B1": (0, 0, 2),
    "C1": (2, 0, 2),
    "D1": (2, 2, 2),
}


L1 = dots(str(sys.stdin.readline().strip()))
L2 = dots(str(sys.stdin.readline().strip()))
d1 = get_mid_p(L1)
d2 = get_mid_p(L2)
flag1 = L1[0] == L2[0] or L1[1] == L2[1] or L1[0] == L2[1] or L1[1] == L2[0]
flag2 = d1 == d2
if flag1 or flag2:
    print("YES")
else:
    print("NO")
