191028 Ch7. Event Driven Programming
-	Space Programming: the curses library
-	The curses library: terminal을 제어, cursor의 위치를 조절, text의 형태 제어
-	basic curses functions
-	initscr() -초기화
-	endwin() – turn off curses and reset tty terminal setting 초기화
-	refresh() – 화면에 출력
-	move(r, c) – cursor 이동
-	addstr(s), addch(c) – string 또는 char 그리기
-	clear()
-	standout(), standend(): standout mode를 키고 끔 (text 강조)
-	Curses는 두 버전의 화면을 저장(virtual, updated) – 변화된 부분만 전송
-	Time Programming: sleep
-	움직이는 물체: 특정한 시간 간격마다 출력 – timer, interval 설정
-	Programming with Time: Alarms, Interval Timers
-	sleep(n) - process를 특정 시간 n초만큼 block 후 alarm signal 발생
-	signal(SIGALRM, handler);
alarm(n);  - n초 뒤에 alarm signal 발생
pause();	  - signal 발생 전까지 process block
-	단점: 시간 설정의 제약 (1초 단위)
-	Interval Timer 3가지
-	Real Timer: 실제 실행 시간 = Elapsed time = CPU time + IO time + waiting time
-	Virtual Timer: process에 의한 시간 = CPU time (user mode)
-	Profile Timer: process & system calls(kernel)의 동작시간 = user + kernel mode time
-	Virtual & Profile timer 구분 이유: 최적화 기준을 설정하기 위함
-	Interval Timer 정하기
-	1. initial interval(초기 주기) & 2. repeating interval(반복 주기)
-	itimerval 구조체에 저장 – timeval 구조체 - microsecond까지 저장
-	getitmer(timer, struct itimerval *val) – 현재 설정 값 불러오기
setitmer(timer, struct itimerval *newval, struct itimerval *oldval) – 값 설정
-	timer의 역할: suspend execution, schedule future action

-	Signal Handling I: Using signal
-	Signal: 프로그램 동작 중에 발생하는 interruption
-	종류: keystroke / Illegal process / elapsed timer등
-	반응: Default action, signal / Ignore signal / Invoke function	
-	문제: 처음 signal 발생 후 handler 실행 중 다른 signal 발생하는 경우 그냥 지나 감
-	한계: 어떤 signal이 발생했는지는 알 수 있지만, signal이 발생한 이유는 모름
-	inthandler와 signal 사이에 발생하는 경우 문제 발생
-	SIGQUIT을 무시만 할 수 있고, block(지연)할 수 있는 방법이 없음.

-	Signal Handling II: sigaction
-	sigaction(signum, action, prevaction) – signal 처리방식을 더 세밀하게 정해 줌
-	사용하는 이유
-	signal() -> 특정 signal 동작을 정하는데 한계
-	signal -> handler 발생 -> 원인 알기 위해
-	sys. call 호출, signal 발생 -> return 안하고 handler 들어갔다가 다시 돌아오도록
-	struct sigaction – signal의 처리방법, 동작 등 기술
-	void *sa_handler – signal 발생시 호출할 값 – SIG_DFL, SIG_IGN, function
-	void (*sa_sigaction)(int signum, siginfo_t *, void *) – 새로운 handler
-	두 handler 중 한 function을 통해 signal에 대한 정보(발생 이유)를 알 수 있음. -> signal() sys. call 과의 차이점
-	sigset_t sa_mask – block 할 signal을 저장
-	int sa_flags: signal handling 시의 동작을 세밀하게 기술
-	SA_RESETHAND – mousetrap mode – handler 실행 이후 signal 재발생 시 default action 수행, 동일한 handler 실행 위해 초기화
-	SA_NODEFER – block 안하고 signal 다시 실행하도록 해줌
-	SA_RESTART – return 안하고 restart
-	SA_SIGINFO – hander 2개 중에 선택

-	Protecting Data from Corruption
-	Critical Sections: interrupt가 생기는 경우 망가질 수 있는 중요한 부분
-	보호하는 방법: signal을 block 또는 ignore 한다.
-	sigprocmask(how, sigset_t *sigs, sigset_t *prev) – 특정 signal을 block
-	how: SIG_BLOCK, SIG_UNBLOCK, SIG_SET
-	*sigs: list of signals to use
-	*prev: previous signal mask
-	building signal sets with sigsetops
-	sigemptyset(*setp) – signal 모음(setp)을 비워줌
-	sigfillset(*setp) – 모든 signal을 setp list에 넣음
-	sigaddset(*setp, signum) – setp 모음에서 signum을 넣음
-	sigdelset(*setp, signum) – setp 모음에서 signum을 뺌
-	Reentrant Code: 이미 실행 중에 다시 실행되어도 문제 발생 X 함수
-	recursive – set SA_NODEFER flag
-	blocking – clear SA_NODEFER flag


-	Kill: Sending Signals from a Process
-	한 process가 다른 process에게 signal을 보낼 때 사용
-	ex) inter-process communication
-	kill(pid, sig) – process에 signal을 보냄
-	pit_t pid: process id of target
-	int sig: signal
-	특징)
-	조건: 전송하는 대상과 같은 user ID를 갖거나, 관리자(superuser)여야 함
-	자기 자신에게도 보낼 수 있음
-	모든 종류의 signal을 보낼 수 있음. – keyboard, timer, kernel
-	custom 가능한 default signal: SIGUSR1, SIGUSR2

-	Signals on Input: Asynchronous I/O
-	이전까지: timer 통해 user input 기다림 -> event driven X
user input 들어올 때마다 action -> event-driven O
목표: SIGIO signal 발생하면 동작하도록 만들어주기
-	pause() 기능: signal 발생 전까지 process를 ‘suspend’상태로 만들어 CPU가 다른 process를 실행할 수 있도록 해줌.
-	방법1: Using O_ASYNC
1.	‘handler’ 만들기
2.	fcntl (file attribute 바꿔줌) 이용하여 F_SETOWN (SIGIO) 설정 – input notification signal 전송
3.	input signal 켜기 – O_ASYNC (기다리지 않고 바로 return)를 file descriptor 0(stdin)에 설정
4.	signal 기다리기 위해 pause 반복
-	방법2: Using aio_read
1.	‘handler’ 만들기 (on_input) – input이 들어왔을 때 실행
2.	aiocb 구조체 – 기다릴 input & 전송할 signal 저장
3.	read 명령으로 aio_read에게 구조체 전달
4.	write handler – aio_return
-	Asynchronous I/O가 필요한 이유?
-	의미 없는 조건 검사(pause 실행을 위한)를 반복할 필요 X
