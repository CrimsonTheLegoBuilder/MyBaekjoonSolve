import sys


def pow_mod(e, x):
    if x == 1:
        return e % MOD
    elif x % 2 != 0:
        return (pow_mod(e, x // 2) ** 2 * e) % MOD
    else:
        return (pow_mod(e, x // 2) ** 2) % MOD


a, b, MOD = map(int, sys.stdin.readline().strip().split())

print(pow_mod(a, b))
