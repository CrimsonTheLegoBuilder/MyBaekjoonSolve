S = input()
if len(S) < 5:
    print("NO")
    exit(0)
F = [0] * 5
for s in S:
    if s == "M":
        F[0] = 1
    if s == "O":
        F[1] = 1
    if s == "B":
        F[2] = 1
    if s == "I":
        F[3] = 1
    if s == "S":
        F[4] = 1
# print("YES" if sum(F) == 5 else "NO")
