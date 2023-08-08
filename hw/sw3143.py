def boyer_moore(s1, s2, x):
    global cnt
    if B_M[ord(s1[x])] != -1:
        # print(B_M[ord(s1[x])])
        return B_M[ord(s1[x])]
    else:
        return len(s2)


def pattern_match(s1, s2, x):
    global cnt
    for o in range(len(s2)):
        if ord(s1[x - len(s2) + 1 + o]) != ord(s2[o]):
            return 0
    return 1


for tc in range(1, int(input())+1):
    A, B = map(str, input().split())
    B_M = [-1] * 128
    cnt = 0
    for i in range(len(B)-1, -1, -1):
        if B_M[ord(B[i])] == -1:
            B_M[ord(B[i])] = len(B) - i - 1
    # print(B_M)
    i = len(B) - 1
    # print(i)
    while i < len(A):
        i += boyer_moore(A, B, i)
        flag = pattern_match(A, B, i)
        # print(i)
        if flag:
            cnt += 1
            i += len(B)
        else:
            i += len(B)

    ans = len(A) - len(B)*cnt + cnt
    print(f'#{tc} {ans}')
