import sys

color = ('black', 'brown', 'red', 'orange', 'yellow', 'green', 'blue', 'violet', 'grey', 'white')
ans = 0

a = sys.stdin.readline().strip()
ans += 10 * color.index(a)
b = sys.stdin.readline().strip()
ans += color.index(b)
c = sys.stdin.readline().strip()
ans *= 10 ** color.index(c)

print(ans)
