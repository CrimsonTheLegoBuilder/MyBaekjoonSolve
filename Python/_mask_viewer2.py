import os
import cv2
import numpy as np
import matplotlib.pyplot as plt

IMG_DIR = "images"
LBL_DIR = "labels"
EPSILONS = np.linspace(0.001, 0.05, 20)

# 사용자로부터 파일 이름 입력
file = input("테스트할 이미지 파일 이름 (확장자 포함): ").strip()
name = os.path.splitext(file)[0]
img_path = os.path.join(IMG_DIR, file)
label_path = os.path.join(LBL_DIR, name + ".txt")

if not os.path.exists(label_path) or not os.path.exists(img_path):
    print("⚠️ 해당 이미지 또는 라벨 파일이 존재하지 않습니다.")
    exit()

image = cv2.imread(img_path)
h, w = image.shape[:2]

with open(label_path, "r") as f:
    lines = f.readlines()

# 라벨에서 첫 번째 polygon만 가져오기
target_polygon = None
for line in lines:
    parts = line.strip().split()
    if len(parts) < 7:
        continue
    coords = list(map(float, parts[1:]))
    points = [(coords[i] * w, coords[i+1] * h) for i in range(0, len(coords), 2)]
    target_polygon = np.array(points, dtype=np.float32).reshape((-1, 1, 2))
    break

if target_polygon is None:
    print("⚠️ 사용할 수 있는 polygon이 없습니다.")
    exit()

# 여러 epsilon 값에 대해 단순화
fig, axs = plt.subplots(4, 5, figsize=(18, 14))
axs = axs.flatten()

for idx, epsilon_ratio in enumerate(EPSILONS):
    epsilon = epsilon_ratio * cv2.arcLength(target_polygon, True)
    approx = cv2.approxPolyDP(target_polygon, epsilon, True)
    vis_img = image.copy()

    if len(approx) >= 3:
        cv2.polylines(vis_img, [approx], isClosed=True, color=(0, 255, 0), thickness=2)

    axs[idx].imshow(cv2.cvtColor(vis_img, cv2.COLOR_BGR2RGB))
    axs[idx].set_title(f"epsilon={epsilon_ratio:.3f} ({len(approx)} pts)")
    axs[idx].axis("off")

plt.suptitle(f"Polygon Simplification Sweep for {file}", fontsize=16)
plt.tight_layout()
plt.show()
