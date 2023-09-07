for _ in range(int(input())):
    order = input()
    score = 0
    tmp = 1
    for i in order:
        if i == 'O':
            score += tmp
            tmp += 1
        else:
            tmp = 1
    print(score)
