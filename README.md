# VEDA 3기 linux mini project 과제

## 1. 서버

### 1-1. 구성
```
.
├── CMakeLists.txt
├── include
│   ├── buzzController.h
│   ├── buzz.h
│   ├── fnd.h
│   ├── httpobject.h
│   ├── httpserver.h
│   ├── ledController.h
│   ├── led.h
│   ├── linkedList.h
│   ├── peripheral.h
│   └── yl40.h
├── libs
│   ├── data_structure
│   │   ├── CMakeLists.txt
│   │   └── linkedList.c
│   ├── http
│   │   ├── CMakeLists.txt
│   │   └── httpserver.c
│   ├── indicators
│   │   ├── buzz.c
│   │   ├── CMakeLists.txt
│   │   ├── fnd.c
│   │   └── led.c
│   └── sensors
│       ├── CMakeLists.txt
│       └── yl40.c
├── src
│   ├── buzzController.c
│   ├── ledController.c
│   └── main.c
└── third_party
    └── cjson
        ├── cJSON.c
        ├── cJSON.h
        └── CMakeLists.txt
```

### 1-2. 빌드 및 실행 방법

이 파일이 있는 디렉토리를 기준으로 아래와 같은 명령을 통해 서버를 빌드할수 있습니다.
``` shell
cd server

mkdir build

cd build

cmake ..
cmake --build .
```

실행 방법은 다음과 같습니다.
``` shell
./mini_server mini_server
```

데몬으로 실행되기 때문에 프로세스를 종료하기 위해서 아래와 같은 방법을 사용하고 있습니다.
``` shell
pgrep -f mini_server    # 프로세스 id 획득
kill -9 [프로세스id]
```

### 1-3. 구현 기능
- LED ON/OFF            - 구현
- LED PWM 제어          - 구현
- PWM 조도 센서 연동    - 구현
- FND 알람              - 구현
- FND 알람 취소         - 구현
- 부저 제어             - 구현
- 데몬 프로세스         - 구현
- 동적 라이브러리       - 미구현

## 2. 클라이언트

### 2-1. 구성
```
.
├── CMakeLists.txt
├── include
│   └── client.h
├── src
│   ├── httpClient.c
│   ├── main.c
│   └── parsing.c
└── third_party
    └── cjson
        ├── cJSON.c
        ├── cJSON.h
        └── CMakeLists.txt
```

### 2-2. 빌드 및 실행 방법

이 파일이 있는 디렉토리를 기준으로 아래와 같은 명령을 통해 서버를 빌드할수 있습니다.
``` shell
cd client

mkdir build

cd build

cmake ..
cmake --build .
```

실행 방법은 다음과 같습니다.
``` shell
./mini_client <ip> <port>
```

### 2-3. 구현 기능
- LED ON/OFF            - 구현
- LED PWM 제어          - 구현
- PWM 조도 센서 연동    - 구현
- FND 알람              - 구현
- FND 알람 취소         - 구현
- 부저 제어             - 구현
- INT 신호에만 종료     - 구현