### Generics in C
To  use advanced memory terminology to describe what happens. 
```C++
int a = 7; 
int b = 117;
```
|ap or *a| -> 7 is addressed by ap|
|-|---|

|bp or *b| -> 117 is addressed by bp|
|-|---|


```C++
swap(&a, &b);

void swap(int* ap, int* bp) 
{
  int temp = *ap;  // copied
  *ap = *bp;  // *ap: L-value, the space addressed by ap
  *bp = temp;
}
```
|ap or *a| -> 117|
|-|---|

|bp or *b| -> 7|
|-|---|

|temp| -> 7|
|-|---|

Make `a` and `b` **`float`**. 
The pictures can even stay the same in terms of the drawings –
in terms of the sizes. They're still four bytes, and as long as I exchange all these things.
If I pass in `double` or `char`, or `bool`, or `struct student`, the same rules apply.
It's bit pattern swapping or bit pattern roation.  

<br>
<br>

**But how to get generic version in Pure C:.**
to exchange two arbitrarily sized figures.  
to pass in an arbitrary address here, and I don’t want to constrain it to point to any one type.

```C++
void swap(void* vp1, void* vp2, int size) 
{
  // void temp = *vp1;  absolutely wrong
  // 1. void is not a Data Type
  // 2. void* can not be de-referenced, the maschine doesn't know how many bytes to go out and embrace as part of the identification process.
  char buffer[size];
  memcpy(buffer, vp1, size); 
  memcpy(vp1, vp2, size);
  memcpy(vp2, buffer, size);
}
```
Type `void *`:. not mean that it points to nothing; it just means that it points to **something that doesn’t have any type
information about it - so generall or generic**. 

|vp1| -> value1|
|-|---|

|vp2| -> value2|
|-|---|

This right here, that declaration of an array, is supported by our version of a compiler.
True anti-C that’s compatible with all compilers actually doesn’t allow you to put anything other than a constant inside. But the real implementation to probably **dynamically allocate a
block that is that number of bytes, move it, use it as a buffer even though you are copying to the heap** as opposed to the stack, and then you get to free it at the end. So most of the energy is invested in the dynamic allocation and de-allocation of a buffer or  space.


<br>

```C++
int a = 7; 
int b = 117;
swap(&a, &b, sizeof(int));
```
The one perk of this over templates is that just this code gets compiled and **the same assembly code** that corresponds to this right here gets used for calls for int, double, struct.... 

But **disadvantage**: the parameter `size` can have the different size as the data type (like `swap(&int_p1, &int_p2, sizeof(int) + 1);`). In the mean time the compile can not detect such error.  
So if we cast a pointer to `void*`, we are already telling the compiler, that I can accept risk and let me not compiler to handle that. Compiler does not know the type contact.

<br>

It depends, how we define what is a disaster:  
```C++
int i = 1; 
short s = 2;
swap(&i, &s, sizeof(short));
```

before swap:

|vp1 or &i| -> High Add. 00000000 00000000 00000000 00000001 Low Add.|
|-|---|

|vp2 or &s| -> High Add. 00000000 00000010 Low Add.|
|-|---|

after swap:

|vp1 or &i| -> High Add. 00000000 00000000 00000000 00000010 Low Add.|
|-|---|

|vp2 or &s| -> High Add. 00000000 00000010 Low Add.|
|-|---|

[Code Example](lec4.c)
```C++
#include <stdio.h>
#include <string.h> // memcpy
#include <stdint.h>

void swap(void* , void* , int );

void swap(void* vp1, void* vp2, int size) 
{
  char buffer[size];
  printf("During Swapping\nFirst void*: %p\nSecond void*: %p\n", vp1, vp2);
  memcpy(buffer, vp1, size);
  for (int i = 0; i < size; i++) printf("buffer[%d]: %d\n", i, buffer[i]);
  memcpy(vp1, vp2, size);
  memcpy(vp2, buffer, size);
}

int main()
{
  uint64_t i = 1; 
  uint32_t s = 2;
  printf("Before Swapping\nFirst uint64_t = %ld at %p\nSecond uint32_t = %hi at %p\n", i, (void*)&i, s, (void*)&s);
  swap(&i, &s, sizeof(uint32_t));
}
```

**Important:** always the last byte copied in the first time of `memcpy` (in Little Endian)
```
Output:
Before Swapping
First uint64_t = 1 at 0x7fff1f641240
Second uint32_t = 2 at 0x7fff1f64123c
During Swapping
First void*: 0x7fff1f641240
Second void*: 0x7fff1f64123c
buffer[0]: 1
buffer[1]: 0
buffer[2]: 0
buffer[3]: 0
```


<br>
<br>

**If templates in C++:**
When you deal with templates, it expands a compilation or a call
to swap of `int` or swap of `double` in a template setting, actually expands two independent versions of the same code and compiles them in the int specific domain or the `double`, `int` specific domain.

That’s not a tragedy悲剧 if you’re only calling swap twice **but if you call swap in a very large code base**, you **call swap fifty different ways** with fifty different data types you **get fifty different copies of the same code in your executable file.**

<br>
<br>

```C++
char* husband = strdup("Fred");
char* wife = strdup("Wilma");
```

|husband|add_1 -> pointing at the "Fred" in Heap|
|-|---|

|wife|add_2 -> pointing at the "Wilma" in Heap|
|-|---|

<br>

|vp1|&husband -> pointing at the husband|
|-|---|

|vp2|&wife -> pointing at the wife|
|-|---|

**in Heap:**

|add_1|"Fred\0"|
|-|---|

|add_2|"Wilma\0"|
|-|---|

```C++
swap(&husband, &wife, sizeof(char*));
```
When I wanted to exchange two `int`s, I passed `int *` to swap.   
If I want to exchange two `char *`s I have to pass in the address of `char *`s to swap.
Actuatlly `vp1` and `vp2` are `char**`.

|buffer|add_1|
|-|---|

|husband|add_2 -> pointing at the "Wilma" in Heap|
|-|---|

|wife|add_1 -> pointing at the "Fred" in Heap|
|-|---|

<br>

|vp1|&husband -> pointing at the husband|
|-|---|

|vp2|&wife -> pointing at the wife|
|-|---|

**in Heap:**

|add_1|"Fred\0"|
|-|---|

|add_2|"Wilma\0"|
|-|---|

<br>
<br>
<br>

```C++
swap(husband, wife, sizeof(char*));  // assuming sizeof(char*) == 4
```
Before `swap`:
|husband|add_1 -> pointing at the "Fred" in Heap|
|-|---|

|wife|add_2 -> pointing at the "Wilma" in Heap|
|-|---|

<br>

|vp1|add_1 -> pointing at the "Fred" in Heap|
|-|---|

|vp2|add_2 -> pointing at the "Wilma" in Heap|
|-|---|

**in Heap:**

|add_1|"Fred\0"|
|-|---|

|add_2|"Wilma\0"|
|-|---|
<br>

After `swap`:

|buffer|"Fred"|
|-|---|

|husband|add_1 -> pointing at the "Wilm" in Heap|
|-|---|

|wife|add_2 -> pointing at the "Freda" in Heap|
|-|---|

<br>

|vp1|add_1 -> pointing at the "Wilm" in Heap|
|-|---|

|vp2|add_2 -> pointing at the "Freda" in Heap|
|-|---|

**in Heap:**

|add_1|"Wilm\0"|
|-|---|

|add_2|"Freda\0"|
|-|---|

Without the `&` it will still compile and it will still execute. It will not crash. I promise you.
It will exchange the first 4 Byte / `char` of the strings.

<br>
<br>
<br>

```C++
swap(husband, &wife, sizeof(char*));  // assuming sizeof(char*) == 4
```
One is `char*`, another is `char**`. "Fred" 4 Bytes will be exchanged with the address `&wife` 4 Bytes.

Before `swap`:

|husband|add_1 -> pointing at the "Fred" in Heap|
|-|---|

|wife|add_2 -> pointing at the "Wilma" in Heap|
|-|---|

<br>

|vp1|add_1 -> pointing at the "Fred" in Heap|
|-|---|

|vp2|&wife -> pointing at the wife|
|-|---|

**in Heap:**

|add_1|"Fred\0"|
|-|---|

|add_2|"Wilma\0"|
|-|---|
<br>

After `swap`:

|buffer|"Fred"|
|-|---|

|husband|add_2 -> pointing at the "Wilma" in Heap|
|-|---|

|wife|"Fred"|
|-|---|

<br>

|vp1|add_1 -> pointing at the "Fred" in Heap|
|-|---|

|vp2|&wife -> pointing at the wife|
|-|---|

**in Heap:**

|add_1|"Wilm\0"|
|-|---|

|add_2|"Freda\0"|
|-|---|

`char * wife = "Fred"` $\equiv$ $0x46 72 65 64$ is going to normally be interpreted as an **address**. Okay? So that means that whenever 4-byte figure that corresponds to, if you pass `wife`, it's going to jump to the `Fred` address in memory, which you certainly do not own. If it doesn't crash because it's not inside the stack or the heap, which probably will be the case. But if it doesn't crash, it's just going to print out random characters that happen to reside at `Fred`, interpreted as an address. 


<br>
<br>
<br>

---
### Generics Linear search 

A linear search from front to back of the array for the first instance of key in that array, and I want it to return the index of it or -1 if it can't be found.
```C++
int linearSearch(int key, int array[], int size)
{
  for (int i = 0; i < size; ++i) {
    if (array[i] == key) {
      // array[i] same as array + i * sizeof(int)
      return i;
    }
  }
  return -1;
}
```

So if I want to go generic and I make this a void *, then all of a sudden, I lose **the implicit point arithmetic** that comes with array notation, like ptr++. In fact, you **can't use array notation on `void *` for the same reasons you can't dereference it**.

```C++
void* linearSearch(void* key, void* base, int n, int elemSize)
{
  for (int i = 0; i < n; ++i) {
    void* elemAdd = (char*)base + i * elemSize; 
    // base + i * elemSize : wrong, compiler not know how large is the offset, not know the type of the base
    if (memcmp(key, elemAdd, elemSize) == 0) {
      return elemAdd;
    }
  }
  return NULL;
}
```
Just cast it to be a `char *` so that I can do normal math against a pointer.



`memcmp` of key address, elem address, elem size to compare memory patterns  
like string comparison but it’s not dealing with characters specifically. It compares this many bytes at that address to this many bytes at that address and if they are a dead match, it returns zero. It would otherwise return a positive number or a negative number depending on whether or not the first non-matching bytes differ in a negative direction or a positive direction.


<br>
<br>
<br>

```C++
void* linearSearch(void* key, void* base, int n, int elemSize, int (*cmpFunc)(void*, void*))
{
  for (int i = 0; i < n; ++i) {
    void* elemAdd = (char*)base + i * elemSize; 
    // base + i * elemSize : wrong, compiler not know how large is the offset, not know the type of the base
    if (cmpFunc(key, elemAdd, elemSize) == 0) {
      return elemAdd;
    }
  }
  return NULL;
}
```

That means of **the fifth parameter has to be a function that takes two `void *`s**.


```C++
int IntCmp(void* elem1, void* elem2){
  int* ip1 = elem1;
  int* ip2 = elem2;
  return *ip1 - *ip2;
}

int array[] = {4, 2, 3, 7, 11, 6};
int size = 6;
int searchNum = 7;

// sizeof(int) is compile time evaluated as 4
int* found = linearSearch(&searchNum, array, size, sizeof(int), IntCmp);
if (found == NULL) {
  // not found
} else {
  // found
}
```

|array|4|
|-|---|

|array + 4|2|
|-|---|

|array + 8|3|
|-|---|

|array + 12|7|
|-|---|

|array + 16|11|
|-|---|

|array + 20|6|
|-|---|

<br>

|&searchNum|7|
|-|---|



All the other languages are all so much younger that they've learned from C's mistakes and they have better solution for supporting **generics**. 

**Advantage:**  
It's very fast. You only use one copy of the code, ever, to do all of your linear searching. 

**The template approach:**  
more type safe. You get more information and compile time, but you get code bloat because you've got one instance of that linear search algorithm for every single data type you ever searched for. 

<br>
<br>

**Another Example:** 
```C++
int StrCmp(void* vp1, void* vp2)
{
  // because vp1, vp2 are char**

  // vp2 from notes as char** 
  // | afrer one time de-reference: char* as element in notes, which pointin at a char(string) 
  // | after two time de-reference: a char(string) 
  char* s1 = *(char**)vp1;
  char* s2 = *(char**)vp2;
  // s1, s2 are pointing at the first char of the string
  return strcmp(s1, s2);
}


char* notes[] = {"Ab", "F#", "B", "Gb", "D"};  // notes is pointer to pointer == array of pointer
char* favoriteNode = "Eb";
char** found = linearSearch(&favoriteNote, notes, 5, sizeof(char*), StrCmp);
// why &favoriteNote: notes is already a char**
// why sizeof(char*): a single char** de-referenced is a char*
```

|notes|add_0|
|-|---|

|notes + 4|add_1|
|-|---|

|notes + 8|add_2|
|-|---|

|notes + 12|add_3|
|-|---|

|notes + 16|add_4|
|-|---|

<br>

|favoriteNode|"Eb"|
|-|---|

<br>

|&favoriteNode|favoriteNode|
|-|---|

<br>

|add_0|"Ab"|
|-|---|

|add_1|"F#"|
|-|---|

|add_2|"B"|
|-|---|

|add_3|"Gb"|
|-|---|

|add_4|"D"|
|-|---|

How `notes` is stored:  
They’re **not in the Heap**, they’re actually **global variables that happen to be constant**. It’s like normal global variables, except they happen to be character arrays that reside up there, and these are replaced at load time with the base addresses of the A, F , Gb, D.

<br>
<br>

---
When I say function, I’m talking about this **object-oriented-less unit**, which is just some block of code that gets called as a function that has no object or class declaration aroun it.

When I’m talking about the type of number functions or functions that are inside classes, I don’t refer to them as functions, I refer to them as **methods**.  

**The difference between a function and a method**, they look very similar, except that methods actually have the address of the relevant object lying around as this invisible parameter via this invisible
parameter called this.


---

```C++
void* binarySearch(void* key, void* base, int n, int elemSize, int (*cmpFunc)(void*, void*))
{
  for (int i = 0; i < n; ++i) {
    void* elemAdd = (char*)base + i * elemSize; 
    // base + i * elemSize : wrong, compiler not know how large is the offset, not know the type of the base
    if (cmpFunc(key, elemAdd, elemSize) == 0) {
      return elemAdd;
    }
  }
  return NULL;
}
```

The type of function that gets passed right here has to be either a **global function** that has nothing to do with the class or it has to be **a method inside a class that’s declared as static**, which means that it does not have any this pointer passed around on your behalf behind
the scenes.

**C++ methods**, those **number functions that are defined in classes**, normally **pass around the address of the receiving object via an invisible parameter called `this`**. If you need to you can actually refer to the keyword this inside the implementation of any method, and it just evaluates to the address of the object that’s being manipulated.

That’s what makes a method different than a regular function. **Regular functions** have nothing to do with objects so there’s **no invisible `this` pointer being passed around**. You have to pass one of those object-oriented-less normal functions, or the name of one, as the fifth primary to bsearch.