############## 주의 ##############
# 입력을 받기위한 input 함수는 절대 사용하지 않습니다.
# 내장 함수 sum 함수를 사용하지 않습니다.
# 사용시 감점처리 되니 반드시 확인 바랍니다.
def sum_primes(number):
    start = 2                  # 소수는 2부터 시작한다.
    ans = 0                    # 답을 담을 그릇
    except_num = 17            # 제외할 숫자는 17
    prime = [1] * (number + 1) # 에라토스테네스의 체
    for i in range(2, int(number**.5) + 1):   # 에라토스테네스의 체 만들기 시작
        if prime[i]:           # 만일 2부터 1이 켜져있다면?
            for j in range(i + i, number + 1, i):  
                prime[j] = 0   # 배수는 전부 끈다
    for k in range(start, number):   # 2부터 합산 시작
        if k == except_num:          # 17은 제외
            continue
        if prime[k]:                 # k가 만일 소수라면?
            ans += k                 # 더한다
    return ans
    # 여기에 코드를 작성하여 함수를 완성합니다.
    


# 아래 코드는 실행 확인을 위한 코드입니다.
if __name__ == '__main__':
    # 예시 코드는 수정하지 마세요.
    print(sum_primes(22)) # => 60
    print(sum_primes(33)) # => 143
    # 여기부터 아래에 추가 테스트를 위한 코드 작성 가능합니다.