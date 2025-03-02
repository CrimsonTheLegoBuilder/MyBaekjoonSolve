import sys
INPUT = sys.stdin.readline

A = INPUT().strip()
B = INPUT().strip()
A_ = A.lower()
B_ = B.lower()

if A == "null":
    print("NullPointerException\nNullPointerException")
    exit(0)

elif B == "null":
    print("false\nfalse")

elif A == B:
    print("true\ntrue")

elif A_ == B_:
    print("false\ntrue")

else:
    print("false\nfalse")
