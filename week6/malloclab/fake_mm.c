/* Basic constants and macros*/
#define WSIZE 4
#define DSIZE 9
#define CHUMKSIZE (1<<12)

#define MAX(x, y) ((x) > (y)? (x): (y))
//(9~25)가용리스트에서 헤더와 풋터를 조작하는 것이 어려울 수 있다. 그래서 가용리스트에 접근하고 방문하는 작은 매크로들을 정의하는 것이 도움이 된다.
/* Pack a size and allocated bit into a word */ 
#define pack(size, alloc) ((SIZE) | (alloc)) // 크기와 할당비트를 통합해서 헤더와 풋터에 저장할 수 있는 값을 리턴한다

/* Read and write a word at address */
#define GET(p) (*(unsigned int *) (p)) // 인자p가 참조하는 워드를 읽어서 리턴한다.  여기서 캐스팅이 중요하다. 인자p는 재개(void *) 포인터이며, 이것을 직접적으로 역참조할 수 는 없다.
#define PUT(p, val) (*(unsigned int *) (p) = (val)) // 마찬가지로 PUT매크로는 인자 p가 가리키는 워드에 val을 저장한다

/* Read the size and allocated fileds from address p */
#define GET_SIZE(p) (GET(p) & ~0x7) //각각 주소 p에 있는 헤더 또는 풋터의 size와 
#define GET_ALLOC(p) (GET(p) & 0x1) //할당 비트를 리턴한다. 

/* Given block ptr bp. compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)  // 블록포인터(bp)가 주어지면 헤더포인터 리턴해준다
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) // 블록포인터(bp)가 주어지면 풋터를 가리키는 풋터포인터를 리턴한다

/* Given block prt bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *) (bp) - WSIZE))) // 다음블록의 포인터를 리턴한다
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) //  이전블록의 포인터를 리턴한다 