import sys


def tree_order(node):
    global pre_o, in_o, post_o
    if 'A' <= node <= 'Z':
        pre_o += node
        tree_order(tree[node][0])
        in_o += node
        tree_order(tree[node][1])
        post_o += node


N = int(sys.stdin.readline().strip())
tree = {}
for _ in range(N):
    p, ch1, ch2 = map(str, sys.stdin.readline().strip().split())
    tree[p] = (ch1, ch2)
pre_o, in_o, post_o = '', '', ''
tree_order('A')
print(pre_o)
print(in_o)
print(post_o)
