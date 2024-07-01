# Vehicla Access Management System

### 차량 출입 관리 시스템
| 번호판 인식을 통한 차량 판별 (등록/미등록) <br>
| 엑셀을 이용한 출입 기록 <br>

---

### 주요 기능

- PC
  - 웹캠을 이용한 차량 번호판 인식
  - 출입 차량 기록 (엑셀)
  - UART 통신을 이용해 ATMEGA128과 연결
  - 포트, 보드레이트 설정 (GUI)
  - 날짜별 엑셀 불러오기 (GUI)
- ATMEGA128
  - 차량 접근 감지
  - 차단기 제어
  - 방문 동 호수 입력

---

### 동작 시나리오
<center>

![run](https://github.com/GreenClothes/Vehicle_Access_Management_System/assets/106455871/90ccf0d5-857d-4799-8389-4dbd6437e115)
</center>

### 예외 시나리오
<center>

![exception](https://github.com/GreenClothes/Vehicle_Access_Management_System/assets/106455871/61cae690-9596-48e8-bb2b-ead4dce08331)
</center>

---

### 동작 검증

- 차량 출입 기록 불러오기
  - 날짜 선택 후 불러오기 클릭

<center>

![GUI](https://github.com/GreenClothes/Vehicle_Access_Management_System/assets/106455871/c01f7382-0362-4808-a912-285c95d9b0a8)
</center>

- 등록 차량 출입
  - 차량 접근 시 LCD에 'Welcome' 출력
  - 번호판 인식 후 LCD에 '등록' 출력 및 차단기 개방/폐쇄
  - 엑셀 파일에 기록

<center>

![등록차량출입](https://github.com/GreenClothes/Vehicle_Access_Management_System/assets/106455871/33a0c5f6-6525-452b-975c-5b12a3748e87)
</center>

- 방문 차량 출입
  - 차량 접근 시 LCD에 'Welcome' 출력
  - LCD에 'input APT NUM' 출력 및 키패드로 동호수 입력
  - 동호수 입력 완료 후 '#' 입력 시 차단기 개방/폐쇄
  - 엑셀 파일에 기록 및 차량 사진 저장
  
<center>

![방문차량출입](https://github.com/GreenClothes/Vehicle_Access_Management_System/assets/106455871/a305867e-5c7f-4b38-8a8a-860a2d6f03bc)
</center>

- 어두울 때 차량 출입
  - 차량 접근 시 LCD에 'Welcome' 출력 및 고휘도 LED 점등
  - 차단기 폐쇄 시 고휘도 LED 점멸

<center>

![어두울때](https://github.com/GreenClothes/Vehicle_Access_Management_System/assets/106455871/092c329b-1c53-4df6-bd95-1d49ad33dc27)
</center>

- 차단기 수동 조작
  - 리모컨 2번 버튼으로 차단기 개방 조작
  - 수동 개방 시 차량 접근 감지 중단
  - 리모컨 2번 버튼으로 차단기 폐쇄 조작
  - 폐쇄 시 차량 접근 감지 재개

<center>

![차단기점검](https://github.com/GreenClothes/Vehicle_Access_Management_System/assets/106455871/3b68a480-8634-44b8-8039-23be8b087caa)
</center>

- 번호판 인식이 되지 않는 경우 (출입 시)
  - 차량 접근 시 LCD에 'Welcome' 출력
  - LCD에 'no detection' 출력 및 파란색 LED 점등, 스위치 활성화
  - 스위치 누를 시, LCD에 'input APT NUM' 출력 및 키패드로 동호수 입력
  - 동호수 입력 완료 후 '#' 입력 시 차단기 개방/폐쇄
  - 엑셀 파일에 기록 및 차량 사진 저장

<center>

![번호판인식X](https://github.com/GreenClothes/Vehicle_Access_Management_System/assets/106455871/8e4f4bc9-2608-44a8-abab-f4437c1c59e8)
</center>

- 번호판 인식이 되지 않는 경우 (미출입 시)
  - 차량 접근 시 LCD에 'Welcome' 출력
  - LCD에 'no detection' 출력 및 파란색 LED 점등, 스위치 활성화
  - 10초 대기 간 스위치가 눌리지 않으면 초기 상태로 복귀

<center>

![인식만출입X](https://github.com/GreenClothes/Vehicle_Access_Management_System/assets/106455871/d3218e21-bc0d-4084-8384-1c4392628ba3)
</center>

---

### 데이터 패킷 구조

<center>

| __start of packet__ | __device type__ | __request__ | __control device__ | __data length__ | __data__ | __end of packet__ |
</center>

- start of packet
  - 패킷 시작을 알림
  - 0xff 로 고정 값
- device type
  - 장치 타입
  - 0x10 으로 고정 값
- request
  - 패킷을 보내는 목적
  - 0x01
    - 모터 동작 요청
  - 0x02
    - LCD 동작 요청 (번호판 인식 불가)
  - 0x03
    - 키패드 입력 동작 요청
  - 0x04
    - 패킷 재전송 요청
- control device
  - 동작 요청할 주변 기기
  - 0x01
    - 모터 동작
  - 0x02
    - 스위치 동작 (입력)
  - 0x04
    - 동작 요청하지 않음
- data length
  - 데이터 길이
- data
  - 데이터 (입력받은 동호수 정보)
- end of packet
  - 패킷의 끝을 알림
  - 0x00 으로 고정 값

---