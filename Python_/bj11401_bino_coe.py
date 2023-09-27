import sys

MOD = 10 ** 9 + 7
a, b = map(int, sys.stdin.readline().strip().split())
nu = 1
de = 1


def pow_mod(e, x):
    if x == 1:
        return e % MOD
    elif x % 2 != 0:
        return (pow_mod(e, x // 2) ** 2 * e) % MOD
    else:
        return (pow_mod(e, x // 2) ** 2) % MOD


for i in range(a - b + 1, a + 1):
    nu = (nu * i) % MOD

for j in range(1, b + 1):
    de = (de * j) % MOD

print((nu * pow_mod(de, MOD - 2)) % MOD)
