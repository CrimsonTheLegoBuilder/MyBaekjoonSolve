def preorder(node):
    global cnt
    if node:
        cnt += 1
        preorder(tree[node][0])
        preorder(tree[node][1])


for tc in range(1, int(input())+1):
    E, N = map(int, input().split())
    tree = [[0] * 3 for _ in range(E+2)]
    temp = list(map(int, input().split()))
    for i in range(E):
        p = temp[i*2]
        c = temp[i*2+1]
        if tree[p][0] == 0:
            tree[p][0] = c
        else:
            tree[p][1] = c
        tree[c][2] = p
    root = 1
    cnt = 0
    preorder(N)
    print(f'#{tc} {cnt}')


'''
13
1 2 1 3 2 4 3 5 3 6 4 7 5 8 5 9 6 10 6 11 7 12 11 13
5
3 1 3 2 2 4 2 5
'''