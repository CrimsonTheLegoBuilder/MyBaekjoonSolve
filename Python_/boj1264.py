import sys

while 1:
    a = sys.stdin.readline().strip()
    if a == '#':
        break
    cnt = 0
    for x in a:
        if x in ['a', 'i', 'e', 'u', 'o', 'A', 'I', 'U', 'E', 'O']:
            cnt += 1
    # print(cnt)
