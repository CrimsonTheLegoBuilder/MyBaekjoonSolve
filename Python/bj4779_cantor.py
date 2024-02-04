def cantor(n):
    if n == 0:
        return '-'
    result = []
    s1 = cantor(n - 1)
    result.append(str(*s1) + ' ' * 3 ** (n - 1) + str(*s1))
    return result


# f = open('___ANSWER___.txt', 'r')
while True:
    try:
        line = cantor(int(input()))
        # line = cantor(int(f.readline().strip()))
        print(*line)
    except:
        break
