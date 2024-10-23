import matplotlib.pyplot as plt


# 함수: 입력을 파싱하여 점 좌표 추출
def parse_polygon_input(input_str):
    lines = input_str.strip().split('\n')
    polygons = []

    for line in lines:
        tokens = line.split()
        coords = list(map(float, tokens[1:]))
        polygons.append(coords)

    return polygons


# 함수: 점을 그리는 함수
def plot_polygons(polygons):
    plt.figure(figsize=(6, 6))

    for polygon in polygons:
        x_coords = polygon[::2]  # 짝수 인덱스는 x좌표
        y_coords = polygon[1::2]  # 홀수 인덱스는 y좌표

        # 다각형을 그리기 위해 마지막 점이 첫 번째 점과 연결되도록 닫음
        x_coords.append(x_coords[0])
        y_coords.append(y_coords[0])

        # 다각형 그리기 (테두리만)
        plt.plot(x_coords, y_coords, 'bo-', linewidth=2)  # 'bo-'는 파란색 원 점과 실선

    plt.gca().set_aspect('equal', adjustable='box')  # 좌표 축 비율을 동일하게
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Polygons Plot')
    plt.show()


# 입력
a = """
4 0.000000 2.236068 4.472136 2.236068 4.472136 3.354102 0.000000 3.354102
4 4.472136 2.236068 8.944272 2.236068 8.944272 3.354102 4.472136 3.354102
4 0.000000 2.236068 8.944272 2.236068 8.944272 3.354102 0.000000 3.354102
5 0.000000 0.000000 8.944272 0.000000 8.944272 0.137453 1.253767 0.980581 0.000000 0.980581
3 0.000000 1.118034 8.944272 0.137453 8.944272 1.118034
3 0.000000 0.980581 1.253767 0.980581 0.000000 1.118034
5 0.000000 0.000000 8.944272 0.000000 8.944272 0.137453 1.253767 0.980581 0.000000 0.980581
3 1.253767 0.980581 10.198039 0.000000 10.198039 0.980581
3 8.944272 0.000000 10.198039 0.000000 8.944272 0.137453
4 0.000000 0.000000 10.198039 0.000000 10.198039 0.980581 0.000000 0.980581
"""

# 파싱 및 그리기
polygons = parse_polygon_input(a)
plot_polygons(polygons)
