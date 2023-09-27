import sys

n, m = map(int, input().split())
set_N = set()
set_M = set()


for _ in range(n):
    set_N.add(sys.stdin.readline().strip())

for _ in range(m):
    set_M.add(sys.stdin.readline().strip())

# print(set_M)

list_N_M = list(set_N & set_M)

arr_NM = list(sorted(list_N_M))

print(len(arr_NM))
for i in arr_NM:
    sys.stdout.write(i + '\n')
