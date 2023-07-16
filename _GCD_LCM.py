def gcd(x, y):
    if not y:
        return x
    if not x % y:
        return y
    z = x % y
    return gcd(y, z)


def lcm(x, y):
    return x * y // gcd(x, y)


a, b = map(int, input().split())
print(gcd(a, b))
print(lcm(a, b))
