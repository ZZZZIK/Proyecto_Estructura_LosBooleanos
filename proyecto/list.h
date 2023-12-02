#ifndef List_h
#define List_h

typedef struct Node{
    void * data;
    struct Node * next;
    struct Node * prev;
} Node;

typedef struct List{
    Node * head;
    Node * tail;
    Node * current;
} List;

List * createList(void);

void * firstList(List * list);

void * nextList(List * list);

void * lastList(List * list);

void * prevList(List * list);

void pushFront(List * list, void * data);

void pushBack(List * list, void * data);

void pushCurrent(List * list, void * data);

void * popFront(List * list);

void * popBack(List * list);

void * popCurrent(List * list);

void cleanList(List * list);

int sizeList(List * list);

#endif /* List_h */