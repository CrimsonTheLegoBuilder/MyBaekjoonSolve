

def meet(y, slp, dot):
    if slp[1] == 0:
        return
    x = slp[0] * (y - dot[1]) / slp[1] + dot[0]
    return x


s = []
for i in range(10):
    n = int(input())
    s.append(n % 42)

s = set(s)
print(len(s))
