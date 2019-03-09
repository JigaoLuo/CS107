## Int Stack

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
|-|---|---|---|---|``

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
  s->elems = malloc(4 * sizeof(int));  // 4 * 4
  assert(s->elems != NULL);
}

void StackDispose(Stack *s)
{
  free(s->elems);
}
```


```C++
void StackPush(Stack* s, int value)
{
  if (s->logicalLen == s->allocLen) {
    s->allocLen *= 2;
    s->elems = realloc(s->elems, s->allocLen * sizeof(int));
    assert(s->elems != NULL);  // a Marko
  }
  s->elems[s->logicalLen] = value;
  s->logicalLen++;
}
```

`realloc:`  
takes parameter: pointing to a dynamically allocated block, and it just takes care of all the details to make that block this big.   
1. If it can **resize it in place**, all it does is it records that the block has been extended to include more space and it returns the same exact address.

    |add_of_array|0|1|2|3|
    |-|---|---|---|---|

    to

    |add_of_array|0|1|2|3|-|-|-|-|
    |-|---|---|---|---|---|---|---|---|

2. However, it may be the case that you pass that in and you want to double the size or make it bigger, and that space is in use. So it actually does a lot of work for you there.  It really calls malloc somewhere else on your behalf. 

    |add_of_array_old (will be freed)|0|1|2|3|
    |-|---|---|---|---|

    to

    |add_of_array_new (will be returned)|0|1|2|3|-|-|-|-|
    |-|---|---|---|---|---|---|---|---

Complexity: $O(m + n)$  
$m$: # Elements in Heap to find a space can be allocated  
$n$: # Times of copy


<br>
<br>
<br>

```C++
int StackPop(Stack* s)
{
  assert(s->logicalLen > 0);
  s->logicalLen--;
  return s->elems[s->logicalLen];
}
```

<br>
<br>
<br>
<br>
<br>

## Generic Stack
### `Stack.h`

```C++
typedef struct {
  void* elems;
  int elemSize;  // new field
  int logicalLen;
  int allocLeng;
} Stack;

void StackNew(Stack* s, int elemSize);

void StackDispose(Stack* s);

void StackPush(Stack* s, void* elemAddr);

void StackPop(Stack* s, void* elemAddr);
```

<br>
<br>

```C++
void StackNew(Stack* s, int elemSize)
{
  s->elemSize = elemSize;
  asser(s->elemSize > 0);
  s->logicalLen = 0;
  s->allocLen = 4;
  s->elems = malloc(4 * elemSize);  // 4 * 4
  assert(s->elems != NULL);
}

void StackDispose(Stack *s)
{
  free(s->elems);
}
```

Example: `Stack` for `double`:

|add_1| add_of_array|
|-|---|

|add_1 + 4| 8|
|-|---|

|add_1 + 8| 0|
|-|---|

|add_1 + 12| 4|
|-|---|

<br>

|add_of_array (8B * 4 = 32B)|-|-|-|-|
|-|---|---|---|---|


```C++
void StackPush(Stack* s, void* elemAddr)
{
  if (s->logicalLen == s->allocLen) {
    StackGrow(s);
  }

  // 定位到最后一个Stack位子 (unfilled) 
  // s->elems is void*, must be cast to (char*) to do pointer arithmetic 
  void* target = (char*)s->elems + s->logicLen * s->elemSize; 
  memcpy(target, elemAddr s->elemSize);
  s->logicalLen++;
}
```

Example: `Stack` for `double` with already 3 elements:

|add_1| add_of_array|
|-|---|

|add_1 + 4| 8|
|-|---|

|add_1 + 8| 3|
|-|---|

|add_1 + 12| 4|
|-|---|

<br>

|add_of_array (8B * 4 = 32B)|filled|filled|filled|-|
|-|---|---|---|---|


|elemAddr|to be pushed in Stack (4B)|
|-|---|


<br>
<br>
<br>

```C++
static void StackGrow(Stack* s)
{
  s->allocLen *= 2;
  s->elems = realloc(s>elems, s->allocLen * s->elemSize);
}
```

`StackPush`, and `StackNew`, and `StackDispose` are all marked as global functions, and that the symbols, or the names of those functions, should be exported and accessible from other `.o` files, or made available to other `.o` files. 

`StackGrow` is local or internal. And even though the function name exists, it can’t be called from other files. That may seem like it was a silly waste of time, but it really is not.
Because you can imagine in a code base of say one million files. Think Microsoft Office, the whole thing, probably has on the order of hundreds of thousands of files, maybe tens of thousands, I don’t know what it is, more than a few. You can imagine a lot of people defining their own little `swap` functions. And if they’re not marked as internal functions at the time that everything is linked together to build Word or Excel or whatever, the linker is gonna freak out and say, which version of `swap` do I call? I can’t tell. Because it has 70 million of them. But if all 70 million are
marked as `private`, or `static`, then there’s none of those collisions going on at the time the application’s built.


<br>
<br>
<br>

```C++
void StackPo(Stack* s, void* elemAddr)
{
  void* source = (char*)s->elems + (s->logicLen - 1) * s->elemSize;
  memcpy(elemAddr, source, s->elemSize);
  s->logicLen--;
}
```