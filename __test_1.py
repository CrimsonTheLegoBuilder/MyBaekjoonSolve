def facto(n):
    if n <= 1:
        return 1
    return n * facto(n - 1)


def fibo(n):
    if n <= 1:
        return n
    return fibo(n - 1) + fibo(n - 2)


print(facto(10))
print(fibo(10))


