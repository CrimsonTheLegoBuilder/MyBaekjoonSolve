def stars(n):
    if n == 1:
        return '*'
    result = []
    s1 = stars(n // 3)
    for i in range(n // 3):
        result.append(s1[i] + s1[i] + s1[i])
    for j in range(n // 3):
        result.append(s1[j] + ' ' * (n // 3) + s1[j])
    for k in range(n // 3):
        result.append(s1[k] + s1[k] + s1[k])
    return result


for l in stars(int(input())):
    print(l)
