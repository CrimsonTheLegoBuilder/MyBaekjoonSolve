import sys
sys.stdin = open('input_four_cal.txt', 'r')


def postorder(n):
    if order[n].isnumeric():
        return int(order[n])
    else:
        a = postorder(tree[n][0])
        b = postorder(tree[n][1])
        t = order[n]
        if t == '+': return a+b
        if t == '-': return a-b
        if t == '*': return a*b
        if t == '/': return a//b
        return order[n]


for tc in range(1, 11):
    V = int(input())
    tree = [[0] * 3 for _ in range(V+1)]
    order = [0] * (V+1)
    for _ in range(V):
        p, *e = map(str, input().split())
        if len(e) > 1:
            order[int(p)] = e[0]
            tree[int(p)][0] = int(e[1])
            tree[int(p)][1] = int(e[2])
            tree[int(e[1])][2] = int(p)
            tree[int(e[2])][2] = int(p)
        else:
            order[int(p)] = e[0]
    # print(order)
    # print(tree)

    print(f'#{tc} {postorder(1)}')


