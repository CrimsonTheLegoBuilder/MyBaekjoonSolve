def max_(a, b): return a if a >= b else b


# def bt(x):
#     global ans
#     if len(stack) == 2:
#         temp = numbers[stack[0]] * numbers[stack[1]]
#         if scan(temp):
#             ans = max_(ans, temp)
#         return
#     for i in range(x, N):
#         stack.append(i)
#         bt(i+1)
#         stack.pop()


def scan(no):
    target = list(map(int, str(no)))
    for i in range(len(target)-1):
        if target[i+1] < target[i]:
            return 0
    return 1


def scan2(no):
    pre = 10
    while no:
        cur = no % 10
        if cur > pre:
            return 0
        pre = cur
        no //= 10
    return 1


for tc in range(1, int(input())+1):
    stack = []
    N = int(input())
    numbers = tuple(map(int, input().split()))
    ans = -1
    for i in range(N-1):
        for j in range(i+1, N):
            a = numbers[i] * numbers[j]
            if scan2(numbers[i] * numbers[j]):
                ans = max_(ans, a)
    # bt(0)
    print(f'#{tc} {ans}')
