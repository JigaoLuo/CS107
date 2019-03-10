
### Stack and Heap

#### RAM $2^{32}$:
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

|Code Segement (end)|--|
|---|--|

|...|--|
|---|--|

|Code Segement (start)|--|
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

### THE FACT

|Heap Segement (start)|(4 or 8 byte) size_of_node  ptr1|-| 60 bytes allocated (`ptr1`)|-|--|--|--|--|--|--|--|--|--|--|Heap Segement (end)|
|---|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|


There is **a data structure** that overlays the entire heap segment, okay. It is manually managed using lots of `void*` business.

The data structure used by the heap manager **overlays a linked list of what are called free notes**.
And it always keeps the address of the very first free note, and because you’re not using this as a client.   
The heap manager uses it as **a variably sized node**

In the first 4 or 8 bytes – keeps track of how big that node is. 
So, it might have subdivided that, and to the left of that line might have the size of that node, the right of that line might actually have a pointer to that right there.

Now it’s not like there’s ints and doubles typing any of the material over here. The heap manager has to do this very generically so it constantly is casting addresses to be `void*`s and `void**`s, okay, in order to actually jump through what this thing called the free list
behind the scenes to figure out which node best accommodates the next malloc request. When you free this node right here it has to be threaded back into the free list.

<br>
<br>

#### Exmaple for THE FACT

`int* arr = malloc(40 * sizeof(int));` allocates more than 160 Bytes.

|Heap Segement (start)|-|4 Byte or 8 Byte Header for the coming 160 Bytes|(`arr`)160 Bytes allocated|-|-|-|--|--|--|--|--|--|--|--|--|--|Heap Segement (end)|
|---|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|

`free(arr);`:
`free` says, “Oh, I’m just assuming that this a pointer that I handed back earlier. If I really **handed this back**, then I know that I put down, as **a little footprint, how big this node was**. So I’m gonna cast this pointer to be an `int*` . So that I can gracefully **back up 4 or 8 bytes**, interpret those 4 or 8 bytes, in a way that I know I laid down information before.” And say, “Oh look, there’s a number 164 (for example).”  
That means, these 164 Bytes should somehow be threaded back into the heap. 

<br>
<br>

### Pay Attention:
1. Wrong Style: 
```C++
int* arr = malloc(100 * sizeof(int));
// ... 
free(arr + 60);  // DO NOT TRY THIS
```

We got allocated assumed 100 * 4 + 8 = 408Bytes(404 or 408 Byte) and then, you realize that you don’t need all the space.  
Maybe you **only need to use 60 of the integers** and you’ve recorded that in some other variable.

Well, if this is the 400 byte figure that you’ve logically gotten, and you’ve been handed that address and there’s a **pre-node header (assumed 8 Bytes)** of meaningful information to `malloc` and `realloc`, and you go ahead and hand back that address to `free`, **depending on the implementation it may be very naïve and just assume, without error-checking, that it was something that was handed back via `malloc` or `realloc` before.** 

It might do some internal checking, but `malloc`, `free`, `realloc` are supposed to **be implemented as quickly as possible** and not do the error checking for you because they’re assuming that you’re really good at this C and C+ + programming thing, So they’re not gonna interfere with
execution by doing all of this error checking every single time `free` and `malloc` get called.

So if you were to do this, right here, and it doesn’t do any integrity checks on the number that it gets, **it will still blindly back up 8 bytes**, whatever happens to reside in the two `int` at index 58 and 59, would be interpreted as one of these things right here. 

And if it happens to store at index 59 where 1000 is stored as `int`.
This 1000 will be interpreted as the allocated length, because 8 Byte at index 58 and 59 interpreted as the node header. `free` will go from this point forward 1000 bytes and just blindly follow whatever algorithm it does to integrate a 1000 byte block back into the heap.

总结来说 `free(arr + 60);` 会胡乱`free`

<br>
<br>

2. Wrong Style: 

`int array[100];` statically allocate your array, and you **don’t involve
the heap at all**

But if try to `free(array);`:  
if `free` doesn’t do any error-checking on the address, it’s not
even obligated to do any error-checking to confirm that it’s in the heap segment in the first place, it would go to the base address of your static array, back up 4 or 8 bytes, whatever figure and bit pattern happens to reside there would be interpreted as the size of some node, That should be incorporated into **the free list data structure**(the collection of free nodes that the heap can consider for future – in response to future call to `malloc` and `realloc`). 

<br>
<br>
<br>
<br>

### Idea to implementations
The fastest implementation:  
Don’t do error checking. 

Some implementations:  
do actually basically keep track of a set of `void*` that have
been handed back. And it’ll **do a very quick check** of the `void*`, that it gets passed to it, against – and make sure it’s a member of the set of `void*` that have been handed out.

And if it’s **in debug mode**, it might give an error if it’s not present. If it’s **not in debug mode**, it may ignore it and say, “I’m not going to free this thing because it will only cause problems later on.” 


<br>
<br>
<br>
<br>

### But 160 is not a perfect power of 2 (Implementations depending)
160 is not exactly a power of 2. 

Implementations that I’ve seen, if this is the entire heap, I’ve seen this as a heuristic. When `malloc(6)`, it will quickly recognize whether or not the number that’s supplied is less than, $2^3$ or $2^4$, basically categorize and throw it in a size bucket as to whether it’s small, medium or large. 

I’ve seen some implementations actually divide the heap up, so that anything less than or equal to, $2^3$, is allocated in the first part. Anything less than or equal to $2^6$, might be allocated from middle segment. 

|Heap Segement (end)|--|
|---|--|

|...|--|
|---|--|


|64 Byte Part|64B|64B|64B|64B|64B|64B|64B|...|
|---|--|--|--|--|--|--|--|--|

|8 Byte Part|8B|8B|8B|8B|8B|8B|8B|...|
|---|--|--|--|--|--|--|--|--|

|Heap Segement (start)|--|
|---|--|

`malloc(160);` if it goes ahead and allocates 192 bytes, or 256 bytes, you actually don’t mind all that much **because at least you’re given certainly given enough memory to meet the 160 request**.


But it is still a implementation depending problem. **So you certainly can’t assume you have so many memory like 256.** You’re just supposed to assume that **you’ve got the 160 bytes** and that was it. 

<br> 

#### Consequence:  

- **Why running over the boundaries of an array can cause problems? Sometimes, it doesn’t.**

- `array[-1] = ...;`. It happens to overlay the space (in Node Header) where malloc and realloc actually put down information about how big the node is. This 4 bytes where `malloc` is really relying on meaningful information to be preserved. And it will completely toy with the
implementation of `malloc` and `realloc` in its ability to do the job properly.

<br> 
<br> 
<br> 

### Linked List

|Heap Segement (start)|Head1| 20 bytes allocated|(`e_ptr_1` to next free block) 20 bytes not allocated|Head2|(`ptr1`) 60 bytes allocated|(`e_ptr_2` to next free block) `e_ptr_1`200 bytes allocated|Head3|(`ptr2`) 44 bytes allocated|`e_ptr_2`--|--|--|--|--|--|--|--|--|--|Heap Segement (end)|
|---|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|

As far as all these empty nodes (not allocated) are concerned, it
wants to be able to **quickly scan the heap for unused blocks** whenever `malloc` and `realloc` are called. 

What will typically happen is that it will interpret these nodes right here as **variably sized nodes**. It’ll **overlay a link list of blobs** over these allocated block. 

It can from **a circular link list approach**循环链表. Every single
time you call `malloc` or `free`, it obviously wants to** traverse this link list** and come up with some node that’s big enough to meet the request to find a block big enough the size to be allocated.


 
## Stack Segement
```C++
void A()
{
  int a;
  short b[4];
  double c;
  
  B();
  C();
}

void B()
{
  int x;
  char* y;
  char* z[2];

  C();
}

void C()
{
  double m[3];
  int n;
}
```

In Stack:

After `A()` called:

|??|??|
|---|--|

|&a|a (4B) <- `Stack Frame Pointer`|
|---|--|

|b = a + 4|b[0] (2B)|b[1] (2B)|
|---|--|--|

|b + 4 = a + 8|b[2] (2B)|b[3] (2B)|
|---|--|--|

|&c = a + 12|c (8B = 2 lines compared with a) <- `Stack Pointer`|
|---|--|

All of the memory that’s needed for these variable **are packed as aggressively as possible and there’s even some ordering scheme that’s in place**. 

<br>
<br>

After `B()` called:

|??|??|
|---|--|

|&a|a (4B)|
|---|--|

|b = a + 4|b[0] (2B)|b[1] (2B)|
|---|--|--|

|b + 4 = a + 8|b[2] (2B)|b[3] (2B)|
|---|--|--|

|&c = a + 12|c (8B = 2 lines compared with a)|
|---|--|

|&x = a + 20|x (4B)  <- `Stack Frame Pointer`|
|---|--|

|&y = a + 24|y (4B)|
|---|--|

|&z = a + 28|z[0] (4B)|
|---|--|

|&z + 4 = a + 32|z[1] (4B)  <- `Stack Pointer`|
|---|--|

<br>
<br>

After `C()` called:

|??|??|
|---|--|

|&a|a (4B)|
|---|--|

|b = a + 4|b[0] (2B)|b[1] (2B)|
|---|--|--|

|b + 4 = a + 8|b[2] (2B)|b[3] (2B)|
|---|--|--|

|&c = a + 12|c (8B = 2 lines compared with a)|
|---|--|

|&x = a + 20|x (4B)|
|---|--|

|&y = a + 24|y (4B)|
|---|--|

|&z = a + 28|z[0] (4B)|
|---|--|

|&z + 4 = a + 32|z[1] (4B)|
|---|--|

|&m = a + 36|m[0] (8B = 2 lines compared with 4Byte)   <- `Stack Frame Pointer`|
|---|--|

|&m + 8 = a + 44|m[1] (8B = 2 lines compared with 4Byte)|
|---|--|

|&m + 16 = a + 50|m[2] (8B = 2 lines compared with 4Byte)|
|---|--|

|&n = a + 58|x (4B)   <- `Stack Pointer`|
|---|--|

<br>
<br>

After `C()` exits:

|??|??|
|---|--|

|&a|a (4B)|
|---|--|

|b = a + 4|b[0] (2B)|b[1] (2B)|
|---|--|--|

|b + 4 = a + 8|b[2] (2B)|b[3] (2B)|
|---|--|--|

|&c = a + 12|c (8B = 2 lines compared with a)|
|---|--|

|&x = a + 20|x (4B)    <- `Stack Frame Pointer`|
|---|--|

|&y = a + 24|y (4B)|
|---|--|

|&z = a + 28|z[0] (4B)|
|---|--|

|&z + 4 = a + 32|z[1] (4B)    <- `Stack Pointer`|
|---|--|

|&m = a + 36|m[0] (8B = 2 lines compared with 4Byte)|
|---|--|

|&m + 8 = a + 44|m[1] (8B = 2 lines compared with 4Byte)|
|---|--|

|&m + 16 = a + 50|m[2] (8B = 2 lines compared with 4Byte)|
|---|--|

|&n = a + 58|x (4B)|
|---|--|

It simply raises the **stack pointer** back to where it was before `C()` was called. Whatever information has been in the `C()`, **it actually stays there**. But it’s supposed to be out of view and useless; it can’t be accessed but can be overwritten. 