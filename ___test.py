def cross(d1, d2, d3):  # cross product / CCW / get area
    if len(d2) == 0:
        return
    return (d2[1] - d1[1]) * (d3[2] - d2[2]) - (d2[2] - d1[2]) * (d3[1] - d2[1])


print(cross([0, 1, 2], [], [12, 12, 31]))
print(None or 1)