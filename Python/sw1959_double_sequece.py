n = int(input())

for i in range(1, n + 1):
    a, b = map(int, input().split())
    arr_A = list(map(int, input().split()))
    arr_B = list(map(int, input().split()))
    ans = set()
    if a < b:
        for j in range(b - a + 1):
            MAX = 0
            for k in range(a):
                MAX += arr_A[k] * arr_B[k + j]
            ans.add(MAX)

    elif a > b:
        for j in range(a - b + 1):
            MAX = 0
            for k in range(b):
                MAX += arr_B[k] * arr_A[k + j]
            ans.add(MAX)

    elif a == b:
        MAX = 0
        for j in range(a):
            MAX += arr_A[j] * arr_B[j]
            ans.add(MAX)

    print('#' + str(i) + ' ' + str(max(ans)))
