for tc in range(1, 11):
    _, s = map(str, input().split())
    s = list(s)
    stack = []
    while s:
        a = s.pop()
        if stack and stack[-1] == a:
            stack.pop()
            continue
        stack.append(a)
    while stack:
        a = stack.pop()
        s.append(a)
    print(f'#{tc} {"".join(s)}')
