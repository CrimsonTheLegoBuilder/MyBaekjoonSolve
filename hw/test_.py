import sys; sys.stdin = open('input_keyword.txt', 'r')
# T = int(input())
# for tc in range(1, T+1):
#     N, K = map(int, input().split())
#     arr = [list(map(int, input().split())) for _ in range(N)]
#     possible = 0
#     for i in range(N):
#         cnt = 0
#         for j in range(N):
#             if j+1 >= N or arr[i][j+1] == 0:
#                 if cnt == K:  # 앞이나 뒤가 1인 경우가 고려되고 있지 않음
#                     possible += 1
#                     cnt = 0  # 초기화 과정 같은데 if 문 밖으로 빠져나옴
#             if arr[i][j]:
#                 cnt += 1
#     for j in range(N):
#         cnt = 0
#         for i in range(N):
#             if arr[i][j] == 0:
#                 if cnt == K:
#                     possible += 1
#                     cnt = 0  # 초기화 과정 같은데 if 문 밖으로 빠져나옴
#             else:
#                 cnt += 1
#
#     print(f'#{tc} {possible}')


# import sys; sys.stdin = open('word.txt')
T = int(input())
for tc in range(1, T+1):
    N, K = map(int, input().split())
    arr = [list(map(int, input().split())) for _ in range(N)]
    possible = 0

    for i in range(N):
        cnt = 0
        for j in range(N):
            if arr[i][j] == 0:
                if cnt == K:
                    possible += 1
                cnt = 0
            else:
                cnt += 1
        if cnt == K:
            possible += 1

    for j in range(N):
        cnt = 0
        for i in range(N):
            if arr[i][j] == 0:
                if cnt == K:
                    possible += 1
                cnt = 0
            else:
                cnt += 1
        if cnt == K:
            possible += 1

    print(f'#{tc} {possible}')