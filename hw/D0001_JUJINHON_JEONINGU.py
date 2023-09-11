import socket
import time
import math

# 닉네임을 사용자에 맞게 변경해 주세요.
NICKNAME = '귀염뽀짝먼지뭉팅이'

# 일타싸피 프로그램을 로컬에서 실행할 경우 변경하지 않습니다.
HOST = '127.0.0.1'

# 일타싸피 프로그램과 통신할 때 사용하는 코드값으로 변경하지 않습니다.
PORT = 1447
CODE_SEND = 9901
CODE_REQUEST = 9902
SIGNAL_ORDER = 9908
SIGNAL_CLOSE = 9909


# 게임 환경에 대한 상수입니다.
TABLE_WIDTH = 254
TABLE_HEIGHT = 127
NUMBER_OF_BALLS = 6
HOLES = [[0, 0], [127, 0], [254, 0], [0, 127], [127, 127], [254, 127]]

order = 0
balls = [[0, 0] for i in range(NUMBER_OF_BALLS)]

sock = socket.socket()
print('Trying to Connect: %s:%d' % (HOST, PORT))
sock.connect((HOST, PORT))
print('Connected: %s:%d' % (HOST, PORT))

send_data = '%d/%s' % (CODE_SEND, NICKNAME)
sock.send(send_data.encode('utf-8'))
print('Ready to play!\n--------------------')


while True:

    # Receive Data
    recv_data = (sock.recv(1024)).decode()
    print('Data Received: %s' % recv_data)

    # Read Game Data
    split_data = recv_data.split('/')
    idx = 0
    try:
        for i in range(NUMBER_OF_BALLS):
            for j in range(2):
                balls[i][j] = float(split_data[idx])
                idx += 1
    except:
        send_data = '%d/%s' % (CODE_REQUEST, NICKNAME)
        print("Received Data has been currupted, Resend Requested.")
        continue

    # Check Signal for Player Order or Close Connection
    if balls[0][0] == SIGNAL_ORDER:
        order = int(balls[0][1])
        print('\n* You will be the %s player. *\n' % ('first' if order == 1 else 'second'))
        continue
    elif balls[0][0] == SIGNAL_CLOSE:
        break

    # Show Balls' Position
    print('====== Arrays ======')
    for i in range(NUMBER_OF_BALLS):
        print('Ball %d: %f, %f' % (i, balls[i][0], balls[i][1]))
    print('====================')

    angle = 0.0
    power = 0.0

    ##############################
    # 이 위는 일타싸피와 통신하여 데이터를 주고 받기 위해 작성된 부분이므로 수정하면 안됩니다.
    #
    # 모든 수신값은 변수, 배열에서 확인할 수 있습니다.
    #   - order: 1인 경우 선공, 2인 경우 후공을 의미
    #   - balls[][]: 일타싸피 정보를 수신해서 각 공의 좌표를 배열로 저장
    #     예) balls[0][0]: 흰 공의 X좌표
    #         balls[0][1]: 흰 공의 Y좌표
    #         balls[1][0]: 1번 공의 X좌표
    #         balls[4][0]: 4번 공의 X좌표
    #         balls[5][0]: 마지막 번호(8번) 공의 X좌표

    # 여기서부터 코드를 작성하세요.
    # 아래에 있는 것은 샘플로 작성된 코드이므로 자유롭게 변경할 수 있습니다.

    '''
    PT99990099
    SYNP-SIIR-ASCJ-FAET
    '''

    def cross(d1, d2, d3, d4):
        return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


    def dot(d1, d2, d3, d4):
        return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


    def cal_length(d1, d2):
        return (d2[0] - d1[0])**2 + (d2[1] - d1[1])**2


    def make_vector(d1, d2):
        return d2[0] - d1[0], d2[1] - d1[1]


    def cal_dist(d1, d2, d3):
        torque_ = abs(cross(d1, d2, d2, d3))
        length = cal_length(d1, d2)**.5
        return torque_ / length


    def is_collide(d1, d2, d3):
        dist = cal_dist(d1, d2, d3)
        plus = dot(d1, d3, d3, d2) > 0
        return dist < D + 0.01 and plus


    def hit_pos(d1):
        coord = []
        for h_ in HOLES:
            vx_, vy_ = make_vector(h_, d1)
            length = cal_length(d1, h_)
            x, y = d1
            x1 = x + vx_ * (D / length)
            y1 = y + vy_ * (D / length)
            flag = 1
            for l1, l2 in boundary:
                if cal_dist(l1, l2, d1) < D:
                    flag = 0
            for i in range(1, 6):
                if dot((x1, y1), balls[i], balls[i], h_) > 0 and is_collide((x1, y1), h_, balls[i]) < D + 0.01:
                    flag = 0
            if flag:
                coord.append((x1, y1))
        for i in range(6):
            pass
        return coord


    def is_cross(d1, d2, d3, d4):
        flag1 = cross(d1, d2, d2, d3) * cross(d2, d1, d1, d4)
        flag2 = cross(d3, d4, d4, d1) * cross(d4, d3, d3, d2)
        return flag1 > 0 and flag2 > 0


    def crs_pos(d1, d2):
        d_ = d1
        for i in range(6):
            d3, d4 = boundary[i]
            if is_cross(d1, d2, d3, d4):
                if d3[0] - d4[0] == 0 and d1[1] - d2[1] == 0:
                    x = d3[0]
                    y = d1[1]
                    d_ = x, y
                elif d1[0] - d2[0] == 0 and d3[1] - d4[1] == 0:
                    x = d1[0]
                    y = d3[1]
                    d_ = x, y
                elif d3[0] - d4[0] == 0:
                    x = d3[0]
                    y = d1[1] + (d1[1] - d2[1]) * (d3[0] - d1[0]) / (d1[0] - d2[0])
                    d_ = x, y
                elif d3[1] - d4[1] == 0:
                    x = d1[0] + (d1[0] - d2[0]) * (d3[1] - d1[1]) / (d1[1] - d2[1])
                    y = d3[1]
                    d_ = x, y
                if dot(d1, d_, d_, d2) > 0:
                    return d_
        return


    def one_cushion_t2h(j):
        candidate = []
        d1 = balls[j]
        x, y = d1
        d2 = 508 - x - D, -y
        d_ = crs_pos(d1, d2)
        flag = 1
        if d_ is not None:
            for i in range(1, 6):
                if i != j:
                    if is_collide(d1, d_, balls[i]) and is_collide((0, 0), d_, balls[i]):
                        flag = 0
        if flag:
            vx, vy = make_vector(d2, d1)
            l_ = cal_length(d2, d1) ** .5
            d__ = x + vx * (D / l_), y + vy * (D / l_)
            candidate.append(d__)
        d2 = -x, 254 - y - D
        d_ = crs_pos(d1, d2)
        flag = 1
        if d_ is not None:
            for i in range(1, 6):
                if i != j:
                    if is_collide(d1, d_, balls[i]) and is_collide((0, 0), d_, balls[i]):
                        flag = 0
        if flag:
            vx, vy = make_vector(d2, d1)
            l_ = cal_length(d2, d1) ** .5
            d__ = x + vx * (D / l_), y + vy * (D / l_)
            candidate.append(d__)

        return


    tol = 0.5
    HOLES_ = [[0+tol, 0+tol], [127, 0+tol], [254-tol, 0+tol], [0+tol, 127-tol], [127, 127-tol], [254-tol, 127-tol]]
    boundary = (((5.73, 2.865), (121.27, 2.865)), ((132.73, 2.865), (248.27, 2.865)), ((251.135, 5.73), (251.135, 121.27)),
                ((5.73, 121.27), (121.27, 121.27)), ((132.73, 121.27), (248.27, 121.27)), ((2.865, 5.73), (2.865, 121.27)))
    target = ((1, 3, 5), (2, 4, 5))
    D = 5.73      # 공의 직경
    C = balls[0]  # H: 흰 공 cue
    x, y = C      # x, y: 흰 공의 X, Y 좌표를 나타내기 위해 사용한 변수
    angles = []   # 흰 공을 칠 수 있는 모든 각도
    cnt = 0       # 8번 공을 제외한 모든 공을 넣었는지 판단하는 변수 cnt

    for i in target[(order + 1) % 2][:-1]:  # 8번 공을 제외한 모든 공에 대해서 내 공을 판단한다
        T = balls[i]
        x_, y_ = T
        print(T)
        if x_ < 0.0 and y_ < 0.0:    # 공이 자리에 없으면 넘어감
            # if i == 1 or i == 3:      # 내 공인데 들어갔다면?
            cnt += 1
            continue
        for H in HOLES_:              # 모든 구멍에 대해서 탐색
            if dot(C, T, T, H) > 0:   # 흰 공 - 목적구 - 구멍 내적이 양수일 경우는 이 순서대로 줄을 선 경우 - 1큐에 넣을 수 있는 경우
                # print(C, T, H)
                vx2, vy2 = make_vector(H, T)  # 구멍으로부터 목적구로 벡터를 구함
                l_ = cal_length(H, T)         # 목적구까지의 거리
                ratio = D / l_                # 공의 직경 / 목적구로부터 구멍의 거리
                C_ = x_ + vx2 * ratio, y_ + vy2 * ratio  # 흰 공이 맞춰야할 목적구의 타점
                # print(C_)
                x__, y__ = C_
                t1 = 90.0 - math.degrees(math.atan2(y__ - y, x__ - x))  # 어째선지 각도가 y축에 대해 CW로 정렬되어있다.
                # print(t1)
                l = cal_length(C, C_)          # 흰 공과 타점과의 거리
                # print('dist', l**.5)
                torque = cross(C, C_, C_, H)   # 토크. 입사각을 구하기 위한 계산과정.
                if l <= 0.0:
                    l = math.inf  # Prevent ZeroDivisionError
                theta = math.atan(abs(torque / l))  # 입사각
                # flag = 1
                # for j in target[order % 2]:
                #     if is_collide(C, C_, balls[j]) or is_collide(C_, H, balls[j]):
                #         flag = 0
                # if flag:
                angles.append((theta, t1, l_**.5 + l**.5))  # 입사각, 공을 쳐야할 각도, 힘을 구하기 위한 거리를 튜플로 묶는다.
        print(cnt)
    if cnt == 2:
        T = balls[-1]
        x_, y_ = T
        if x_ < 0.0 and y_ < 0.0:
            cnt += 1
            continue
        for H in HOLES_:
            if dot(C, T, T, H) > 0:
                # print(C, T, H)
                vx2, vy2 = make_vector(H, T)
                l_ = cal_length(H, T)
                ratio = D / l_
                C_ = x_ + vx2 * ratio, y_ + vy2 * ratio
                # print(C_)
                x__, y__ = C_
                t1 = 90.0 - math.degrees(math.atan2(y__ - y, x__ - x))
                # print(t1)
                l = cal_length(C, C_)
                # print('dist', l ** .5)
                torque = cross(C, C_, C_, H)
                if l <= 0.0:
                    l = math.inf
                theta = math.atan(abs(torque / l))
                # flag = 1
                # for j in target[order]:
                #     if is_collide(C, C_, balls[j]) or is_collide(C_, H, balls[j]):
                #         flag = 0
                # if flag:
                angles.append((theta, t1, l_**.5 + l**.5))  # 입사각, 공을 쳐야할 각도, 힘을 구하기 위한 거리를 튜플로 묶는다.

    angles.sort()  # 입사각이 제일 작은 타점이 최적해일 확률이 높음. 정렬.
    t, angle, distance = angles[0]  # 언패킹
    print(t, math.degrees(t))
    # power: 거리 distance 에 따른 힘의 세기를 계산
    # power = distance * .25 * (math.pi - 2*t)
    power = distance * .68 * (math.pi - 2*t)
    # power = 100
    print(power)

    # 주어진 데이터(공의 좌표)를 활용하여 두 개의 값을 최종 결정하고 나면,
    # 나머지 코드에서 일타싸피로 값을 보내 자동으로 플레이를 진행하게 합니다.
    #   - angle: 흰 공을 때려서 보낼 방향(각도)
    #   - power: 흰 공을 때릴 힘의 세기
    # 
    # 이 때 주의할 점은 power는 100을 초과할 수 없으며,
    # power = 0인 경우 힘이 제로(0)이므로 아무런 반응이 나타나지 않습니다.
    #
    # 아래는 일타싸피와 통신하는 나머지 부분이므로 수정하면 안됩니다.
    ##############################

    merged_data = '%f/%f/' % (angle, power)
    sock.send(merged_data.encode('utf-8'))
    print('Data Sent: %s' % merged_data)

sock.close()
print('Connection Closed.\n--------------------')