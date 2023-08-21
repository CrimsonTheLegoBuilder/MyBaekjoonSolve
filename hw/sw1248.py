import sys
sys.stdin = open('input_tree.txt', 'r')


def preorder(node):
    global cnt
    if node:
        cnt += 1
        preorder(tree[node][0])
        preorder(tree[node][1])


for tc in range(1, int(input())+1):
    V, E, t1, t2 = map(int, input().split())
    tree = [[0] * 3 for _ in range(V+1)]
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
    while tree[root][2] != 0:
        root += 1
    # for row in tree:
    #     print(*row)
    # preorder(root)
    v1 = t1
    grand_parent1 = []
    while tree[v1][2] != 0:
        v1 = tree[v1][2]
        grand_parent1.append(v1)
    v2 = t2
    grand_parent2 = []
    while tree[v2][2] != 0:
        v2 = tree[v2][2]
        grand_parent2.append(v2)
    g_p = 1
    # print(grand_parent1)
    # print(grand_parent2)
    flag = 0
    for i in grand_parent1:
        for j in grand_parent2:
            if i == j:
                g_p = i
                flag = 1
                break
        if flag:
            break
    cnt = 0
    preorder(g_p)
    print(f'#{tc} {g_p} {cnt}')


'''
13
1 2 1 3 2 4 3 5 3 6 4 7 5 8 5 9 6 10 6 11 7 12 11 13
5
3 1 3 2 2 4 2 5
'''