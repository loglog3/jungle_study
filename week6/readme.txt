<malloc lab과 직접 관련되는 section들>
19장: malloc-basic 
 - GNU malloc API와 다양한 구현 방식 설명
 - implicit list 방식의 malloc 구현에 관한 자세한 설명
20장: malloc-advanced
 - explicit & seg list방식의 malloc 구현, garbage collection 개념에 대한 설명
 - well-known memory bug에 관한 설명
 - 44p : gdb, valgrind등의 memory bug를 잡기위한 접근 방식 설명 포함

<malloc lab 수행에 도움을 줄 수 있는 내용을 담은 section들>
14장: ecf-procs
 - exception의 개념과 종류, process의 개념에 대한 설명 
 - 14p : segmentation fault에 관한 설명 포함
11장: memory-hierarchy
 - 컴퓨터 시스템의 메모리 계층구조에 대한 설명
17장: vm-concepts
 - 가상 메모리와 page fault에 대한 이론적 설명

-------------
- 1) EC2를 다음과 같이 세팅합니다. (한 줄씩 입력)

    ```python
    sudo apt update
    sudo apt install build-essential
    sudo apt install gdb
    sudo apt-get install gcc-multilib g++-multilib

    git clone https://github.com/bumkyulee/malloclab-handout-jungle.git

    make clean
    make
    ```

- 2) 간단한 파일 설명

    ```python
    mm.c : 수정해야 할 유일한 파일

    /traces : 테스트 꾸러미 폴더
    mdriver.c : 테스트 꾸러미 및 기타 라이브러리를 실행해서 코드를 채점하는 프로그램
    그 외 : mm.c 및 mdriver.c에 필요한 파일들. 만지지 말 것
    ```

- 3) 실행하기

    ```python
    ./mdriver : 채점하기

    ./mdriver -f traces/binary2-bal.rep : 특정 테스트 세트로 채점
    ```

- 4) 시작하기
    1. 세팅 후 `./mdriver` 를 실행하면 `out of memory` 에러 발생
    2. 책에 있는 implicit list 방식대로 malloc을 구현해서 해당 에러를 없애기
    3. 이후 (시간이 된다면) explicit list와 seglist 등을 활용해 점수를 높이기

**GOAL**

- implicit 방법을 이용해 `./mdriver` 가 정상 작동하도록 코드를 완성하기