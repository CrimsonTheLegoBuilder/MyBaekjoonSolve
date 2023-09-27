import sys

a = sys.stdin.readline().strip()
a_set = set()

for i in range(len(a)):
    for j in range(len(a) - i):
        a_set.add(a[i:i + j + 1])

print(len(a_set))
