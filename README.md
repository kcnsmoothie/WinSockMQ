# WinSockMQ

C++ 소켓 프로그래밍으로 구현해보는 메시지 큐

---

## 개요

WinSockMQ는 Windows 소켓(WinSock) 기반으로 메시지 큐(MQ)의 핵심 개념을 직접 구현해보는 프로젝트입니다.
프로듀서(Producer), 컨슈머(Consumer), 브로커(Broker) 구조를 직접 설계하고, 네트워크 소켓을 통해 메시지를 송수신하면서 MQ의 동작 방식을 이해하고자 합니다.

---

## 구조

* **Producer**: 메시지를 생성하고 익스체인지에 전송
* **Exchange**: 수신한 메시지를 큐에 저장하고, 소비자에 분배
* **Consumer**: 익스체인지로부터 메시지를 받아 처리
* 각 모듈은 독립된 C++ 실행 파일로 구성되어 있으며, 프로젝트 솔루션 파일(WinSockMQ‑exchange.sln)을 포함하고 있습니다.

---

## 주요 기능

**기능 구현 사항 명세**

**Producer(Server)**

- Exchange로 메시지 전송 기능
- 메시지 전송 시 함께 전달해야하는 정보
    - Exchange 이름
    - Routing key : Exchange에서 Queue로 바인딩하기 위해 필요

**Exchange**

- Producer에서 받은 메시지와 Routing Key를 함께 저장 기능
- Routing Key를 읽고 해당되는 Queue로 전달 기능

**Queue**

- Exchange에서 메시지 수신 기능
- 자신을 구독중인 Queue에 메시지를 전송하는 기능

**Consumer(Client)**

- 특정 Queue를 구독하는 기능
- 구독하는 Queue에서 메시지 수신

---

## 추가 기능 구현 사항

### 멀티 스레드 환경으로의 전환
- producer인지 consumer인지 구분
    -  P/C 역할 플래그를 전송
- 여러 대의 producer가 동시에 메시지를 발송
    - producer가 메시지를 보내고 수신했는지 확인할 수 있는 기능
    - producer가 연결 종료 요청 전까지 연결 유지
- 여러 대의 consumer가 연속적으로 queue에 저장된 메시지 수신
    - consumer가 queue에서 메시지를 pop
    - queue에서 메시지 정상 삭제

---

## 설치 및 실행 방법

1. Windows 환경에서 Visual Studio 2022를 준비합니다.
2. 저장소를 클론 받습니다:

   ```bash
   git clone https://github.com/kcnsmoothie/WinSockMQ.git
   ```
3. `WinSockMQ‑exchange.sln`을 Visual Studio에서 엽니다.
4. Producer, Exchange, Consumer 프로젝트 각각을 빌드합니다.
5. Exchange를 먼저 실행한 후 Producer, Consumer를 실행해주세요
6. 콘솔 로그를 확인하며 MQ의 작동을 확인합니다.

---

## 학습 포인트

* Windows 소켓 API(WinSock)에 익숙해질 수 있습니다.
* 메시지 큐의 핵심 개념 — 생산, 라우팅, 소비 — 를 소켓 코드를 따라가며 이해할 수 있습니다.

---

## 기여 및 피드백

이 프로젝트는 학습용이지만, 개선이나 제안은 언제든지 환영합니다.
버그 리포트, 기능 제안, 코드 리팩터링 등 다양한 방식으로 기여해주세요.


