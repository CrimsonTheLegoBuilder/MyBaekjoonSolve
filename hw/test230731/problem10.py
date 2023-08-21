############## 주의 ##############
# 입력을 받기위한 input 함수는 절대 사용하지 않습니다.
# 내장 함수 sum, min, max, len 함수를 사용하지 않습니다.
# 사용시 감점처리 되니 반드시 확인 바랍니다.
def find_one(matrix):
    length = 0
    for _ in matrix:              # 크기부터 구하고 시작하면 되지 ㅋㅋㅋㅋㅋㅋㅋ
        length += 1
    for i in range(length):       # 2차원 배열을 조회해보자
        for j in range(length):   # 하나씩 하나씩
            if matrix[i][j] == 1: # 1이 존재한다면?
                return i, j       # 문제에서는 하나라고 했으니까 바로 탈출
    return "'1' doesn`t exist."   # 1이 없을 수도 있다
    # 여기에 코드를 작성하여 함수를 완성합니다.
            


# 아래 코드는 실행 확인을 위한 코드입니다.
if __name__ == '__main__':
    # 예시 코드는 수정하지 마세요.
    sample_matrix = [
      [0, 0, 0],
      [0, 1, 0],
      [0, 0, 0]
    ]
    print(find_one(sample_matrix))  # => (1, 1)
    # 여기부터 아래에 추가 테스트를 위한 코드 작성 가능합니다.
    