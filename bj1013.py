import re
import sys

for _ in range(int(sys.stdin.readline().strip())):
    sign = sys.stdin.readline().strip()
    p = re.compile('(100+1+|01)+')
    m = p.fullmatch(sign)
    if m:
        print("YES")
    else:
        print("NO")

# results = []
#
# for _ in range(int(input())):
#     sign = input()
#     p = re.compile('(100+1+|01)+')
#     m = p.fullmatch(sign)
#     if m:
#         results.append("YES")
#     else:
#         results.append("NO")
#
# for result in results:
#     print(result)

