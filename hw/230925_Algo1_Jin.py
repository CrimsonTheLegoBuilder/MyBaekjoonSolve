def d2x(d):  # 10진법을 16진법으로
    return str(d) if 0 <= d <= 9 else chr(d+55)


def solve(order, k):
    ky = int(k, 16)
    tmp = ''
    for o in order:
        tmp += d2x(int(o, 16) ^ ky)  # 암호를 정수형으로 읽어들여 key 와 toggle
    return tmp


for tc in range(int(input())):
    _ = input()   # 암호문의 길이
    OD = input()  # 암호문
    K = input()   # 암호문의 key
    print(f'#{tc+1} {solve(OD, K)}')


# def x2d(x):  # 16진법을 10진법으로
#     return ord(x)-55 if 'A' <= x <= 'F' else int(x)
#
#
# def x2d_o(n, order):               # 모든 문자열 전체에 대해 16진법을 10진법으로
#     num = [0] * n
#     for j, o in enumerate(order):  # 각 인덱스와 문자를 생성
#         num[j] = x2d(o)            # 16진법을 10진법으로
#     return num
#
#
# def d2x(d):  # 10진법을 16진법으로
#     return str(d) if 0 <= d <= 9 else chr(d+55)
#
#
# def d2x_o(num, k):              # 모든 암호문에 대해 10진법을 16진법으로
#     tmp = ''
#     for j in range(N):
#         tmp += d2x(num[j] ^ k)  # 10진법을 16진법으로 암호 해독
#     return tmp
#
#
# for tc in range(int(input())):
#     N = int(input())  # 암호문의 길이
#     OD = input()      # 암호문
#     K = x2d(input())  # 암호문의 key
#     print(f'#{tc+1} {d2x_o(x2d_o(N, OD), K)}')
