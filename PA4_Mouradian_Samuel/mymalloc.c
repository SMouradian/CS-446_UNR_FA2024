// Assignment Name - Homework 4
// Author - Samuel Mouradian
// Computer Science 446 - Section 1001
// Professor - Christos Papachristos
// Due Date - 11.19.2024


// LIBRARIES
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>


// DEFINITIONS
#define MBLOCK_HEADER_SZ offsetof(mblock_t, payload)


// OTHER DECLARATIONS
typedef struct _mblock_t{
    struct _mblock_t* prev;
    struct _mblock_t* next;
    size_t size;
    int status;
    void* payload;
} mblock_t;

typedef struct _mlist_t{
    mblock_t* head;
} mlist_t;


// GLOBAL VARIABLES
mlist_t mlist = {.head = NULL};


// FUNCTION DECLARATIONS
void* mymalloc(size_t);
void myfree(void* ptr);
mblock_t* findLastMemListBlock();
mblock_t* findFreeBlockOfSize(size_t size);
void splitBlockAtSize(mblock_t* block, size_t newSize);
void coallesceBlockPrev(mblock_t* freedBlock);
void coallesceBlockNext(mblock_t* freedBlock);
mblock_t* growHeapBySize(size_t size);


// MAIN
int main(){
    void* p1 = mymalloc(10);
    void* p2 = mymalloc(100);
    void* p3 = mymalloc(200);
    void* p4 = mymalloc(500);
    myfree(p3); p3 = NULL;
    myfree(p2); p2 = NULL;
    void* p5 = mymalloc(150);
    void* p6 = mymalloc(500);
    myfree(p4); p4 = NULL;
    myfree(p5); p5 = NULL;
    myfree(p6); p6 = NULL;
    myfree(p1); p1 = NULL;
}


// FUNCTION DEFINITIONS
void myfree(void* ptr){
    if(ptr == NULL){
        return;
    }

    mblock_t* block = (mblock_t*)((char*)ptr - MBLOCK_HEADER_SZ);

    block->status = 0;
    if(block->prev != NULL && block->prev->status == 0){
        coallesceBlockPrev(block);
    }

    if(block->next != NULL && block->next->status == 0){
        coallesceBlockNext(block);
    }
}

void* mymalloc(size_t size){
    mblock_t* freeBlock = findFreeBlockOfSize(size);

    if(freeBlock != NULL){
        splitBlockAtSize(freeBlock, size);
        return freeBlock->payload;
    }

    return growHeapBySize(size) != NULL ? (findLastMemListBlock()->payload) : NULL;
}

mblock_t* findLastMemListBlock(){
    mblock_t* currBlock = mlist.head;

    while (currBlock->next != NULL) {
        currBlock = currBlock->next;
    }

    return currBlock;
}

mblock_t* findFreeBlockOfSize(size_t size){
    mblock_t* currBlock = mlist.head;

    while(currBlock != NULL){
        if(currBlock->status == 0 && currBlock->size >= size){
            return currBlock;
        }
        currBlock = currBlock->next;
    }

    return NULL;
}

void splitBlockAtSize(mblock_t* block, size_t newSize){
    if(block->size >= newSize + MBLOCK_HEADER_SZ){
        mblock_t* newBlock = (mblock_t*)((char*)block + MBLOCK_HEADER_SZ + newSize);

        newBlock->size = block->size - newSize - MBLOCK_HEADER_SZ;
        newBlock->status = 0;
        newBlock->payload = (void*)((char*)newBlock + MBLOCK_HEADER_SZ);

        block->size = newSize;
        block->status = 1;
        block->payload = (void*)((char*)block + MBLOCK_HEADER_SZ);

        newBlock->next = block->next;
        newBlock->prev = block;

        if(block->next != NULL){
            block->next->prev = newBlock;
        }

        block->next = newBlock;
        
        if(newBlock->next == NULL) {
            mblock_t* lastBlk = findLastMemListBlock();
            if (lastBlk == block) {
                mlist.head = block;
            }
        }
    }
}

void coallesceBlockPrev(mblock_t* freedBlock){
    if(freedBlock != NULL && freedBlock->prev != NULL){
        mblock_t* prevBlk = freedBlock->prev;

        if(prevBlk->status == 0){
            mlist.head = prevBlk;
        }

        freedBlock->next = NULL;
        freedBlock->prev = NULL;
    }
    else{}
}

void coallesceBlockNext(mblock_t* freedBlock){
    if(freedBlock != NULL && freedBlock->next != NULL && freedBlock->next->status != 0){
        mblock_t* nextBlk = freedBlock->next;

        if(nextBlk->status == 0){
            nextBlk->size += freedBlock->size + MBLOCK_HEADER_SZ;
            nextBlk->next = freedBlock->next;
            
            if(nextBlk->next != NULL){
                nextBlk->next->prev = freedBlock;
            }
            
            nextBlk->next = NULL;
            nextBlk->prev = NULL;
        }
    }
}

mblock_t* growHeapBySize(size_t size){
    size_t totalSize = size + MBLOCK_HEADER_SZ;
    void* newHeapSpace = sbrk(totalSize);

    mblock_t* newBlock = (mblock_t*)newHeapSpace;

    newBlock->size = size;
    newBlock->status = 0;
    newBlock->prev = NULL;
    newBlock->next = NULL;
    newBlock->payload = (void*)((char*)newBlock + MBLOCK_HEADER_SZ);

    if (mlist.head == NULL) {
        mlist.head = newBlock;
    } else {
        mblock_t* lastBlk = findLastMemListBlock();
        lastBlk->next = newBlock;
        newBlock->prev = lastBlk;
    }

    return newBlock;
}