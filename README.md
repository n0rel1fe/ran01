C:/User 경로 기준으로 탐색
'Documents', 'Desktop', 'Downloads' 대상 디렉토리

/User/<사용자이름>
  - 사용자이름이 한국어일 경우, 경로로 호출하지 못하는 문제
  - C:/User 아래의 전체 파일을 탐색하게 되면 탐색 속도가 현저히 느려짐
  - 대안으로 블랙리스트 처리 방법을 적용함.
  - const wchar_t* blacklist[] = { L"AppData", L"All Users", L"Default", L"Public" };
  - 블랙리스트는 탐색하지 않음.

대소문자 구분하여 정확히 대상 디렉토리만 탐색하도록 함.

접근할 수 없는 디렉토리는 건너뜀.

디렉토리 내의 파일을 리스트화함. 

windows10(vm)에서 exe 파일로 테스트 완료.

![searchdir_re](https://github.com/00forensic/ran01/assets/105771793/d01668b8-a493-4a3c-84c1-e90a44ab2faf)
![dir_file_re](https://github.com/00forensic/ran01/assets/105771793/7ac20159-e0e9-414a-9cd3-f9968a8872cc)
