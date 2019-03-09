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
void StackPop(Stack* s, void* elemAddr)
{
  void* source = (char*)s->elems + (s->logicLen - 1) * s->elemSize;
  memcpy(elemAddr, source, s->elemSize);
  s->logicLen--;
}
```


Use a generic stack to store a collection of strings and print them out in reverse order.

```C++
int main(int argc, char** argv)
{
  const char* friends[3] = {"AL", "Bob", "Carl"};
  Stack stringStack;
  StackNew(&string， sizeof(char*));
  for (int i = 0; i < 3; ++i) {
    char* copy = strdup(friend[i]);
    StackPush(&stringStack, &copy);
  }
  char* name;
  for (int i = 0; i < 3; ++i) {
    StackPop(&stringStack, &name);
    printf("%s\n", name);
    free(name);
  }
  StackDispose(&stringStack);
}
```

- After `Stack stringStack;`: 16 Bytes of **nothing** meaningful

|add_1| ---|
|-|---|

|add_1 + 4| ---|
|-|---|

|add_1 + 8| ---|
|-|---|

|add_1 + 12| ---|
|-|---|

<br>

|char_add_1| "AL\0"|
|-|---|

|char_add_2| "Bob\0"|
|-|---|

|char_add_3| "Carl\0"|
|-|---|

<br>
<br>
<br>

- After `StackNew(&string， sizeof(char*));`: 16 Bytes of **something** 

|add_1| add_of_array|
|-|---|

|add_1 + 4| 4 - elemSize - sizeof(char*)|
|-|---|

|add_1 + 8| 0|
|-|---|

|add_1 + 12| 4 - allocLeng|
|-|---|

<br>

|add_of_array (4B * 4 = 16B)|-|-|-|-|
|-|---|---|---|---|

|char_add_1| "AL\0"|
|-|---|

|char_add_2| "Bob\0"|
|-|---|

|char_add_3| "Carl\0"|
|-|---|

<br>
<br>
<br>

- `i = 0`:

|copy|char_add_1|
|-|---|

|ptr_to_copy|copy|
|-|---|

a deep copy of `"Al\0"` in the heap.

When you `push` an element onto the stack, **you transfer ownership from you to the stack**. 

|add_1| add_of_array|
|-|---|

|add_1 + 4| 4 - elemSize - sizeof(char*)|
|-|---|

|add_1 + 8| 1|
|-|---|

|add_1 + 12| 4 - allocLeng|
|-|---|

<br>

|add_of_array (4B * 4 = 16B)|char_add_1|-|-|-|
|-|---|---|---|---|

|char_add_1| "AL\0"|
|-|---|

|char_add_2| "Bob\0"|
|-|---|

|char_add_3| "Carl\0"|
|-|---|


<br>
<br>
<br>

- `i = 1`:

|copy (redeclared and reinitialized)|char_add_2|
|-|---|

|ptr_to_copy|copy|
|-|---|

<br>

|add_1| add_of_array|
|-|---|

|add_1 + 4| 4 - elemSize - sizeof(char*)|
|-|---|

|add_1 + 8| 2|
|-|---|

|add_1 + 12| 4 - allocLeng|
|-|---|

<br>

|add_of_array (4B * 4 = 16B)|char_add_1|char_add_2|-|-|
|-|---|---|---|---|

|char_add_1| "AL\0"|
|-|---|

|char_add_2| "Bob\0"|
|-|---|

|char_add_3| "Carl\0"|
|-|---|

<br>
<br>
<br>

- `i = 2`:

|copy (redeclared and reinitialized)|char_add_3|
|-|---|

|ptr_to_copy|copy|
|-|---|

<br>

|add_1| add_of_array|
|-|---|

|add_1 + 4| 4 - elemSize - sizeof(char*)|
|-|---|

|add_1 + 8| 3|
|-|---|

|add_1 + 12| 4 - allocLeng|
|-|---|

<br>

|add_of_array (4B * 4 = 16B)|char_add_1|char_add_2|char_add_3|-|
|-|---|---|---|---|

|char_add_1| "AL\0"|
|-|---|

|char_add_2| "Bob\0"|
|-|---|

|char_add_3| "Carl\0"|
|-|---|


<br>
<br>
<br>

```C++
for (int i = 0; i < 3; ++i) {
  StackPop(&stringStack, &name);
  printf("%s\n", name);
  free(name);
}
```
So, I’m really transferring ownership of these three
dynamically allocated copies over to the stack. What I wanna do now is I wanna go ahead and I wanna print these things out. I really wanna ask for ownership back.

- `i = 1`:

|name|char_add_3|
|-|---|

|ptr_to_name|name|
|-|---|

<br>

|add_1| add_of_array|
|-|---|

|add_1 + 4| 4 - elemSize - sizeof(char*)|
|-|---|

|add_1 + 8| 2|
|-|---|

|add_1 + 12| 4 - allocLeng|
|-|---|

<br>

|add_of_array (4B * 4 = 16B)|char_add_1|char_add_2|-|-|
|-|---|---|---|---|

|char_add_1| "AL\0"|
|-|---|

|char_add_2| "Bob\0"|
|-|---|

|char_add_3| "Carl\0"|
|-|---|



<br>
<br>
<br>

### New version of `StackNew`

#### Motivation

```C++
int main(int argc, char** argv)
{
  const char* friends[3] = {"AL", "Bob", "Carl"};
  Stack stringStack;
  StackNew(&string， sizeof(char*));
  for (int i = 0; i < 3; ++i) {
    char* copy = strdup(friend[i]);
    StackPush(&stringStack, &copy);
  }
  StackDispose(&stringStack);
}
```

```C++
void StackDispose(Stack *s)
{
  free(s->elems);
}
```
Now, the problem comes, when I actually call `StackDispose`, the stack actually has some elements that it still owns.

`Stac ispose` should be able to say, “Okay, I seem to have retained ownership of some elements that were pressed onto me. I would like to be able to dispose of these things on behalf of the client before I go and clean up, and donate this blob of memory back to the heap.” 

Many times there’s nothing to be done at all. When this thing stores `int`s, or `long`s, or `double`s, or `char`s, you don’t have to go in and zero them out. That’s really not that useful. 

You do have to be careful about donating back any **dynamically allocated
resources**, or maybe any open files. That’s less common, but dynamic memory allocation is certainly more common. If these things really are owned by the stack at the time it’s disposed of, then the `StackDispose` function has to figure out how to actually pass these things right there to `free`. It’s actually very difficult to do that because the implementation of `StackDispose` doesn’t actually know that these things are pointers. It just knows, at best, that they’re four-byte figures. It is capable of computing these addresses, and so if the depth of the stack is three, so that those three arrows are arrows that point to elements it’s holding for the client. 

#### Upgrade
What I wanna do here is I want to upgrade this right here to not take two arguments, but to take three arguments.

```C++
typedef struct { // upgraded
  void* elems;
  int elemSize;  // new field
  int logicalLen;
  int allocLeng;
  void (*freeFun)(void*);
} Stack;

void StackNew(Stack* s, int elemSize, void (*freeFun)(void*)) // upgraded
{
  s->elemSize = elemSize;
  asser(s->elemSize > 0);
  s->logicalLen = 0;
  s->allocLen = 4;
  s->elems = malloc(4 * elemSize);  // 4 * 4
  assert(s->elems != NULL);
}
```
The fifth field `void (*freeFun)(void*)` is actually either some `NULL` pointer, or a pointer to a legitimate合法 freeing function.

`void (*freeFun)(void*)` takes a void* and doesn’t return anything.

The idea here is that I want to pass to the constructor function information about **how to destroy any elements that it holds for me when I call `StackDispose`.**

Those three arrows – if you’re dealing with a stack of depth three, and it’s storing strings – it’s prepared to pass those three things in sequence. Those three things in sequence to this function that you write and pass `freeFun` And it will invoke `freeFun` for every single element it
holds for you.


|add_1| add_of_array|
|-|---|

|add_1 + 4| 4 - elemSize - sizeof(char*)|
|-|---|

|add_1 + 8| 0|
|-|---|

|add_1 + 12| 4 - allocLeng|
|-|---|

|add_1 + 16| freeFun_add|
|-|---|

<br>

|freeFun_add| freeFun|
|-|---|



If you’re storing these base types that have no freeing needs, I expect the client to pass an `NULL` here, and that will be checked for and `StackDispose`.  
If you’re storing things like `char*`s, or `pointers to structs`, or even `direct struct`s that have pointers inside that are **pointed to dynamically allocated memory**, then you have a **meaningful free function**.

```C++
void StackDispose(Stack *s)  // upgraded
{
  if (s->freeFun != NULL) {
    for (int i =0; i < s->logicLen; ++i) {
      s->freeFun((char*)s->elems + i * s->elemSize);
    }
  }
  free(s->elems);
}
```

<br>
<br>
<br>

- Example with `String` (for the situation above before upgrading):
```C++
void stringFree(void* elem)
{
  free(*(char**)elem);
}
```

```C++
const char* friends[3] = {"AL", "Bob", "Carl"};
Stack stringStack;
StackNew(&string， sizeof(char*), stringFree);
// ...
```

- **Wrong** example with `String`:

```C++
void stringFree(void* elem)
{
  free((char*)elem); // pointer wrongly placed
}
```

**What would happen?**  
Only `free`: `s->elems`, `s->elems + 4`, `s->elems + 8` ... in the case of `String`. $\equiv$ `char_add_1`, `char_add_2`, `char_add_3` will be freed. These things are not `malloc` or `realloc` in Heap. 

Whan we want: the `char*` pointed by `char_add_1`, `char_add_2`, `char_add_3` should be freed. These things are `malloc` or `realloc` in Heap. 


|add_1| s->elems|
|-|---|

|add_1 + 4| 4 - elemSize - sizeof(char*)|
|-|---|

|add_1 + 8| 2|
|-|---|

|add_1 + 12| 4 - allocLeng|
|-|---|

<br>

|s->elems (4B * 4 = 16B)|char_add_1|char_add_2|char_add_3|-|
|-|---|---|---|---|

|char_add_1| "AL\0"|
|-|---|

|char_add_2| "Bob\0"|
|-|---|

|char_add_3| "Carl\0"|
|-|---|