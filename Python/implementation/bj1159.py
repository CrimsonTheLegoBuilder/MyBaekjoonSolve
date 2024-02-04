n = int(input())
dic = {}
ans = []

for _ in range(n):
    a = list(str(input()))
    if a[0] not in dic:
        dic[a[0]] = 1
    else:
        dic[a[0]] += 1

for i in dic.keys():
    if dic[i] >= 5:
        ans.append(i)

ans.sort()
if len(ans):
    print(''.join(ans))
else:
    print('PREDAJA')
