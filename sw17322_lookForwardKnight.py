n = int(input())
MOD = 10 ** 9 + 7


def pow_mod(e, x):
    if x == 1:
        return e % MOD
    elif x % 2 != 0:
        return (pow_mod(e, x // 2) ** 2 * e) % MOD
    else:
        return (pow_mod(e, x // 2) ** 2) % MOD


for i in range(1, n + 1):
    nu = 1
    de = 1
    flag = 0

    a, b = map(int, input().split())
    EA2 = 2 * a - b
    EA1 = 2 * b - a

    if EA2 % 3 != 0 or EA1 % 3 != 0 or EA2 < 0 or EA1 < 0:
        print('#' + str(i) + ' 0')
        flag += 1

    if flag == 0:
        x = EA2 // 3
        y = EA1 // 3
        C = x + y

        for j in range(C - x + 1, C + 1):
            nu = (nu * j) % MOD

        for k in range(1, x + 1):
            de = (de * k) % MOD

        print('#' + str(i) + ' ' + str((nu * pow_mod(de, MOD - 2)) % MOD))
