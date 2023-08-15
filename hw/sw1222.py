for t in range(1, 11):
    _ = input()
    arr = list(input())
    ans = 0
    for i in arr:
        if i.isnumeric():
            ans += int(i)
    print(f'#{t} {ans}')
