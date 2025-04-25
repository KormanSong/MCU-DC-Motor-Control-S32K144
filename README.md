# S32K144 Based Automotive Control System

## Implementation of an MCU-based system for DC motor control using NXP S32K144

## 1. 프로젝트 개요 (Overview)

* **과목:** 경북대학교 전자공학부 '마이크로프로세서설계실험' 팀 프로젝트 (2인 팀)
* **플랫폼:** NXP S32K144 MCU
* **목표:** MCU의 다양한 페리페럴(GPIO, ADC, PWM, Timer Interrupt 등)을 활용하여 실시간으로 DC 모터의 속도 및 방향을 제어하고, 7세그먼트로 속도를 표시하는 자동차 제어 시스템 구현.
* **나의 기여:** 정밀한 속도 제어를 가능하게 하는 PWM 신호 채널 설계, 현제 속도 및 상태를 표시하기 위한 7세그먼트 LED 출력 오류 수정, 코드 재사용성 및 이식성 향상을 위한 `port.h` 헤더 파일 작성, 버튼 입력 관련 인터럽트 문제 해결 등 주요 기능 구현에 기여.

## 2. 주요 기능 (Key Features)

* 가변 저항(ADC 입력) 값에 따른 DC 모터 속도 제어 (PWM Duty Cycle 조절)
* 릴레이 스위치(GPIO 출력)를 이용한 DC 모터 정/역방향 회전 제어
* 7세그먼트 디스플레이 및 타이머 인터럽트를 활용한 실시간 속도 표시 (1~10 단계)
* 브레이크 버튼(GPIO Level Interrupt) 구현 (버튼 누르는 동안 속도 0)
* 각 버튼(브레이크, 정방향, 역방향) 동작 상태 LED 표시 기능

## 3. 하드웨어 구성 (Hardware Setup)

* **MCU:** NXP S32K144
* **모터:** DC Motor & Motor Driver
* **제어:** Relay Switch (방향 전환용), 가변 저항 (속도 조절용), 푸쉬 버튼 (제어 입력용)
* **출력:** 7-Segment Display (속도 표시용), LEDs (상태 표시용)
* (세부 회로도는 `doc/` 폴더에 추가 예정)

## 4. 핵심 사용 기술 (Tech Stack & Core Concepts)

* **Language:** C
* **MCU Peripherals:** S32K144 (GPIO Input/Output & Interrupt, FTM for PWM, ADC, LPIT for Timer Interrupt)
* **Concepts:** Real-time Control, Interrupt Handling (NVIC), Clock Configuration, Hardware Abstraction (`port.h`)

## 5. 작동 시연 (Demo Video)

[![S32K144 자동차 제어 시스템 데모 영상](https://img.youtube.com/vi/b7nBCa_eMpU/0.jpg)](https://youtu.be/m-b0-jh9N6o)


## 6. (향후 README 업데이트 예정)

* 소스 코드 상세 설명 (Source Code Explanation)
* 빌드 및 실행 방법 (Build & Usage)
* 프로젝트 결과 및 고찰 (Results & Lessons Learned)
