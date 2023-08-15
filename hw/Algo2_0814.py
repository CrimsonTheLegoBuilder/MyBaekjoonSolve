t = int(input())
for tc in range(1, t+1):
    string = input()
    stack = []
    flag = 0  # 깃발이 서면 이 문장은 틀린 문장임
    ans = 0   # 답

    if ord('0') <= ord(string[0]) <= ord('9'):  # 먼저 앞 뒤 검사부터
        print(f'#{tc} -1')
        continue
    if ord('0') <= ord(string[-1]) <= ord('9'):  # 괄호가 없으면 끝
        print(f'#{tc} -1')
        continue

    for s in string:
        # print(stack)  # 확인용 출력
        if ord('0') <= ord(s) <= ord('9'):  # 꺼낸 문자가 숫자라면?
            stack.append(int(s))            # 정수로 바꿔서 스택에 쌓기

        else:                               # 꺼낸 문자가 괄호라면?
            if stack and s == ')':          # 스택이 차 있고 더하는 괄호일 때 (일단 문장 구성이 됐을 때)
                if stack[-1] == '(':        # 혹시나 괄호가 비어있을 수 있음. 그래도 문장 구성이 잘못된 건 아님.
                    stack.pop()             # 빼고 다음으로 넘어간다.
                elif stack[-1] == '{' or stack[-1] == '}' \
                        or stack[-1] == ')':  # 괄호가 비어있는데 짝까지 안 맞으면?
                    stack.pop()               # 일단 비움
                    flag = 1                  # 이 문장은 틀렸다.
                else:                         # 괄호 안에 숫자가 있거나 다른 괄호와 짝일 때
                    ans = stack.pop()         # 일단 하나를 꺼낸다
                    while stack[-1] != '(':
                        if stack[-1] == '{' or stack[-1] == '}' \
                                or stack[-1] == ')':
                            flag = 1          # 괄호가 올바르게 닫히지 않았다면 틀림.
                            break
                        ans += stack.pop()    # 답에 더한다
                    stack.pop()               # 괄호 버리기
                    stack.append(ans)         # 답을 다시 담는다.
            elif s == ')':
                flag = 1                      # 닫힌 괄호부터 나왔다면 잘못된 문장임

            if stack and s == '}':            # 스택이 차 있고 곱하는 괄호일 때
                if stack[-1] == '{':
                    stack.pop()
                elif stack[-1] == '(' or stack[-1] == '}' \
                        or stack[-1] == ')':
                    stack.pop()
                    flag = 1
                else:
                    ans = stack.pop()
                    while stack[-1] != '{':
                        if stack[-1] == '(' or stack[-1] == '}' \
                                or stack[-1] == ')':
                            flag = 1
                            break
                        ans *= stack.pop()
                    stack.pop()
                    stack.append(ans)
            elif s == '}':
                flag = 1    # 닫힌 괄호부터 나왔다면 잘못된 문장임

            if s == '(':
                if stack and isinstance(stack[0], int):
                    flag = 1
                stack.append(s)  # 열린 괄호 넣기
                
            if s == '{':
                if stack and isinstance(stack[0], int):  # 이것도 못 넣음...
                    flag = 1
                stack.append(s)  # 열린 괄호 넣기

    if not stack:  # 이걸 못 넣었네...
        flag = 1
    if stack:
        if stack[0] == '(' or stack[0] == ')' or stack[0] == '{' or stack[0] == '}':  # 연산이 끝났는데 괄호가 남아있다면?
            flag = 1
    if len(stack) > 1:  # 연산이 끝났는데 괄호가 남아있다면?
        flag = 1
    if flag:
        ans = -1
    print(f'#{tc} {ans}')
