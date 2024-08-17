import matplotlib.pyplot as plt

# 파란 점(B)과 빨간 점(R)의 좌표 리스트
B = [(1, 4), (5, 4), (9, 3)]
R = [(7, 7), (11, 7), (15, 6), (7, 1), (10, 0), (15, 1)]

# B와 R의 x, y 좌표 분리
B_x, B_y = zip(*B)
R_x, R_y = zip(*R)

# 그래프 그리기
plt.figure(figsize=(8, 6))
plt.scatter(B_x, B_y, color='blue', label='Blue Points (B)')
plt.scatter(R_x, R_y, color='red', label='Red Points (R)')

# B 점에 번호 붙이기
for i, (x, y) in enumerate(B):
    plt.text(x, y, f'B{i+1}', fontsize=12, ha='right', color='blue')

# R 점에 번호 붙이기
for i, (x, y) in enumerate(R):
    plt.text(x, y, f'R{i+1}', fontsize=12, ha='right', color='red')

# 그래프 설정
plt.title('Scatter Plot of Blue and Red Points with Labels')
plt.xlabel('X')
plt.ylabel('Y')
plt.legend()
plt.grid(True)

# 그래프 출력
plt.show()
