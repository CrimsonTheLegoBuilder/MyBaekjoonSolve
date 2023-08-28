for tc in range(1, int(input())+1):
    N, M = map(int, input().split())  # N은 학생 수, M은 과목 수
    students = [tuple(map(int, input().split())) for _ in range(N)]  # 각 학생들이 제출한 답안
    answer = tuple(map(int, input().split()))  # 실제 답지
    score = [0] * N  # 각 학생들의 점수
    for i in range(N):  # 채점 들어갑니다~
        temp = 1  # 1점부터 시작
        for j in range(M):
            if students[i][j] == answer[j]:  # 답을 맞췄다면?
                score[i] += temp  # 이 문항의 점수는 temp 점
                temp += 1  # temp 보너스 붙음
            else:
                temp = 1  # 틀리면 다시 1
    print(f'#{tc} {max(score) - min(score)}')  # 최고 점수와 최소 점수의 차이를 출력
