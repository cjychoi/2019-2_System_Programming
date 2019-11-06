/*
	시스템 프로그래밍 HW3 : List Directory Tree
	작성자 : 컴퓨터학부 14학번 남상윤
	설명 : 부족하지만 봐주셔서 감사합니다. :)
	        코드 설명이 더 필요하시면 nsun9505@naver.com으로 연락주세요.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define HEAP_MAX_SIZE 1000			// 정렬을 위해 사용할 heap 크기
#define FILE_NAME_SIZE 256				// 파일 이름 길이 최대

typedef struct node{
	char filename[FILE_NAME_SIZE];
	struct node * next;
}Node;

typedef struct linkedlist{
	Node * head;
	Node * tail;
	int numOfData;
}LList;

// heap에 저장할 자료구조 정의
typedef struct heapdata{
	ino_t inode_num;
	char file_name[FILE_NAME_SIZE];
}HeapData;

// Heap
typedef struct heap{
	HeapData data[HEAP_MAX_SIZE];
	int numOfData;
}Heap;

/* 	Heap 관련 함수
	initHeap() : 힙을 초기화 합니다. 
	insertHeap() : 힙에 데이터를 삽입합니다.
	deleteHeap() : 힙에서 가장 큰/작은 원소를 가져옵니다.(가져오면서 삭제합니다.) 
	isHeapEmpty() : 힙이 비어있는지 확인합니다.
*/
void initHeap(Heap * ph);
void insertHeap(ino_t inode_num, char * filename);
HeapData deleteHeap(void);
int isHeapEmpty(void);

/*
	HW 3 : List Directory Tree를 해결하기 위한 함수들
	get_inode() : filename에 해당하는 inode number를 받아옵니다.
	printPathTo() : 재귀(recursive) 함수이며, 현재 디렉터리에서 루트 디렉터리까지 이동합니다.
	printInodeAndFileName() : 디렉터리 내의 파일과 아이노드 넘버를 출력합니다.
	printPath() : pwd 명령어를 했을 떄의 출력과 같은 결과를 구현한 함수입니다.
*/
ino_t get_inode(char * filename);
Node * inum_to_name(ino_t inode_to_find);
void printPathTo(ino_t this_inode, ino_t);
void printInodeAndFileName(char *, ino_t, int);
void printPath(void);

/*
	Linked list를 구현하기 위한 함수들입니다.
	initLList() : Linked list를 초기화합니다.
	createNode() : Linked list에 삽입되는 노드를 생성합니다.
	insertLList() : Linked list에 node를 삽입합니다.
	deleteLList() : Linked list에서 node 하나를 삭제합니다.	
*/	
void initLList(void);
Node * createNode(char * fname);
void insertLList(Node * newNode);
Node * deleteLList(void);

// 아이노드 넘버를 기준으로 정렬(Heap-sorting)하여 출력하기 위한 Heap
static Heap heap;
// path를 출력하기 위해 루트 디렉토리로 가면서 거쳐간 디렉터리들을 담는 linked list입니다.
LList path;

/*
	main함수
	힙과 링크드 리스트를 초기화합니다.
	printPathTo(ino_t ino_1, ino_t ino_2)
	  - ino_1은 현재 디렉토리의 아이노드 넘버를 매개변수로 받습니다.
	  - ino_2는 아래에서 설명하겠습니다.
*/
int main(void){
	initHeap(&heap);
	initLList();
	printPathTo(get_inode("."), get_inode("."));
	printPath();

	return 0;
}

/* 	현재 디렉터리(절대경로)를 출력합니다.
	path에는 루트디렉토리러 가면서 거쳐간 디렉터리들의 이름이 담깁니다.
	순서대로 출력하여 pwd 명령어의 출력과 동일한 결과를 보여줍니다.
*/
void printPath(void){
	Node * cur, * prev;
	printf("current directory : ");
	for(cur = path.head; cur;){
		prev = cur;
		cur = cur->next;

		printf("/%s", prev->filename);
		free(prev);
	}
	printf("\n");
}

/*
	realpath() (이미 정의되어 있는 함수입니다.)는 현재 디렉토리의 절대경로를 두 번째 인자에 저장합니다.
 	- realpath()는 #include <stdlib.h>를 통해 사용할 수 있습니다.
	- 자세한 내용은 구글링이나 $man 3 realpath 명령을 통해 확인하시면 됩니다.
	this_inode는 현재 디렉터리가 루트(/) 디렉터리까지 가기 위한 비교 값으로 사용됩니다.
	find_inode는 현재 디렉터리에서 지나온(한 단계 아래의) 디렉터리를 찾기 위해 사용하였습니다.	
*/
void printPathTo(ino_t this_inode, ino_t find_inode){
	char real_path[1000];
	static int blank = 1;					// 빈칸 출력을 위함.

	// 상위 디렉터리로 이동하기 전 현재 디렉터리의 절대경로를 real_path에 저장
	realpath(".", real_path);					
	if(get_inode("..") == this_inode)				// root 디렉터리라면
		printf("%-10ld /\n", get_inode("/"));			// 루트(/) 디렉터리의 아이노드 넘버 출력
	else{
		chdir("..");						// 상위 디렉터리로 이동
		printPathTo(get_inode("."), get_inode(real_path));	// recursive call, 
								// chdir("..")을 실행하였으므로 이미 상위 디렉터리로 이동한 상태
								// real_path의 inode는 find_inode가 되며 현재 디렉터리(chdir("..")한 상태)에서
								// 바로 한 단계 전의 디렉터리(chdir("..")을 실행하기 전의 디렉터리)의 아이노드 값을
								// 저장하여 상위 디렉터리에서 하위 디렉터리(바로 전에 지나온)를 찾아서 링크드 리스트에 삽입하여
								// pwd 명령어와 같은 출력을 하기 위함.
	}

	// real_path는 recursive 특성을 이용하여 시스템 스택에 저장된 각 realpath()의 결과가 다르게 됩니다.
	// 즉, 각 시스템 스택에서 real_path 변수는 루트 디렉터리까지 가면서 지나온 디렉터리의 절대 경로가 저장되어 있습니다.
	// printInodeAndFileName()는 real_path에 저장된 절대 경로에 있는 값을 전달하여 해당 디렉터리 내에 있는 파일들을 출력합니다.
	// blank는 전역으로 선언하여 사용할 수도 있습니다.
	// 루트(/) 디렉터리까지 갔다면 시스템 스택에 있는 각 함수들은 전의 함수(자신을 호출한 함수)로
	// 돌아가기 전에 blank를 증가시키며 빈칸 출력을 증가시킵니다.
	printInodeAndFileName(real_path, find_inode, blank++);	 
}

/*
	printInodeAndFileName()
	간략한 설명 : 아이노드 넘버와 해당 파일명을 힙에 들어가는 데이터로 정의하여 힙-소팅 실행
	- fname은 디렉터리(루트 디렉터리로 가면서 지나온 디렉터리)의 절대경로입니다.
	- inode_to_find는 현재 디렉터리로 오기 전의 디렉터리의 inode 값이 담겨져 있습니다.
	- blank는 출력 결과에서 빈칸을 얼마나 출력할지 결정하는 함수입니다.(없어도 됩니다.)	
*/
void printInodeAndFileName(char * fname, ino_t inode_to_find, int blank){
	DIR * dir_ptr;
	struct dirent * direntp;
	HeapData data;
	int i;

	if((dir_ptr = opendir(fname)) == NULL){
		perror(fname);
		exit(1);
	}

	while((direntp = readdir(dir_ptr)) != NULL){
		if(!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, ".."))
			continue;

		// 디렉터리에서 바로 전에 지나온 디렉터리의 아이노드를 찾았다면
		// 링크드 리스트에 저장
		if(direntp->d_ino == inode_to_find)
			insertLList(createNode(direntp->d_name));
		
		// ".", ".."을 제외한 파일과 해당 아이노드를 힙에 삽입 
		insertHeap(direntp->d_ino, direntp->d_name);
	}
	
	// 디렉터리 내의 항목들을 모두 읽었다면(?) heap에는 데이터([inode number, filename] 쌍)를 
	// 하나씩 빼오면서(min heap 기준으로) 출력합니다. (heap-sorting)
	// 여기서 힙은 비워지므로 다음 출력에서도 사용할 수 있습니다.
	while(!isHeapEmpty()){
		data = deleteHeap();
		printf("%-10ld", data.inode_num);
		for(i=0; i<blank; i++) printf("  ");
		printf("%s\n",data.file_name);
	}
}

// 설명 생략
ino_t get_inode(char * filename){
	struct stat info;
	if(stat(filename, &info) == -1){
		fprintf(stderr, "Cannot stat ");
		perror(filename);
		exit(1);
	}
	return info.st_ino;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/* 지료구조 설명은 생략하겠습니다. */
//////////////////////////////////////////////////////////////////////////////////////////////////
void initHeap(Heap * ph){
	ph->numOfData = 0;
}

void insertHeap(ino_t inode_num, char * filename){
	HeapData newData;
	int i = ++(heap.numOfData);
	// create heap data
	newData.inode_num = inode_num;
	strcpy(newData.file_name, filename);

	while((i > 1) && (newData.inode_num < heap.data[i/2].inode_num)){
		heap.data[i] = heap.data[i/2];
		i /= 2;
	}
	heap.data[i] = newData;
}

HeapData deleteHeap(void){
	HeapData delData = heap.data[1];
	HeapData temp = heap.data[(heap.numOfData)--];
	int parent = 1, child = 2;

	while(child <= heap.numOfData){
		if(child < heap.numOfData && heap.data[child].inode_num >= heap.data[child+1].inode_num)
			child += 1;

		if(temp.inode_num <= heap.data[child].inode_num)
			break;

		heap.data[parent] = heap.data[child];
		parent = child;
		child *= 2;
	}

	heap.data[parent] = temp;
	return delData;
}

int isHeapEmpty(void){
	if(heap.numOfData == 0)
		return 1;
	return 0;
}

void initLList(void){
	path.head = NULL;
	path.tail = NULL;
	path.numOfData = 0;
}

Node * createNode(char * fname){
	Node * newNode = (Node*)malloc(sizeof(Node));
	newNode->next = NULL;
	strcpy(newNode->filename, fname);
	return newNode;
}

void insertLList(Node * newNode){
	if(path.head == NULL)
		path.head = newNode;
	else
		path.tail->next = newNode;
	path.tail = newNode;
	path.numOfData += 1;
}
