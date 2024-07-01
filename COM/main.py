import tkinter
import serial
import cv2
import numpy as np
import matplotlib.pyplot as plt
import pytesseract
import os
import threading
import tkinter.ttk as ttk
import tkinter.messagebox as msgbox
from tkinter import * # __all__
from tkinter import filedialog

from license_plate import *
from DATABASE import *
from PACKET import *

import struct

# 수신 시작 나타냄
# 최초 수신시 0xe0 쓰레기값 버리기 위함
start = 1
# 차량 번호판 인식 여부 나타냄
detect = 0

# 기본 창 설정
root = Tk()
root.title("PARKING")
root.geometry("640x480") # 가로 * 세로

# 직렬포트 초기화
ser = serial.Serial(xonxoff=True)

# [연결] 버튼 동작
def Setting_serial_btncmd():
    if PORT_combobox.get() == '포트':
        msgbox.showinfo(title="ERROR", message="연결 포트를 선택해주세요")
        return
    else:
        PORT = PORT_combobox.get()

    try:
        BAUDRATE = int(BAUDRATE_txt.get("1.0", END))
    except:
        msgbox.showinfo(title="ERROR", message="Baudrate를 정확히 입력해주세요")
        return

    # ser PORT, BAUDRATE 값 넣어줄 부분#######################################################
    print(PORT)
    print(type(BAUDRATE))
    ser.port = PORT
    ser.baudrate = BAUDRATE
    ser.open()

    if ser.isOpen():
        msgbox.showinfo(title="Success", message="{0}에 연결 성공".format(PORT))
    else:
        msgbox.showinfo(title="ERROR", message="연결 실패")


CONNECT_label = Label(root, text="연결 설정")
CONNECT_label.pack()
CONNECT_label.place(x=10, y=200)

# 포트 선택 combobox
values = ['COM' + str(i) for i in range(1, 16)] # COM1 ~ COM15까지 연결 포트 선택
PORT_combobox = ttk.Combobox(root, width=7, height=7, values=values)
PORT_combobox.pack()
PORT_combobox.set('포트')
PORT_combobox.place(x=90, y=200)

# 통신속도 입력
BAUDRATE_txt = Text(root, width=23, height=1)
BAUDRATE_txt.pack()
BAUDRATE_txt.insert(END, "통신속도 입력")
BAUDRATE_txt.place(x=180, y=201)

# 연결 버튼
SERIAL_btn = Button(root, text="연결", command=Setting_serial_btncmd)
SERIAL_btn.pack()
SERIAL_btn.place(x=380, y=198)


# 5초마다 데이터 읽기
def UART_DATA_READ():
    global start, detect
    timer = threading.Timer(3, UART_DATA_READ)
    timer.start()

    while 1:
        if timer.is_alive() == 0:
            print('timer restart')
            timer = threading.Timer(5, UART_DATA_READ)
            timer.start()
        else:
            print('timer alive')

        DATA_BUFF = []
        PACKET = []

        re_send_flg = 0
        send_request = 1
        count = 0
        print('reset data buff\n')
        if ser.isOpen():
            while 1:
                if start:  # 수신 시작 시 들어오는 쓰레기 값 버리기
                    data = ser.read()
                    data = ser.read()
                    start = 0
                    return

                timer.cancel()
                print('reading')

                data = ser.read(11)
                print(data, 'input data', type(data))
                # print(data, ' ', type(data))
                for i in range(len(data)):
                    DATA_BUFF.append(struct.pack('B', data[i]))

                print(DATA_BUFF, 'data buff\n')
                try:
                    if len(DATA_BUFF) == 11:
                        print(DATA_BUFF)
                        ser.flushInput()
                        # timer.start()
                        break
                except:
                    pass
        else:
            return
        if DATA_BUFF.count(b'\x00') == 11:
            print('zero data')
            #return
            break

        if DATA_BUFF[0] != b'\xff':
            print('error DATA_BUFF[0]')
            PACKET = resPACKET('', 're send')
            for data in PACKET:
                ser.write(data)
            #return # DATA_BUFF[0] != 0xff이면 오류
            break

        if DATA_BUFF[1] != b'\x01':
            print('error DATA_BUFF[1]')
            PACKET = resPACKET('', 're send')
            for data in PACKET:
                ser.write(data)
            #return # DATA_BUFF[1] != 0x01이면 오류
            break

        if DATA_BUFF[2] == b'\x01':  # type : 차량 번호판 인식 요청
            print('type 0x01')
            webcam = cv2.VideoCapture(0)

            # 총 3번의 인식
            for i in range(3):
                _, frame = webcam.read()
                #frame = cv2.imread('car2.png')
                license_plate_chars = read_license_plate(frame)
                print(license_plate_chars)
                if len(license_plate_chars) != 0:
                    print('success')
                    detect = 1
                    send_request = DATA_INPUT(license_plate_chars, 'plate num')  # 번호판 인식 결과 저장
                    # 번호판 인식 성공 메세지 ATMEGA128에 전송
                    if send_request:
                        PACKET = resPACKET(license_plate_chars, 'detect')
                        for data in PACKET:
                            ser.write(data)
                        #return
                        break
                    else:
                        print('VISIT')
                        # 방문 차량 방문 동호수 송신 요청
                        PACKET = resPACKET('', 'apt num')
                        for data in PACKET:
                            ser.write(data)
                        #return
                        # 번호판 인식률이 떨어지므로 인식이 되었더라도 등록차량이 아니면 차량 사진 저장
                        cv2.imwrite('license_plate_capture/visit/{0}.jpg'
                                    .format(datetime.today().strftime("%Y%m%d%H%M%S")
                                            + '_' + license_plate_chars), frame)
                        break
            # 번호판 인식 실패 메세지 ATMEGA128에 전송
            if detect == 0:
                print("can't detection")
                ser.flushOutput()
                PACKET = resPACKET('', "can't detection")
                for data in PACKET:
                    ser.write(data)
        elif DATA_BUFF[2] == b'\x02':  # type : 아파트 동호수 정보 수신
            print('type 0x02')
            if detect == 0:
                webcam = cv2.VideoCapture(0)
                _, frame = webcam.read()
                # 번호판 인식되지 않았지만 출입하는 차량 사진 저장
                cv2.imwrite("license_plate_capture/can't_detection/{0}.jpg"
                            .format(datetime.today().strftime("%Y%m%d%H%M%S")), frame)
                DATA_INPUT('', "can't detection")
            else:
                detect = 0
            # 데이터 읽기
            PACKET_apt_num = []
            for i in range(6):
                PACKET_apt_num.append(DATA_BUFF[5 + i][0])
            # 엑셀에 동호수 정보 입력
            apt_num = ''
            for i in range(6):
                if i == 3:
                    apt_num += '/'
                apt_num += str(PACKET_apt_num[i])
            DATA_INPUT(apt_num, 'apt num')
            #return
            #break

UART_DATA_READ()

# 저장경로 설정#####################################################################

DATA_label = Label(root, text="날짜 선택")
DATA_label.pack()
DATA_label.place(x=10, y=240)

# 차량 출입 데이터 가져오기
year = [i for i in range(2000, int(datetime.today().strftime("%Y"))+1)] # 년도 선택
YEAR_DATA_combobox = ttk.Combobox(root, width=7, height=7, values=year)
YEAR_DATA_combobox.pack()
YEAR_DATA_combobox.set('년')
YEAR_DATA_combobox.place(x=90, y=240)

month = [i for i in range(1, 13)] # 달 선택
MONTH_DATA_combobox = ttk.Combobox(root, width=7, height=7, values=month)
MONTH_DATA_combobox.pack()
MONTH_DATA_combobox.set('월')
MONTH_DATA_combobox.place(x=180, y=240)

day = [i for i in range(1, 32)] # 일 선택
DAY_DATA_combobox = ttk.Combobox(root, width=7, height=7, values=day)
DAY_DATA_combobox.pack()
DAY_DATA_combobox.set('일')
DAY_DATA_combobox.place(x=270, y=240)

# 파일 가져오기
def GET_DATA_btncmd():
    YEAR = str(YEAR_DATA_combobox.get())
    MONTH = str(MONTH_DATA_combobox.get())
    if len(MONTH) == 1:
        MONTH = '0' + MONTH
    DAY = str(DAY_DATA_combobox.get())
    if len(DAY) == 1:
        DAY = '0' + DAY
    DATE = YEAR+MONTH+DAY
    #DATE='20221125'

    file_path = "./DATA/{0}.xlsx".format(DATE)
    if not os.path.isfile(file_path):
        msgbox.showinfo(title="ERROR", message="{0}.xlsx is not in directory!".format(DATE))
        return

    DATA = DATA_OUTPUT(DATE)

    DATA_table.delete(*DATA_table.get_children())
    for i in range(len(DATA)):
            DATA_table.insert('', index = -1, text='', values=DATA[i])

    TABLE_style = ttk.Style()
    TABLE_style.configure(DATA_table, rowheight=500)
    TABLE_style.configure(DATA_table, height=300)

# 불러온 데이터 표시를 위한 테이블 생성
DATA_table = ttk.Treeview(root, columns=['#1', '#2', '#3', '#4'], height=8)
DATA_table['show'] = 'headings'
DATA_table.pack()

DATA_table.column('#0', width=0, stretch='no')
DATA_table.heading('#0', text='', anchor='center')
DATA_table.column('#1', width=100)
DATA_table.heading('#1', text='TIME', anchor='center')
DATA_table.column('#2', width=200)
DATA_table.heading('#2', text='LICENSE PLATE NUMBER', anchor='center')
DATA_table.column('#3', width=100)
DATA_table.heading('#3', text='TYPE', anchor='center')
DATA_table.column('#4', width=100)
DATA_table.heading('#4', text='APT NUM', anchor='center')

# 데이터 불러오기
DATA_btn = Button(root, text="불러오기", command=GET_DATA_btncmd)
DATA_btn.pack()
DATA_btn.place(x=380, y=238)

root.mainloop()

