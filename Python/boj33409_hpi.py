from fractions import Fraction


def read_fraction() -> tuple:
    p_x, q_x, p_y, q_y = map(int, input().split())
    return Fraction(p_x, q_x), Fraction(p_y, q_y)


def cross(d1, d2, d3) -> Fraction:
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def intersection_(p1, p2, q1, q2):
    a1 = cross(q1, q2, p1)
    a2 = -cross(q1, q2, p2)
    return (p1 * a2 + p2 * a1) / (a1 + a2)


