import sys


def preorder(node):
    global pre_o
    if node:
        pre_o += chr(node + 64)
        preorder(tree[node][0])
        preorder(tree[node][1])


def inorder(node):
    global in_o
    if node:
        inorder(tree[node][0])
        in_o += chr(node + 64)
        inorder(tree[node][1])


def postorder(node):
    global post_o
    if node:
        postorder(tree[node][0])
        postorder(tree[node][1])
        post_o += chr(node + 64)


N = int(sys.stdin.readline().strip())
tree = [[0]*3 for _ in range(27)]
for _ in range(N):
    vts, ch1, ch2 = map(str, sys.stdin.readline().strip().split())
    if 'A' <= ch1 <= 'Z':
        tree[ord(vts) - 64][0] = ord(ch1) - 64
        tree[ord(ch1) - 64][2] = ord(vts) - 64
    if 'A' <= ch2 <= 'Z':
        tree[ord(vts) - 64][1] = ord(ch2) - 64
        tree[ord(ch2) - 64][2] = ord(vts) - 64
pre_o, in_o, post_o = '', '', ''
preorder(1)
inorder(1)
postorder(1)
print(pre_o)
print(in_o)
print(post_o)
