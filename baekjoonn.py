A = [list(input()) for _ in range(5)]
len1 = 0

for i in range(5):
    if len(A[0:][i]) > len1:
         len1 = len(A[0:][i])

print(A)
print(len1)
answer = []

for k in range(len1):
    for j in range(5):
        answer.append(A[j][k])
# test
print(answer)
