import sys

N = int(sys.stdin.readline().strip())
arr = tuple(map(int, sys.stdin.readline().strip().split()))
mod_arr = [0]*3
n1, n2, n3 = [], [], []
for no in arr:
    if no % 3 == 1:
        n1.append(no)
    elif no % 3 == 2:
        n2.append(no)
    else:
        n3.append(no)
    mod_arr[no % 3] += 1

a, b, c = mod_arr
if (a == 0 and b and c) or (b+c+1 < a) or (a and not b and not c):
    print(-1)
else:
    ans2 = []
    while n2 and n3:
        ans2.append(n3.pop())
        ans2.append(n2.pop())
    while n2:
        ans2.append(n2.pop())
    ans1 = []
    while n1 and n3:
        ans1.append(n3.pop())
        ans1.append(n1.pop())
    while n1:
        ans1.append(n1.pop())
    ans = ans1 + ans2
    if n3:
        ans += n3
    print(*ans)
