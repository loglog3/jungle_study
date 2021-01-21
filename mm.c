/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Birthday Party",
    /* First member's full name */
    "이승민",
    /* First member's email address */
    "잘 부탁드립니다. Implicit + Next Fit 입니다",
    /* Second member's full name (leave blank if none) */
    "행복한 하루 되세요",
    /* Second member's email address (leave blank if none) */
    "하나, 둘, 셋, 화이팅!"};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
/////////////////////////////////////////////////////////////
///////그림 9.43 가용리스트 조작을 위한 기본 상수 및 매크로 정의
/* Basic constants and macros */
#define WSIZE 4             /* Word and header/footer size (bytes) */
#define DSIZE 8             /* Double word size (bytes) */
#define CHUNKSIZE (1 << 12) /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))  // find_fit에서 사용하기 위함

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp)-WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp)-WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp)-GET_SIZE(((char *)(bp)-DSIZE)))

static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static char *heap_listp;
void *next;  // 전역변수

///////////////////////////////////////////
////////////////그림 9.46 mm_free 블록을 반환하고 경계 태그 연결을 사용해서
////////////////상수 시간에 인접 가용 블록들과 통합한다

static void *coalesce(void *bp) {
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) { /* Case 1 */
        next = bp;
        return bp;
    }

    else if (prev_alloc && !next_alloc) { /* Case 2 */
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }

    else if (!prev_alloc && next_alloc) { /* Case 3 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    else { /* Case 4 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
                GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    next = bp;
    return bp;
}
///////////////////////////////////////////

///////////그림 9.45 extend_heap 새 가용 블록으로 힙 확장하기
static void *extend_heap(size_t words) {
    // printf("working extend_heap\n");
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));         /* Free block header */
    PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */

    /* Coalesce if the previous block was free */
    return coalesce(bp);
}

/////////////////////////////////////////////////////////////////////////////////////////
//////// 그림 9.44 mm_init: 최초 가용 블록으로 힙 생성하기.
int mm_init(void) {
    /* Create the initial empty heap */
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
        return -1;
    PUT(heap_listp, 0);                                               /* Alignment padding */
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));                    /* Prologue header */
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));                    /* Prologue footer */
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1)); /* Epilogue header */  // 얘가 끝애있는 애입니다
    heap_listp += (2 * WSIZE);

    // printf("working mm_init\n");
    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
        return -1;
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////

void mm_free(void *bp) {
    // puts("working mm_free");
    // next = bp;
    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}

///////////////////place
static void place(void *bp, size_t asize) {  // 받은 bp에 asize를 지정하고, allocate 을 1로 해야한다.
    size_t csize = GET_SIZE(HDRP(bp));

    if ((csize - asize) >= (2 * DSIZE)) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize - asize, 0));
        PUT(FTRP(bp), PACK(csize - asize, 0));
    } else {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
    // #############################################comment###################################################
    /*
    안녕하세요! 이유섭입니다.
    next fit은 탐색이 종료된 시점을 기억하고 있다가 malloc이 다시 실행되면,
    기억하고 있는 block부터 탐색해 나가는 알고리즘이라고 알고 있습니다...!

    그런데, place와 extend함수들에 이를 기억하고자 next를 업데이트 해주는 코드가
    보이지 않아 comment 남깁니다.

    이 자리에 next = bp; 코드를 한줄 추가하면, 메모리를 할당한 자리를 기억하고
    그 다음부터 탐색해나갈 수 있을 것 같습니다.
    */
    
    /*
    1. 이 코드를 추가해 보세요!      
    next = bp;
    */

    /*  
    2. 아래의 코드로 작성했을 때도 동작해야할 것 같은데, 7,8번 trace에서 고장이 나네요ㅠㅠ
    if(GET_SIZE(HDRP(bp)) == 0){
        next = heap_listp;
    }
    else{
        next = NEXT_BLKP(bp);
    }
    */
}
/////////////find_fit

//void *next;
#define FINDBP (char *)(heap_listp)    // 포인터끼리 더하기

static void *find_fit(size_t asize) {  //새로운 블록이 들어갈 bp를 리턴해야한다.
    //######## origin ########
    // puts("working find_fit");
    // printf("next value:  %p\n", (char *)next);
    void *bp;

    for (bp = next; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
            return bp;
        }
    }

    // #############################################comment###################################################
    /*
    안녕하세요! 이유섭입니다.
    next fit으로 잘 구현해주신 것 같습니다!

    next 포인터의 위치에 상관없이 블럭들을 모두 탐색하기 위해서는 bp가 epilogue에 도달했을 때,
    return NULL을 하는 것이 아니라, 처음 위치 (heap listp 혹은 heap_listp의 next block)로 
    bp를 갱신하며 iteration을 이어나가야 한다고 생각합니다.

    가용 가능한 자신보다 큰 블럭을 찾다가, 최초의 next 포인터위치로 되돌아오면 iteration을 종료합시다!
    */
    

    //######## 1 ########
    // void *bp = next;
    // while(bp != PREV_BLKP(next)){ //한바퀴를 돌아 자신의 prev_block까지 탐색해 나간다.
    //     if( !GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))){
    //         return bp;
    //     }
    //     if (0 == GET_SIZE(HDRP(bp))){ //epilogue에 도달하면 bp를 heap_listp로 업데이트
    //         bp = heap_listp;
    //     }
    //     else{
    //       bp = NEXT_BLKP(bp);
    //     }
    // }


    //######## 2 ########
    // void *bp = next;
    // while(1){
    //     if( !GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))){
    //         return bp;
    //     }
    //     if (0 == GET_SIZE(HDRP(bp))){ //epilogue에 도달하면 bp를 heap_listp로 업데이트
    //         bp = heap_listp;
    //     }
    //     else{
    //       bp = NEXT_BLKP(bp);
    //     }

    //     if(bp == next){ //한바퀴를 돌아 자신의 prev_block까지 탐색해 나간다.
    //         break;
    //     }
    // }



    return NULL;
}

////////////////////////////////////////////////////////////////////
//////////그림 9.47 mm_mallc 가용 리스트에서 블록 할당하기.
void *mm_malloc(size_t size) {
    // puts("working mm_malloc");
    size_t asize;      /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap if no fit */
    char *bp;

    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE)
        asize = 2 * DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
        return NULL;
    place(bp, asize);
    return bp;
}
////////////////////////////////////////////////////////////

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */    

// #############################################comment###################################################

/*
https://foramonth.tistory.com/42
제 블로그인데 밑에 realloc관련 내용이 있습니다!
본래의 realloc함수의 역할과 비슷하게 다시 realloc을 구현해봤는데 공유해드립니다.
참고만 해주세요 realloc은!
*/


//######## origin ########
// void *mm_realloc(void *ptr, size_t size) {
//     void *oldptr = ptr;
//     void *newptr;
//     size_t copySize;

//     newptr = mm_malloc(size);
//     if (newptr == NULL)
//         return NULL;
//     copySize = *(size_t *)((char *)oldptr - WSIZE);
//     if (size < copySize)
//         copySize = size;
//     memcpy(newptr, oldptr, copySize);
//     mm_free(oldptr);
//     return newptr;
// }


void *mm_realloc(void *ptr, size_t size)
{
    void *new_ptr = ptr; /* Pointer to be returned */
    void *bp;
    size_t new_size = size; /* Size of new block */
    int remainder;          /* Adequacy of block sizes */
    int extendsize;         /* Size of heap extension */
    int block_buffer;       /* Size of block buffer */
    /* Ignore invalid block size */
    if (size == 0)
        return NULL;
    /* Adjust block size to include boundary tag and alignment requirements */
    if (new_size <= DSIZE)
    {
        new_size = 2 * DSIZE;
    }
    else
    {
        new_size = ALIGN(size + DSIZE);
    }

    if ( GET_SIZE(HDRP(ptr)) < new_size) //늘리고싶다.
    {
        if (!GET_ALLOC(HDRP(NEXT_BLKP(ptr))))//next block is not allocated
        {
            remainder = GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr))) - new_size;
            if (remainder >= 0)
            {
                PUT(HDRP(ptr), PACK(new_size + remainder, 1));
                PUT(FTRP(ptr), PACK(new_size + remainder, 1));
            }
            else
            {
                if((new_ptr = mm_malloc(new_size - DSIZE)) == NULL){
                    return NULL;
                }
                memcpy(new_ptr, ptr, MIN(size, new_size));
                mm_free(ptr);
            }
        }
        else if (!GET_SIZE(HDRP(NEXT_BLKP(ptr))))//next block is epilogue
        {
            remainder = GET_SIZE(HDRP(ptr)) - new_size;
            extendsize = MAX(-remainder, CHUNKSIZE);
            if ((long)(bp = mem_sbrk(extendsize)) == -1)
            {
                return NULL;
            }
            PUT(HDRP(bp), PACK(extendsize, 0));
            PUT(FTRP(bp), PACK(extendsize, 0));
            PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
            PUT(HDRP(ptr), PACK(GET_SIZE(HDRP(ptr)) + extendsize, 0));
            PUT(FTRP(ptr), PACK(GET_SIZE(HDRP(ptr)), 0));
            place(ptr, new_size);
        }
        else
        {
            if((new_ptr = mm_malloc(new_size - DSIZE)) == NULL){
                return NULL;
            }
            memcpy(new_ptr, ptr, MIN(size, new_size));
            mm_free(ptr);
        }
    }
    return new_ptr;
}
