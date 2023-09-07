arr = list(map(int, input().split()))
flag1, flag2 = 1, 1
for i in range(1, 8):
    if arr[i-1] != arr[i]-1:
        flag1 = 0
    if arr[i-1] != arr[i]+1:
        flag2 = 0

if flag1:
    print("ascending")
elif flag2:
    print("descending")
else:
    print("mixeD")
