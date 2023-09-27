n = int(input())


def hanoi(n, s, t, d):
    if n == 1:
        return print(s, d)
    hanoi(n - 1, s, d, t)
    print(s, d)
    hanoi(n - 1, t, s, d)


print(2 ** n - 1)
hanoi(n, 1, 2, 3)
