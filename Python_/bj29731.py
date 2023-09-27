import sys

promise = [
        "Never gonna give you up",
        "Never gonna let you down",
        "Never gonna run around and desert you",
        "Never gonna make you cry",
        "Never gonna say goodbye",
        "Never gonna tell a lie and hurt you",
        "Never gonna stop",
]
N = int(sys.stdin.readline().strip())
cnt = 0
for _ in range(N):
    s = sys.stdin.readline().strip()
    flag = 0
    for i in range(7):
        if s == promise[i]:
            flag = 1
    if flag:
        cnt += 1
print("No" if cnt == N else "Yes")
