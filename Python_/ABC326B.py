def find(X):
    while 1:
        if int(X[0]) * int(X[1]) == int(X[2]):
            print(X)
            return
        X = str(int(X) + 1)


A = input()
find(A)