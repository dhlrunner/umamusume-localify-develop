# My umamusume tools
# ⚠ Spaghetti code warning ⚠
## 나만 쓰려고 만든 툴

Copy of https://github.com/GEEKiDoS/umamusume-localify

### 주의
개발자 본인만 사용할 용도로 제작된 툴입니다.

코드의 가독성이 매우 좋지 않습니다.

버그 등에 관한 피드백은 일체 받지 않습니다.

폐쇠된 로컬 환경에서 본 툴을 테스트 및 실행하고 있으며,

실제 공식 서버에 연결된 클라이언트에서 사용 시 일어나는 모든 문제에 대하여 개발자는 책임을 일절 지지 않을 것임을 밝힙니다.


### 대충 기능 나열
- 설정 파일 자동저장 (json 수정 필요 없음)
- 게임 리소스 경로 변경
- 인게임 설정창 (Dear imGui)
- 라이브 미세 조정
- 라이브 카메라 워크 정지 (만)
- 높은 3D 모델 해상도
- 콘솔 명령어
- 이상한 모드들
- 서버 호스트 변경
- dll 덤프 (보호 우회)
- 등등

### 콘솔 명령어들
| **명령**  | **인수** | **설명** | **비고** |
|-------|---------------|------|------|
|exit|-|프로그램 종료||
|crash|-|SIGSEGV||
|fps|정수 fps값|프레임 제한 설정||
|autofps|-|자동 fps 제한 설정||
|useExclusiveFullscreen|-|전체화면 시 단독 모드 사용||
|resetFullscreen|-|단독 모드 전체화면 해제||
|cutinCharSet|[0]캐릭터 타입 정수 [1]캐릭터 ID 정수 [2]의상 ID 정수 [3]머리ID 정수|컷인 캐릭터 강제 지정 (스킬 컷씬용)| 캐릭터 관련 값이 -1일 경우 기본값 사용|
|cutinRaceCharMotionSet|[0]캐릭터 ID 정수 [1]의상 ID 정수 [2]레이스 등급 정수(G1:100,G2:200, G3:300) [3]레이스 등수 정수(0부터 시작)| 레이스 결과 모션 강제 설정 |값이 -1일 경우 기본값 사용|
|setCardId|[0]캐릭터 cardid 정수|레이스 결과 모션 강제 설정 (G1 레이스 전용)|값이 -1일 경우 기본값 사용|
|resetCutin|-|컷인 캐릭터 설정값 초기화||
|story3dCharSetEnable|-|스토리용 3D 모델 설정 기능 활성화||
|story3dCharSet|[0]캐릭터 ID 정수 [1]의상 ID 정수 [2]모브 ID 정수 [3]머리ID 정수|스토리용 3D 모델 강제설정 기능| story3dCharSetEnable이 켜진 상태에서 값이 -1일 경우 모델 생성시마다 바꿀 캐릭터를 묻습니다.|
|reset|-|게임 재시작||



### 적용되는 게임
DMM판 우마무스메

### 빌드 밎 적용
Visual Studio 2022가 필요합니다.

.sln 파일을 열고, `umamusume-localify` 프로젝트를 빌드하신 뒤 `build\bin\x64` 폴더에 생성된 `version.dll` 파일을 게임 실행 위치에 복사하시고 실행해 주세요.

개발자 컴퓨터 환경이 아닌 다른 환경에서의 정상 실행을 위해 이 부분 밎 관련 코드를 주석처리 해야 할 수 있습니다. (외부 플러그인 로드하는 부분)

https://github.com/dhlrunner/umamusume-localify-develop/blob/606eed79af61f9678d294bdaad0863f69a1f93bf/src/main.cpp#L1886


### 스크린샷들
![image](https://github.com/dhlrunner/umamusume-localify-develop/assets/37791084/767117af-4bfc-48f7-939f-58129aa47792)
![image](https://github.com/dhlrunner/umamusume-localify-develop/assets/37791084/19bef1e1-f436-4684-af9e-bfc6de973da3)
