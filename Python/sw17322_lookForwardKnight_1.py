n = int(input())
MOD = 10 ** 9 + 7


def pow_mod(e, x):
    if x == 1:
        return e % MOD
    elif x % 2 != 0:
        return (pow_mod(e, x // 2) ** 2 * e) % MOD
    else:
        return (pow_mod(e, x // 2) ** 2) % MOD


arr = [1]
fac = 1
for a in range(1, 666667):
    fac *= (a % MOD)
    fac = fac % MOD
    arr.append(fac)

for i in range(1, n + 1):
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

        nu = arr[C]
        de1 = arr[x]
        de2 = arr[C - x]

        print('#' + str(i) + ' ' + str((nu * pow_mod(de1, MOD - 2) * pow_mod(de2, MOD - 2)) % MOD))
