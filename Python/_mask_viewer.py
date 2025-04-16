import os
import cv2
import numpy as np
import random
import matplotlib.pyplot as plt

IMG_DIR = "images"
LBL_DIR = "masks"  # YOLOv8 seg 포맷 폴더

# 랜덤 샘플 10장 선택
image_files = [f for f in os.listdir(IMG_DIR) if f.endswith((".jpg", ".png"))]
# sample_files = random.sample(image_files, min(10, len(image_files)))
sample_files = image_files

def draw_polygon_on_image(image, polygons):
    for poly in polygons:
        pts = np.array(poly, np.int32)
        pts = pts.reshape((-1, 1, 2))
        cv2.polylines(image, [pts], isClosed=True, color=(0, 255, 0), thickness=2)
    return image

for file in sample_files:
    name = os.path.splitext(file)[0]
    img_path = os.path.join(IMG_DIR, file)
    label_path = os.path.join(LBL_DIR, name + ".txt")

    if not os.path.exists(label_path):
        print(f"⚠️ 라벨 없음: {name}")
        continue

    image = cv2.imread(img_path)
    h, w = image.shape[:2]

    polygons = []

    with open(label_path, "r") as f:
        lines = f.readlines()

    for line in lines:
        parts = line.strip().split()
        if len(parts) < 7:
            continue  # polygon이 3쌍 미만이면 건너뜀

        # 클래스 ID는 무시, 나머지 좌표
        coords = list(map(float, parts[1:]))
        points = [(coords[i] * w, coords[i+1] * h) for i in range(0, len(coords), 2)]
        polygons.append(points)

    vis_img = draw_polygon_on_image(image.copy(), polygons)

    # OpenCV는 BGR → RGB로 변환해서 보여줍니다
    plt.figure(figsize=(6, 6))
    plt.imshow(cv2.cvtColor(vis_img, cv2.COLOR_BGR2RGB))
    plt.title(file)
    plt.axis("off")
    plt.tight_layout()
    plt.show()
