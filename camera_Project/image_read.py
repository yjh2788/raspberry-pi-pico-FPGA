import serial

# 시리얼 포트 설정 (포트 이름을 실제 사용 중인 포트로 변경)
ser = serial.Serial('COM6', 9600)  # Windows

with open('output.txt', 'w') as outfile:
    while True:
        if ser.in_waiting > 0:
            data = ser.read(ser.in_waiting).decode('utf-8', errors='ignore')
            outfile.write(data)
            print(data, end='')  # 터미널에 출력

ser.close()