A = 0
a = int(input())
A += a * 3
a = int(input())
A += a * 2
a = int(input())
A += a * 1
B = 0
b = int(input())
B += b * 3
b = int(input())
B += b * 2
b = int(input())
B += b * 1
if A == B:
    print("T")
elif A > B:
    print("A")
else:
    print("B")
