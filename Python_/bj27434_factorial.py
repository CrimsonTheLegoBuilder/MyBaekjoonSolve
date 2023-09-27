n = int(input())

if n == 0:
    print(1)
else:
    for i in range(n - 1, 0, -1):
        n *= i
    print(n)
