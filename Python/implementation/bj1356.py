def prod(lst):
    ans = 1
    for i in lst:
        ans *= i
    return ans


arr = list(map(int, list(str(input()))))

if len(arr) <= 1:
    print('NO')
elif len(arr) == 2:
    if arr[0] == arr[1]:
        print('YES')
    else:
        print('NO')
else:
    for j in range(1, len(arr)):
        if prod(arr[:j]) == prod(arr[j:]):
            print('YES')
            exit(0)
    print('NO')
