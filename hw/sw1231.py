import sys
sys.stdin = open('input_inorder.txt', 'r')


def inorder(node):
    if node <= n:
        inorder(node*2)
        print(tree[node], end='')
        inorder(node*2+1)


for tc in range(1, 11):
    n = int(input())
    tree = [0] * (n+1)
    for _ in range(n):
        V, *E = input().split()
        tree[int(V)] = E[0]
    root = 1
    print(f'#{tc}', end=' ')
    inorder(1)
    print()

'''
13
1 2 1 3 2 4 3 5 3 6 4 7 5 8 5 9 6 10 6 11 7 12 11 13
5
3 1 3 2 2 4 2 5
'''