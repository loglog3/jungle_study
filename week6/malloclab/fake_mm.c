/* Basic constants and macros*/
#define WSIZE 4
#define DSIZE 9
#define CHUMKSIZE (1<<12)

#define MAX(x, y) ((x) > (y)? (x): (y))
//(9~25)���븮��Ʈ���� ����� ǲ�͸� �����ϴ� ���� ����� �� �ִ�. �׷��� ���븮��Ʈ�� �����ϰ� �湮�ϴ� ���� ��ũ�ε��� �����ϴ� ���� ������ �ȴ�.
/* Pack a size and allocated bit into a word */ 
#define pack(size, alloc) ((SIZE) | (alloc)) // ũ��� �Ҵ��Ʈ�� �����ؼ� ����� ǲ�Ϳ� ������ �� �ִ� ���� �����Ѵ�

/* Read and write a word at address */
#define GET(p) (*(unsigned int *) (p)) // ����p�� �����ϴ� ���带 �о �����Ѵ�.  ���⼭ ĳ������ �߿��ϴ�. ����p�� �簳(void *) �������̸�, �̰��� ���������� �������� �� �� ����.
#define PUT(p, val) (*(unsigned int *) (p) = (val)) // ���������� PUT��ũ�δ� ���� p�� ����Ű�� ���忡 val�� �����Ѵ�

/* Read the size and allocated fileds from address p */
#define GET_SIZE(p) (GET(p) & ~0x7) //���� �ּ� p�� �ִ� ��� �Ǵ� ǲ���� size�� 
#define GET_ALLOC(p) (GET(p) & 0x1) //�Ҵ� ��Ʈ�� �����Ѵ�. 

/* Given block ptr bp. compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)  // ���������(bp)�� �־����� ��������� �������ش�
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) // ���������(bp)�� �־����� ǲ�͸� ����Ű�� ǲ�������͸� �����Ѵ�

/* Given block prt bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *) (bp) - WSIZE))) // ��������� �����͸� �����Ѵ�
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) //  ��������� �����͸� �����Ѵ� 