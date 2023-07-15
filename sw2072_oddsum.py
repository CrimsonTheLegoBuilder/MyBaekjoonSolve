n = int(input())

for j in range(1, n + 1):
    summ = 0
    a = list(map(int, input().split()))

    for i in a:
        if i % 2 == 1:
            summ += i

    print('#' + str(j) + ' ' + str(summ))
