import sys

while True:
    vps = sys.stdin.readline().rstrip()
    if vps == '.':
        break

    stack = []
    cnt = 0

    for i in vps:
        if i == '(':
            stack.append('a')
        elif i == '[':
            stack.append('b')
        elif i == ')':
            if len(stack) != 0 and stack[-1] == 'a':
                stack.pop()
            else:
                stack.append(1)
                break
        elif i == ']':
            if len(stack) != 0 and stack[-1] == 'b':
                stack.pop()
            else:
                stack.append(1)
                break

    if len(stack) == 0:
        print('yes')
    else:
        print('no')
