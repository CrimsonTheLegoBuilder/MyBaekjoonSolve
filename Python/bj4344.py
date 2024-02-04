teca = int(input())

for _ in range(teca):
    n = input().split()

    summ = 0
    for i in n[1:]:
        summ += int(i)

    avg = summ / int(n[0])

    x = 0
    for j in n[1:]:
        if int(j) > avg:
            x += 1

    answer = x * 100 / int(n[0])

    print('{:.3f}%'.format(answer))
