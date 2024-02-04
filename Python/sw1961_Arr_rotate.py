n = int(input())

for t in range(1, n+1):
    l = int(input())
    arr = []
    answer90 = []
    answer180 = []
    answer270 = []
    for _ in range(l):
        arr.append(list(map(int, input().split())))

    for i in range(l):
        answer = []
        for j in range(l - 1, -1, -1):
            answer.append(arr[j][i])
        answer90.append(answer)

    for i in range(l - 1, -1, -1):
        answer = []
        for j in range(l - 1, -1, -1):
            answer.append(arr[i][j])
        answer180.append(answer)

    for i in range(l - 1, -1, -1):
        answer = []
        for j in range(l):
            answer.append(arr[j][i])
        answer270.append(answer)

    print('#' + str(t))
    for i in range(l):
        print(''.join(map(str, answer90[i])) + ' ' + ''.join(map(str, answer180[i])) + ' ' + ''.join(map(str, answer270[i])))
