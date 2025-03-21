#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void test_getpname(void) {
  printf(">>>Testing getpname:\n");
  getpname(1);  // init 프로세스(pid 1)의 이름 출력
}

void test_nice(void) {
  printf("\n=== Nice Value Tests ===\n");
  
  // 현재 프로세스의 nice 값 테스트
  int pid = getpid();
  printf("Initial nice value of current process (pid %d): %d\n", pid, getnice(pid));
  
  // setnice 테스트 - 유효한 값
  printf("Setting nice value to 10...\n");
  if(setnice(pid, 10) == 0) {
    printf("New nice value: %d\n", getnice(pid));
  }
  
  // setnice 테스트 - 유효하지 않은 값
  printf("Trying to set invalid nice value (-1)...\n");
  if(setnice(pid, -1) == -1) {
    printf("Successfully rejected invalid nice value\n");
  }
  
  // 존재하지 않는 pid에 대한 테스트
  printf("Trying to get/set nice value for non-existent pid 99999...\n");
  if(getnice(99999) == -1 && setnice(99999, 20) == -1) {
    printf("Successfully handled non-existent pid\n");
  }
}

void test_ps(void) {
  printf("\n=== Process Status Tests ===\n");
  
  // 모든 프로세스 출력
  printf("Printing all processes (ps 0):\n");
  ps(0);
  
  // 현재 프로세스 정보만 출력
  printf("\nPrinting current process info:\n");
  ps(getpid());
  
  // 존재하지 않는 프로세스
  printf("\nPrinting non-existent process (should print nothing):\n");
  ps(99999);
}

void test_meminfo(void) {
  printf("\n=== Memory Info Test ===\n");
  printf(">>>Testing meminfo:\n");
  meminfo();
}

void test_waitpid(void) {
  printf("\n=== Waitpid Tests ===\n");
  
  int pid = fork();
  if(pid == 0) {
    // 자식 프로세스
    sleep(10);  // 부모가 먼저 출력할 수 있도록 잠시 대기
    printf("Child process (pid %d) sleeping for 2 seconds...\n", getpid());
    sleep(200);  // 2초 대기
    exit(0);
  } else {
    // 부모 프로세스
    printf("Parent waiting for child (pid %d)...\n", pid);
    if(waitpid(pid) == 0) {
      printf("Child process terminated successfully\n");
    }
    
    sleep(10);  // 출력이 섞이지 않도록 잠시 대기
    
    // 이미 종료된 프로세스에 대한 waitpid
    printf("Trying to wait for already terminated process...\n");
    if(waitpid(pid) == -1) {
      printf("Successfully handled waiting for non-existent process\n");
    }
    
    sleep(10);  // 출력이 섞이지 않도록 잠시 대기
    
    // 다른 프로세스의 자식에 대한 waitpid
    printf("Trying to wait for non-child process...\n");
    if(waitpid(1) == -1) {  // pid 1은 init 프로세스
      printf("Successfully rejected waiting for non-child process\n");
    }
  }
}

int main(int argc, char *argv[]) {
  printf("Starting system call tests...\n");
  
  test_getpname();  // 맨 처음에 getpname 테스트 실행
  test_nice();
  test_ps();
  test_meminfo();
  test_waitpid();
  
  printf("\nAll tests completed!\n");
  exit(0);
}
