import os
import cv2
import numpy as np
import matplotlib.pyplot as plt

IMG_DIR = "images"
LBL_DIR = "masks_txt_nukki_contrast"
EPSILONS = np.linspace(0.001, 0.05, 20)

# file = "H_A_V_02_0910144019332.jpg"
file = "H_A_V_02_0910144227242.jpg"
name = os.path.splitext(file)[0]
img_path = os.path.join(IMG_DIR, file)

if not os.path.exists(img_path):
    print("⚠️ 이미지 파일이 존재하지 않습니다.")
    exit()

image = cv2.imread(img_path)
h, w = image.shape[:2]

# 시각화 준비
fig, axs = plt.subplots(4, 5, figsize=(20, 16))
axs = axs.flatten()

# 20개 라벨 파일 읽기 및 시각화
for idx in range(len(EPSILONS)):
    label_path = os.path.join(LBL_DIR, f"{name}_eps{idx:02d}.txt")
    if not os.path.exists(label_path):
        print(f"❌ 라벨 파일 없음: {label_path}")
        continue

    vis_img = image.copy()
    with open(label_path, "r") as f:
        lines = f.readlines()

    for line in lines:
        parts = line.strip().split()
        if len(parts) < 7:
            continue
        coords = list(map(float, parts[1:]))
        points = [(coords[i] * w, coords[i+1] * h) for i in range(0, len(coords), 2)]
        polygon = np.array(points, dtype=np.int32).reshape((-1, 1, 2))
        cv2.polylines(vis_img, [polygon], isClosed=True, color=(0, 255, 0), thickness=2)

    axs[idx].imshow(cv2.cvtColor(vis_img, cv2.COLOR_BGR2RGB))
    axs[idx].set_title(f"epsilon={EPSILONS[idx]:.3f}")
    axs[idx].axis("off")

plt.suptitle(f"Polygon Visualization Sweep for {file}", fontsize=18)
plt.tight_layout()
plt.show()
