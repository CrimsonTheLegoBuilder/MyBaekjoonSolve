n = int(input())

arr = list(str(input()))

for i in range(n - 1):
    comp = str(input())
    for j in range(len(comp)):
        if arr[j] != comp[j]:
            arr[j] = '?'

print(''.join(arr))
