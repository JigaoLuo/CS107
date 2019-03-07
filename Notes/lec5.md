## Stack

### `Stack.h`
```C++
typedef struct {
  int* elems;
  int logicalLen;
  int allocLeng;
} Stack;

void StackNew(Stack* s);

void StackDispose(Stack* s);

void StackPush(Stack* s, int value);

void StackPop(Stack* s);

bool isEmtpy();
```

`void StackDispose(Stack* s);` identify the address of the stack structure that should be disposed. This is gonna be a dynamically allocated array that’s not perfectly sized. So I want to keep track of how much space I have and how much of it I’m using.


`Stack s;`  
The allocation of a stack conceptually is to get space - 12 bytes.  
But as far as the declaration is concerned, it doesn’t actually clear these out, or zero them out like Java does, it just inherits whatever bits happen to reside in the 12 bytes that are overlaid by this new variable 继承原有的Bytes.
|add_1| elems 原有的Bytes|
|-|---|

|add_1 + 4| logicalLen 原有的Bytes|
|-|---|

|add_1 + 8| allocLeng 原有的Bytes|
|-|---|


`StackNew(&s);`  
When I call `StackNew` that I pass in the address of `Stack`.   
I’m going to logically do the following:
take the raw space, set up in this way. I’m gonna set it’s length to be zero, make space for four elements, store the address of a dynamically allocated array of integers

`logicalLen:` can be used not only to **store the effective depth of the stack**, but it can **also identify the index** where I’d like to place
the next integer to be pushed. So because I’m **pre-allocating space for four elements, that means that this is a function.**

|add_1| add_of_array|
|-|---|

|add_1 + 4| 0|
|-|---|

|add_1 + 8| 4|
|-|---|

<br>

|add_of_array|-|-|-|-|
|-|---|---|---|---|

`Push` is able to run very, very quickly for the very first 4 `push`. But it’s only when I actually `push` a fifth element, that I detect that  llocated space has been saturated, that I have to  allocate another array that’s twice as big, and move everything over, and then carry on as if the array were of length 8 instead of 4 all along.

```C++
for (int i = 0; i < 5; ++i) {
  StackPush(&s, i);
}
StackDispose(&s);
```

|add_1| add_of_array|
|-|---|

|add_1 + 4| 1|
|-|---|

|add_1 + 8| 4|
|-|---|

<br>

|add_of_array|0|-|-|-|
|-|---|---|---|---|

to

|add_1| add_of_array|
|-|---|

|add_1 + 4| 2|
|-|---|

|add_1 + 8| 4|
|-|---|

<br>

|add_of_array|0|1|-|-|
|-|---|---|---|---|

to

|add_1| add_of_array|
|-|---|

|add_1 + 4| 3|
|-|---|

|add_1 + 8| 4|
|-|---|

<br>

|add_of_array|0|1|2|-|
|-|---|---|---|---|

to

|add_1| add_of_array|
|-|---|

|add_1 + 4| 4|
|-|---|

|add_1 + 8| 4|
|-|---|

<br>

|add_of_array|0|1|2|3|
|-|---|---|---|---|

<br>
<br>
<br>

I don’t have space for the $4th$ `push`. So I have to go and allocate space for everything. So I’m gonna use this **doubling** strategy, where I’ve gotta set aside space for eight elements.  
I copy over all the data. I free old array. Get this to point to this space as if it were the original figure I’ve allocated. Forget about that smaller house, I’ve moved into a bigger house and I hated the older house. 

|add_1| add_of_array_new|
|-|---|

|add_1 + 4| 4|
|-|---|

|add_1 + 8| 4|
|-|---|

<br>

|add_of_array(will be freed)|0|1|2|3|
|-|---|---|---|---|

|add_of_array_new|0|1|2|3|4|-|-|-|
|-|---|---|---|---|---|---|---|---|

<br>
<br>
<br>

---
### `Stack.c`

```C++
void StackNew(Stack* s)
{
  s->logicalLen = 0;
  s->allocLen = 4;
  s->elems = malloc(4 * sizeof(int));
  assert(s->elems != NULL);
}
```
