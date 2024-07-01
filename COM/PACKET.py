import struct

# 패킷 구조
# 장치 타입 / 요청 사항 / 작동 주변 기기 / 데이터 길이 / 데이터 / 패킷 종료
# 장치 타입은 0x10으로 고정
# 요청 사항은 패킷을 보내는 목적
# 작동 주변 기기 : 모터, 스위치
# 데이터는 LCD에 출력할 문자를 LCD EEPROM에 저장하기 위함
# 패킷 종료는 0x00으로, 수신단에서 0x00을 받으면 패킷 읽기 종료
# 데이터에 0x00 존재할 수 있으므로 이를 위해 데이터 길이 전송

def resPACKET(plate_char, info):
    DATA_BUFF = []

    # 패킷 시작
    DATA_BUFF.append(b'\xff')

    # 장치 타입
    DATA_BUFF.append(b'\x10')

    # 전달 명령
    if info == 'detect':
        print('detect')
        DATA_BUFF.append(b'\x01') # request motor active
        DATA_BUFF.append(b'\x01') # motor active
        DATA_BUFF.append(b'\x00') # data length (no data)
        DATA_BUFF.append(b'\x00') # end of packet
        return DATA_BUFF
    elif info == "can't detection":
        print("can't detection")
        DATA_BUFF.append(b'\x02') # request display 'no detection' on LCD
        DATA_BUFF.append(b'\x02') # switch active
        DATA_BUFF.append(b'\x00') # data length (no data)
        DATA_BUFF.append(b'\x00')  # end of packet
        return DATA_BUFF
    elif info == 'apt num':
        print('apt num')
        DATA_BUFF.append(b'\x03') # request only keypad input
        DATA_BUFF.append(b'\x02') # switch active
        DATA_BUFF.append(b'\x00') # data length (no data)
        DATA_BUFF.append(b'\x00')  # end of packet
        return DATA_BUFF
    elif info == 're send':
        DATA_BUFF.append(b'\x04')  # re send request
        DATA_BUFF.append(b'\x04')  # no activation
        DATA_BUFF.append(b'\x00')  # data length (no data)
        DATA_BUFF.append(b'\x00')  # end of packet
        return DATA_BUFF
        
    return DATA_BUFF