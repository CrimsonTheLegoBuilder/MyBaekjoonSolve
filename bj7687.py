import sys

while True:
    lx, ly, lz, X, Y, Z = map(int, sys.stdin.readline().strip().split())
    if lx == ly == lz == X == Y == Z == 0:
        break
    if Z == 0:
        print(X**2+Y**2)
    elif Y == 0:
        print(X**2+Z**2)
    elif X == 0:
        print(Y**2+Z**2)
    elif Z == lz:
        a = (lz+X)**2+Y**2
        b = (lz+Y)**2+X**2
        c = (lx+Y)**2+(lz+(lx-X))**2
        d = (ly+X)**2+(lz+(ly-Y))**2
        print(min(a, b, c, d))
    elif Y == ly:
        a = (ly+X)**2+Z**2
        b = (ly+Z)**2+X**2
        c = (lx+Z)**2+(ly+(lx-X))**2
        d = (lz+X)**2+(ly+(lz-Z))**2
        print(min(a, b, c, d))
    elif X == lx:
        a = (lx+Y)**2+Z**2
        b = (lx+Z)**2+Y**2
        c = (ly+Z)**2+(lx+(ly-Y))**2
        d = (lz+Y)**2+(lx+(lz-Z))**2
        print(min(a, b, c, d))
