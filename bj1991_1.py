import sys


def tree_order(node):
    global pre_o, in_o, post_o
    if node:
        pre_o += chr(node + 64)
        tree_order(tree[node][0])
        in_o += chr(node + 64)
        tree_order(tree[node][1])
        post_o += chr(node + 64)


N = int(sys.stdin.readline().strip())
tree = [[0]*3 for _ in range(27)]
for _ in range(N):
    p, ch1, ch2 = map(str, sys.stdin.readline().strip().split())
    if 'A' <= ch1 <= 'Z':
        tree[ord(p) - 64][0] = ord(ch1) - 64
        tree[ord(ch1) - 64][2] = ord(p) - 64
    if 'A' <= ch2 <= 'Z':
        tree[ord(p) - 64][1] = ord(ch2) - 64
pre_o, in_o, post_o = '', '', ''
tree_order(1)
print(pre_o)
print(in_o)
print(post_o)
