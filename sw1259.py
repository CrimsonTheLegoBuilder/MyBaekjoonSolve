from collections import deque

for tc in range(int(input())):
    N = int(input())
    arr = list(map(int, input().split()))
    nails = deque()
    for i in range(0, 2*N-1, 2):
        nails.append((arr[i], arr[i+1]))
    ans = deque()
    while nails:
        tmp = nails.pop()
        if not ans:
            ans += [*tmp]
        elif ans[0] == tmp[1]:
            ans.extendleft([tmp[1], tmp[0]])
        elif ans[-1] == tmp[0]:
            ans += [*tmp]
        else:
            nails.appendleft(tmp)
        # print(nails)
    print(f"#{tc+1}", *ans)
