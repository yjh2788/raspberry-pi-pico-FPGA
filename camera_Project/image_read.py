import serial

# 시리얼 포트 설정 (포트 이름을 실제 사용 중인 포트로 변경)
ser = serial.Serial('COM6', 9600)  # Windows

with open('output.txt', 'w') as outfile:
    try:
        while True:
            if ser.in_waiting > 0:
                data = ser.read(ser.in_waiting).decode('ascii')
                outfile.write(data)
                outfile.flush()  # 버퍼 비우기
                print(data, end="")  # 터미널에 출력
    except KeyboardInterrupt:
        print("Program interrupted")
    finally:
        ser.close()
        print("Serial port closed")
