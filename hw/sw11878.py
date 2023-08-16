# for t in range(1, int(input())+1):
#     Order = list(input().split())
#     stack = []
#     ans = 'error'
#     for o in Order:
#         if o.isdigit():
#             stack.append(int(o))
#         else:
#             if o != '.' and len(stack) < 2:
#                 ans = 'error'
#                 break
#             if o == '+':
#                 temp = 0
#                 temp += stack.pop() + stack.pop()
#                 stack.append(temp)
#             if o == '*':
#                 temp = 1
#                 temp *= stack.pop() * stack.pop()
#                 stack.append(temp)
#             if o == '-':
#                 temp = 0
#                 temp -= stack.pop() - stack.pop()
#                 stack.append(temp)
#             if o == '/':
#                 temp = 0
#                 temp += int(stack.pop()**-1 * stack.pop())
#                 stack.append(temp)
#             if o == '.':
#                 ans = stack[0]
#                 if len(stack) > 1:
#                     ans = 'error'
#     print(f'#{t} {ans}')
#
# for t in range(1, int(input())+1):
#     Order = list(input().split())
#     stack = []
#     ans = 'error'
#     for o in Order:
#         if o == '.':
#             if len(stack) > 1:
#                 stack = ['error']
#             break
#         if o.isnumeric():
#             stack.append(int(o))
#         else:
#             if o != '.' and len(stack) < 2:
#                 stack = ['error']
#                 break
#             if o == '+':
#                 stack.append(stack.pop() + stack.pop())
#             if o == '*':
#                 stack.append(stack.pop() * stack.pop())
#             if o == '-':
#                 stack.append(-stack.pop() + stack.pop())
#             if o == '/':
#                 a, b = stack.pop(), stack.pop()
#                 if not b:
#                     stack = ['error']
#                     break
#                 stack.append(int(a**-1 * b))
#     print(f'#{t} {stack[0]}')

for tc in range(1, int(input())+1):
    code = list(input().split())
    ans = 0
    stack = []
    for s in code:
        if s.isnumeric():  # 숫자면
            stack.append(int(s))  # stack에 숫자넣음
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
                    ans = stack[0]
                else:
                    ans = 'error'
        except IndexError:
            ans = 'error'
        except ZeroDivisionError:
            ans = 'error'
    print(f'#{tc} {ans}')
