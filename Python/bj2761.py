import sys
from math import tan, pi, inf

D, H = map(float, sys.stdin.readline().strip().split())
d_to_pi = pi / 180.0
while True:
    alpha, beta, gamma = map(float, sys.stdin.readline().strip().split())
    if alpha <= 0.0 and beta <= 0.0 and gamma <= 0.0:
        break
    a = 1.0 / tan(alpha * d_to_pi)**2
    b = 1.0 / tan(beta * d_to_pi)**2
    c = 1.0 / tan(gamma * d_to_pi)**2
    den = inf if a+c-2*b <= 0.0 else a+c-2*b
    z = (D*2**.5) / den**.5
    print(int(z+0.5+H))
