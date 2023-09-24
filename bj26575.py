def f(o):
    ans = 1.0
    for i in o:
        if i[0] == '.':
            ans *= float('0'+i)
        else:
            ans *= float(i)
    return ans


for _ in range(int(input())):
    a = list(map(str, input().split()))
    print(f"${f(a):.2f}")
