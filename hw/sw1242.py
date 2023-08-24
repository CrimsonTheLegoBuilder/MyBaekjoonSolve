import sys
sys.stdin = open('sample_code_2.txt', 'r')


# def find():
#     for i in range(1, N):
#         for j in range(M-1, -1, -1):
#             if code[i-1][j] == '0' and code[i][j] != '0':
#                 scan(i, j)


# def scan(i, j):
#     for x in range(14, 71, 14):
#         temp = code[i][j-x:j+1]
#         if binary(temp):
#
#
#
#
# def binary(temp):
#     for i in range()


# def solve():


for tc in range(1, int(input())+1):
    N, M = map(int, input().strip().split())
    code = [list(map(str, input())) for _ in range(N)]
    ref = {
        '211': 0,
        '221': 1,
        '122': 2,
        '411': 3,
        '132': 4,
        '231': 5,
        '114': 6,
        '312': 7,
        '213': 8,
        '112': 9
    }
    h_to_b = {
        '0': '0000', '1': '0001', '2': '0010', '3': '0011', '4': '0100', '5': '0101', '6': '0110', '7': '0111',
        '8': '1000', '9': '1001', 'A': '1010', 'B': '1011', 'C': '1100', 'D': '1101', 'E': '1110', 'F': '1111'
    }

