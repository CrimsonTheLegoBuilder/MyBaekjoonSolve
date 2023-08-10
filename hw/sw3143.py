def boyer_moore(s1, s2, x):
    if B_M[ord(s1[x])] != -1:
        return B_M[ord(s1[x])]
    else:
        return len(s2)


def pattern_match(s1, s2, x):
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
    i = len(B) - 1
    while i < len(A):
        i += boyer_moore(A, B, i)
        if i >= len(A):
            break
        flag = pattern_match(A, B, i)
        if flag:
            cnt += 1
            i += len(B)
        else:
            i += 1

    ans = len(A) - len(B)*cnt + cnt
    print(f'#{tc} {ans}')
