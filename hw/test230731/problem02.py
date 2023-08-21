############## 주의 ##############
# 입력을 받기위한 input 함수는 절대 사용하지 않습니다.
# 내장 함수 sum, len 함수를 사용하지 않습니다.
# 사용시 감점처리 되니 반드시 확인 바랍니다.
def calc_average(speed_list):
    cnt = 0                   # 속도를 위반한 차량들의 수
    speed_sum = 0             # 속도를 위반한 차량들의 속도의 합
    max_speed = 100           # 기준 속도
    for i in speed_list:
        if i > max_speed:     # 만일 기준 속도 100을 초과했다면?
            cnt += 1          # 1대 추가
            speed_sum += i  # 속도 추가
    ans = speed_sum / cnt     # 평균
    return ans
    # 여기에 코드를 작성하여 함수를 완성합니다.
    
    

# 아래 코드는 실행 확인을 위한 코드입니다.
if __name__ == '__main__':
    # 예시 코드는 수정하지 마세요.
    print(calc_average([119, 124, 178, 192,]))  #=> 153.25
    
    # 여기부터 아래에 추가 테스트를 위한 코드 작성 가능합니다.
    