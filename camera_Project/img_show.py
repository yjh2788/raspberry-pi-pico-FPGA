import numpy as np
from PIL import Image

# 파일에서 데이터를 읽고 정렬
with open("raw data.txt", "r") as file:
    data = file.read()

# 데이터에서 값을 추출하고 정수로 변환
data_values = [int(val, 16) for val in data.split()]

# 이미지 해상도
width, height = 320, 240

# 필요한 픽셀 수
total_pixels = width * height

# 빈 배열을 0으로 초기화
image_array = np.zeros((height, width), dtype=np.uint16)

# 데이터를 배열에 채우기 (넘치는 데이터를 자르고, 부족한 데이터를 0으로 채움)
for i, value in enumerate(data_values):
    if i >= total_pixels:
        break
    row = i // width
    col = i % width
    image_array[row, col] = value

# RGB565 -> RGB888 변환
def rgb565_to_rgb888(value):
    r = ((value >> 11) & 0x1F) * 255 // 31
    g = ((value >> 5) & 0x3F) * 255 // 63
    b = (value & 0x1F) * 255 // 31
    return (r, g, b)

# 이미지 배열을 RGB888로 변환
image_rgb = np.zeros((height, width, 3), dtype=np.uint8)
for row in range(height):
    for col in range(width):
        image_rgb[row, col] = rgb565_to_rgb888(image_array[row, col])

# 이미지를 생성하고 저장
image = Image.fromarray(image_rgb)
image.save("output_image.png")

image.show()
