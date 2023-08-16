isp = {'(': 0, '*': 2, '/': 2, '+': 1, '-': 1}
icp = {'(': 3, '*': 2, '/': 2, '+': 1, '-': 1}

for tc in range(1, 11):
    n = int(input())
    order = input()
    result = []
    stack = []
    for token in order:
        if token.isnumeric():
            result.append(token)
        elif token == ')':
            while stack[-1] != '(':
                result.append(stack.pop())
            stack.pop()
        else:
            while stack and icp[token] <= isp[stack[-1]]:
                result.append(stack.pop())
            stack.append(token)
    while stack:
        result.append(stack.pop())
    # print(result)
    # ans = ''.join(result)
    # print(ans)
    # print(stack)
    for s in result:
        if s.isnumeric():
            stack.append(int(s))
        try:
            if s == '+':
                stack.append(stack.pop() + stack.pop())
            elif s == '-':
                stack.append(-stack.pop() + stack.pop())
            elif s == '*':
                stack.append(stack.pop() * stack.pop())
            elif s == '/':
                stack.append(int(stack.pop()**-1 * stack.pop()))
            elif s == '.':
                if len(stack) == 1:
                    continue
                else:
                    stack = ['error']
        except IndexError:
            stack = ['error']
        except ZeroDivisionError:
            stack = ['error']
    print(f'#{tc} {stack[0]}')
