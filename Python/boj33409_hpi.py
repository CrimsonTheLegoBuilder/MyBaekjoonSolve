from fractions import Fraction


def read_fraction() -> tuple:
    p_x, q_x, p_y, q_y = map(int, input().split())
    return Fraction(p_x, q_x), Fraction(p_y, q_y)


def cross(d1, d2, d3) -> Fraction:
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def intersection_(p1_, p2_, q1_, q2_):
    a1 = cross(q1_, q2_, p1_)
    a2 = -cross(q1_, q2_, p2_)
    x: Fraction = p1_[0] * a2 + p2_[0] * a1
    y: Fraction = p1_[1] * a2 + p2_[1] * a1
    return (p1 * a2 + p2 * a1) / (a1 + a2)


p0 = (Fraction(0, 1), Fraction(0, 1))
p1 = (Fraction(1, 1), Fraction(1, 1))
q0 = (Fraction(0, 1), Fraction(1, 1))
q1 = (Fraction(1, 1), Fraction(0, 1))
c0 = intersection_(p0, p1, q0, q1)
print(c0)
