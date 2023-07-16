ans = 0

for _ in range(4):
    w_rank = str(input())
    for i in range(0, 8, 2):
        if w_rank[i] == 'F':
            ans += 1
    b_rank = str(input())
    for j in range(1, 8, 2):
        if b_rank[j] == 'F':
            ans += 1

print(ans)
