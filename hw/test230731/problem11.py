############## 주의 ##############
# 입력을 받기위한 input 함수는 절대 사용하지 않습니다.
# 내장 함수 sum, min, max, len 함수를 사용하지 않습니다.
# 사용시 감점처리 되니 반드시 확인 바랍니다.
def get_row_col_maxsum(matrix):
    length_r = 0                # 행의 크기
    length_c = 0                # 열의 크기
    flag = 0                    # 최대값이 나온 줄이 행인지 열인지 판단
    ans = -int(10e10)           # 설마 이거보다 작은 수가 최대값이겠나
    for _ in matrix:            # 행의 크기를 구해보자
        length_r += 1
    for _ in matrix[0]:         # 열의 크기를 구해보자. 
        length_c += 1           # 직사각행렬임이 보장된다고 했다. 0번째만 보면 되겠지.
    
    for j in range(length_c):      # 특정 열의 합이 가장 큰 지부터 살펴보자.
        total = 0                  # 매순간 합계를 초기화
        for i in range(length_r):  
            total += matrix[i][j]  # 특정 행의 모든 j번째 숫자를 더하기
        if total > ans:            # 만일 다 더한 수가 이전 합보다 크다면?
            ans = total            # 이 수가 제일 크네
            flag = 1               # 그리고 이 합계는 열에서 나온거야
    
    for i in range(length_r):      # 이번에는 특정 행의 합을 보자
        total = 0
        for j in range(length_c):
            total += matrix[i][j]
        if total > ans:
            ans = total
            flag = 0               # 이 합계는 행에서 나왔음.

    if flag:
        return 'col', ans
    else:
        return 'row', ans
    # 여기에 코드를 작성하여 함수를 완성합니다.
    


# 아래 코드는 실행 확인을 위한 코드입니다.
if __name__ == '__main__':
    # 예시 코드는 수정하지 마세요.
    example_matrix = [
        [1, 2, 3, 4],
        [5, 6, 7, 8],
        [9, 10, 11, 12],
        [13, 14, 15, 16]
    ]

    example_matrix2 = [
        [11, 5, 49, 20],
        [28, 16, 20, 33],
        [63, 17, 1, 15]
    ]
    print(get_row_col_maxsum(example_matrix))   # => ('row', 58)
    print(get_row_col_maxsum(example_matrix2))  # => ('col', 102)

    # 여기부터 아래에 추가 테스트를 위한 코드 작성 가능합니다.
    