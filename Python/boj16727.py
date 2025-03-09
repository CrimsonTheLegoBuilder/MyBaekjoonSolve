P = 0
E = 0
p, e = map(int, input().split())
P += p
E += e
de = e
e, p = map(int, input().split())
P += p
E += e
dp = p
ans = "Penalty"
if P == E and dp > de or P > E:
    ans = "Persepolis"
elif P == E and dp < de or P < E:
    ans = "Esteghlal"
print(ans)
