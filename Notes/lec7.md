## `rotate`

```C++
void rotate(void* front, void* middle, void* end);
```

|a (pointed by `front`)|b|c|d(pointed by `middle`)|e|f|g|h(pointed by `end`)|
|-|---|---|---|---|---|---|---|---|

to

|e|f|g|h(pointed by `end`)||a (pointed by `front`)|b|c|d(pointed by `middle`)|
|-|---|---|---|---|---|---|---|---|

<br>
<br>
<br>

### Naive Idea
(`front` to `middle`) in a `buffer` like in function swap  
(`middle` to `end`) be `memcpy`ed to `front` position.

**But:**
The problem with that is that – unlike all the other examples we’ve dealt with – the source region (`middle` to `end`), and this space starting at `front` can **potentially overlap**. 

The implementation of `memcpy` is brute force. It carries things four bytes at a time, and then at the end does whatever mod tricks it needs to copy off an odd number of bytes, **but it assumes they’re not overlapping**. When they’re overlapping, that brute force
approach might not work.

`memcpy` said, “I don’t wanna bother with that. I wanna **run as quickly as possible**, and I want the client to take responsibility of only calling `memcpy` when, in fact, he or she knows that there’s no overlap.”

Before `memcpy`:
|a|b|c|d|e|-|-|
|-|---|---|---|---|---|---|---|---|

After `memcpy`:
|a|b|_a_|d|e|-|-|
|-|---|---|---|---|---|---|---|---|

|a|b|_a_|_b_|e|-|-|
|-|---|---|---|---|---|---|---|---|

|a|b|_a_|_b_|_a_|-|-|
|-|---|---|---|---|---|---|---|---|

|a|b|_a_|_b_|_a_|_b_|-|
|-|---|---|---|---|---|---|---|---|

|a|b|_a_|_b_|_a_|_b_|_a_|
|-|---|---|---|---|---|---|---|---|

<br>
<br>
<br>

#### `memmove`


If they don’t know if there’s gonna be overlap they have to use a version of the function that’s **slightly less efficient, but does the error checking**.
It has the same exact prototype – that shouldn’t surprise you – but it’s not called `memcpy`. It’s called `memmove`.



```C++
void rotate(void* front, void* middle, void* end)
{
  int frontSize = (char*)middle - (char*)front;  // # Bytes between that
  char buffer[frontSize];
  memcpy(buffer, front, frontSize)

  int backSize = (char*)end - (char*)middle;  // # Bytes between that
  memmove(front, middle, backSize);  // will do the check for us 

  memcpy((char*)end - frontSize, buffer, frontSize);
}
```

<br>
<br>
<br>

### Generic QuickSort

```C++
void quickSort(void* base, int size, elemSize, int (*cmpFun)(void*, void*));
```

> man qsort

> man bsearch

> man memcpy

> man memmove

> man malloc

> realloc

> free


### Stack and Heap

#### RAM:
|$2^{32}-1$|--|
|---|--|

|...|--|
|---|--|

|Stack Segement (begin)|--|
|---|--|

|...|--|
|---|--|

|Stack Segement (end)|--|
|---|--|

|Heap Segement (end)|--|
|---|--|

|...|--|
|---|--|

|Heap Segement (start)|--|
|---|--|

|-|--|
|---|--|

|-|--|
|-|--|

|0|--|
|-|--|

**This Heap boundary**, and that address, and that address is admitted to software, which implements what we call **the heap manager**.

And **the heap manager** is software – it’s code. The implementation for `malloc`, `realloc`, and `free` are all written in the same file, and they basically manage this heap memory.

#### Heap can also be that:
|Heap Segement (start)|--|--|--|--|--|--|--|--|--|--|--|--|--|Heap Segement (end)|
|---|--|--|--|--|--|--|--|--|--|--|--|--|--|--|

`void* a = malloc(40);`

This isn’t exactly what happens, but it’s pretty close.  
It will usually **search from the beginning of the heap**, and look for the first free block of memory that’s able to accommodate this size request.

|Heap Segement (start)|40 bytes allocated|--|--|--|--|--|--|--|--|--|--|--|--|Heap Segement (end)|
|---|--|--|--|--|--|--|--|--|--|--|--|--|--|--|

This pointer `Heap Segement (start)` will be returned.

<br>
<br>

`void* b = malloc(60);`

|Heap Segement (start)|40 bytes allocated|(`ptr`) 60 bytes allocated|--|--|--|--|--|--|--|--|--|--|--|Heap Segement (end)|
|---|--|--|--|--|--|--|--|--|--|--|--|--|--|--|

This pointer `ptr1` will be returned.

<br>
<br>

`free(a);`

|Heap Segement (start)|40 bytes not allocated|(`ptr`) 60 bytes allocated|--|--|--|--|--|--|--|--|--|--|--|Heap Segement (end)|
|---|--|--|--|--|--|--|--|--|--|--|--|--|--|--|

It will go and remove the mark in the first 40 Bytes – it doesn’t clear out the bit patterns because the bit patterns are supposed to no matter – and then donates it back. 

<br>
<br>

`void* c = malloc(44);`

|Heap Segement (start)|40 bytes not allocated|(`ptr1`) 60 bytes allocated|(`ptr2`) 44 bytes allocated|--|--|--|--|--|--|--|--|--|--|Heap Segement (end)|
|---|--|--|--|--|--|--|--|--|--|--|--|--|--|--|

This pointer `ptr2` will be returned.


<br>
<br>

`void* d = malloc(20);`

(but depends on real implementation)

|Heap Segement (start)|20 bytes allocated| 20 bytes not allocated|(`ptr1`) 60 bytes allocated|(`ptr2`) 44 bytes allocated|--|--|--|--|--|--|--|--|--|--|Heap Segement (end)|
|---|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|

This pointer `Heap Segement (start)` will be returned.

Entirely software managed with very little exception, and I say exception because **the operating system and what’s called the loader has to admit to the implementation what the boundaries of the stacks of the heap segment are** – but everything else is really frame in terms of this raw memory allocator.




<br>
<br>
<br>
<br>
<br>

|Heap Segement (start)|(4 or * byte) size_of_node  ptr1|-| 60 bytes allocated (`ptr1`)|-|--|--|--|--|--|--|--|--|--|--|Heap Segement (end)|
|---|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|


There is **a data structure** that overlays the entire heap segment, okay. It is manually managed using lots of `void*` business.

The data structure used by the heap manager **overlays a linked list of what are called free notes**.
And it always keeps the address of the very first free note, and because you’re not using this as a client.   
The heap manager uses it as **a variably sized node**

In the first 4 or 8 bytes – keeps track of how big that node is. 
So, it might have subdivided that, and to the left of that line might have the size of that node, the right of that line might actually have a pointer to that right there.

Now it’s not like there’s ints and doubles typing any of the material over here. The heap manager has to do this very generically so it constantly is casting addresses to be `void*`s and `void**`s, okay, in order to actually jump through what this thing called the free list
behind the scenes to figure out which node best accommodates the next malloc request. When you free this node right here it has to be threaded back into the free list.
