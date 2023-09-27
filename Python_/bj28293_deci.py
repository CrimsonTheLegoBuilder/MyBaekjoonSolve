import math

a, b = map(int, input().split())

print(int(math.ceil(b * math.log10(a))))
