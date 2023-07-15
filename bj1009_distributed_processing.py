import sys

# arr1 = [1]
arr2 = [6, 2, 4, 8]
arr3 = [1, 3, 9, 7]
arr4 = [6, 4]
# arr5 = [5]
# arr6 = [6]
arr7 = [1, 7, 9, 3]
arr8 = [6, 8, 4, 2]
arr9 = [1, 9]

for _ in range(int(sys.stdin.readline().strip())):
    a, b = map(str, sys.stdin.readline().strip().split())

    if a[-1] == '0':
        print(10)
    elif a[-1] == '1' or a[-1] == '5' or a[-1] == '6':
        print(int(a[-1]))
    elif a[-1] == '2':
        print(arr2[int(b) % 4])
    elif a[-1] == '3':
        print(arr3[int(b) % 4])
    elif a[-1] == '4':
        print(arr4[int(b) % 2])
    elif a[-1] == '7':
        print(arr7[int(b) % 4])
    elif a[-1] == '8':
        print(arr8[int(b) % 4])
    elif a[-1] == '9':
        print(arr9[int(b) % 2])
