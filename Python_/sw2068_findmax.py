n = int(input())

for j in range(1, n + 1):
    a = list(map(int, input().split()))

    print('#' + str(j) + ' ' + str(max(a)))
