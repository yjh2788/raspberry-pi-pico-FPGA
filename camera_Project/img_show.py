import numpy as np
from PIL import Image
import cv2

    # image resolution
#width, height = 320, 240
width, height = 176, 144
def get_image_from_txtFile(path="raw data.txt",dtype=np.uint8):
    #read data from file
    with open(path, "r") as file:
        data = file.read()
    # extract pixel data
    data_values = [int(val, 16) for val in data.split()]
    #  pixel num
    total_pixels = width * height
    # empty array
    image_array = np.zeros((height, width), dtype=np.uint16)
    # data clipping
    for i, value in enumerate(data_values):
        if i >= total_pixels:
            break
        row = i // width
        col = i % width
        image_array[row, col] = value
        
    flat_data=image_array.flatten()
    flat_data=flat_data.reshape((-1, 1)).view(dtype).flatten()
    return flat_data

# RGB565 -> RGB888 
def rgb565_to_rgb8882(value):
    r = ((value >> 11) & 0x1F) * 255 // 31
    g = ((value >> 5) & 0x3F) * 255 // 63
    b = (value & 0x1F) * 255 // 31
    return (r, g, b)

def rgb565_to_rgb888(value):
    r = ((value & 0xf800) >> 11) * 255 // 31
    g = ((value & 0x07e0) >> 5) * 255 // 63
    b = (value & 0x001f) * 255 // 31
    return (r, g, b)


def RGB565_to_RGB888(img,height,width):
    img=img.reshape(height, width)
    image_rgb = np.zeros((height, width, 3), dtype=np.uint8)
    for row in range(height):
        for col in range(width):
            image_rgb[row, col] = rgb565_to_rgb888(img[row, col])
    return image_rgb

def split_4bit_values(value):
    # 16bit data -> divide every 4bits
    return [(value >> i) & 0xF for i in (12, 8, 4, 0)]

def split_8bit_values(value):
    # 16비트 값을 4비트씩 나눔
    return [(value >> i) & 0xF for i in (8, 0)]

# def yuyv_to_rgb(yuyv_data, width, height):
#     # convert 16bit data to 8bit data and divide YUYV data
#     yuyv_data_4bit = np.array([split_4bit_values(val) for val in yuyv_data], dtype=np.uint8)
#     yuyv_data_4bit=np.transpose(yuyv_data_4bit,(2,0,1))
    
#     # convert YUYV data to YUV422 format 
#     yuv422_data = np.zeros((height, width, 2), dtype=np.uint8)
#     yuv422_data[:, :, 0] = yuyv_data_4bit[:, :, 0] << 4  # Y0
#     yuv422_data[:, :, 1] = yuyv_data_4bit[:, :, 1] << 4  # U0
#     yuv422_data[:, 1::2, 0] = yuyv_data_4bit[:, 1::2, 2] << 4  # Y1
#     yuv422_data[:, 0::2, 1] = yuyv_data_4bit[:, 0::2, 3] << 4  # V0

#     # YUV422 to RGB
#     rgb_data = cv2.cvtColor(yuv422_data, cv2.COLOR_YUV2RGB_YUY2)
#     return rgb_data

def yuv422_to_rgb(yuv422_data, width, height):
    # Reshape the flat data to (height, width * 2) for processing
    yuv422_data = np.reshape(yuv422_data, (height, width * 2))

    # Extract Y, U, V channels from YUV422 data
    y0 = yuv422_data[:, 0::4]
    u = yuv422_data[:, 1::4]
    y1 = yuv422_data[:, 2::4]
    v = yuv422_data[:, 3::4]

    # Combine Y0 and Y1 to form the Y channel
    y = np.zeros((height, width), dtype=np.uint8)
    y[:, 0::2] = y0
    y[:, 1::2] = y1
    y=yuv422_data[:, 0::2]

    # Upsample U and V to the full image size
    u = np.repeat(u, 2, axis=1)
    v = np.repeat(v, 2, axis=1)

    # YUV channel combine 
    yuv = np.stack((y, u, v), axis=-1)

    # YUV to RGB
    rgb_image = cv2.cvtColor(yuv, cv2.COLOR_YUV2RGB)

    return rgb_image
 
if __name__ == "__main__": 
    image_from_txt=get_image_from_txtFile("output.txt");#"output.txt"
    #RGB888=RGB565_to_RGB888(image_from_txt)
    #RGB888 = Image.fromarray(RGB888)
    #RGB888.save("output_image.png")
    #RGB888.show()
    RGB888=yuv422_to_rgb(image_from_txt,width, height)
    cv2.imshow("YUV2RGB",RGB888);   
        
