from PIL import Image
import numpy as np


def image_to_cpp_rgb_array_with_padding(image_path, target_width=64, target_height=32, padding_color=(255, 255, 255)):
    # 이미지를 열어서 작업합니다.
    with Image.open(image_path) as img:
        # 이미지 비율을 유지하면서 리사이즈
        img.thumbnail((target_width, target_height))

        # 빈 캔버스 생성 (64x32)
        new_img = Image.new("RGB", (target_width, target_height), padding_color)

        # 이미지를 중앙에 배치
        offset_x = (target_width - img.width) // 2
        offset_y = (target_height - img.height) // 2
        new_img.paste(img, (offset_x, offset_y))

        # 픽셀 데이터를 가져와 numpy 배열로 변환 (RGB 값)
        pixel_data = np.array(new_img)

        # 3D 배열(64x32x3)을 1D 배열로 변환
        flat_pixel_data = pixel_data.flatten()

        # C++ 배열 형식으로 변환
        cpp_array = "const unsigned char wifi_image_rgb[] PROGMEM = {\n  "
        cpp_array += ','.join(f"0x{pixel:02x}" for pixel in flat_pixel_data)
        cpp_array += "\n};"

        return cpp_array


# 이미지 파일 경로
image_path = 'now_driving.png'  # 여기에 이미지 파일 경로 입력
# 함수를 호출하여 C++ 배열 생성
cpp_rgb_array_code = image_to_cpp_rgb_array_with_padding(image_path)

# C++ 코드 출력
print(cpp_rgb_array_code)
