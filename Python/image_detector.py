import cv2
import numpy as np

def calculate_image_difference(image1, image2):
    # 이미지를 그레이스케일로 변환
    gray1 = cv2.cvtColor(image1, cv2.COLOR_BGR2GRAY)
    gray2 = cv2.cvtColor(image2, cv2.COLOR_BGR2GRAY)

    # 이미지 차이 계산
    difference = cv2.absdiff(gray1, gray2)

    # 차이의 평균 계산
    mean_difference = np.mean(difference)

    return mean_difference

# 이미지 로드
image1 = cv2.imread('image1.jpg')
image2 = cv2.imread('image2.jpg')

# 이미지 비교
difference = calculate_image_difference(image1, image2)

print(f"두 이미지 간의 차이: {difference}")



# import numpy as np
# import tensorflow as tf
# from tensorflow.keras.models import Sequential
# from tensorflow.keras.layers import Conv1D, MaxPooling1D, Flatten, Dense, Dropout
#
# # 가상의 라이다 스캔 데이터 생성 (실제 데이터를 사용해야 함)
# def generate_fake_lidar_data(num_frames=100, num_points_per_frame=360):
#     return np.random.rand(num_frames, num_points_per_frame)
#
# # 데이터 전처리
# def preprocess_data(data):
#     # 데이터를 정규화하거나 다른 전처리 과정을 수행
#     normalized_data = (data - np.min(data)) / (np.max(data) - np.min(data))
#     return normalized_data
#
# # 라벨 생성 (가상의 라벨 데이터, 실제 데이터에 대한 라벨을 생성해야 함)
# def generate_labels(num_frames, threshold=0.5):
#     # 임의의 임계값을 기준으로 움직임 여부를 나타내는 라벨 생성
#     return np.random.rand(num_frames) > threshold
#
# # 데이터 및 라벨 생성
# lidar_data = generate_fake_lidar_data()
# labels = generate_labels(len(lidar_data))
#
# # 데이터 전처리
# lidar_data = preprocess_data(lidar_data)
#
# # 딥러닝 모델 구성
# model = Sequential()
# model.add(Conv1D(32, 3, activation='relu', input_shape=(lidar_data.shape[1], 1)))
# model.add(MaxPooling1D(2))
# model.add(Conv1D(64, 3, activation='relu'))
# model.add(MaxPooling1D(2))
# model.add(Flatten())
# model.add(Dense(64, activation='relu'))
# model.add(Dropout(0.5))
# model.add(Dense(1, activation='sigmoid'))
#
# # 모델 컴파일
# model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])
#
# # 데이터 형태 조정 (CNN 모델을 사용하기 위해 3D 형태로 조정)
# lidar_data_3d = lidar_data.reshape(lidar_data.shape[0], lidar_data.shape[1], 1)
#
# # 모델 훈련
# model.fit(lidar_data_3d, labels, epochs=10, batch_size=32, validation_split=0.2)
#
# # 모델을 사용하여 움직임 감지
# predictions = model.predict(lidar_data_3d)
# predicted_labels = (predictions > 0.5).flatten()
#
# # 예측 결과 출력
# print("Ground Truth Labels:", labels)
# print("Predicted Labels:", predicted_labels)