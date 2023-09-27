arr = ['000', '001', '010', '011', '100', '101', '110', '111']
ans = []
a = list(input())
if len(a) == 1 and a[0] == '0':
    print(0)
else:
    for i in a:
        ans.append(arr[int(i)])
    print(''.join(ans).lstrip('0'))