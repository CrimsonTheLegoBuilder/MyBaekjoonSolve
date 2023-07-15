n = int(input())
a = 1

while n:
    if n == 0:
        print(1)
    else:
        a *= n
        n -= 1

print(a)
