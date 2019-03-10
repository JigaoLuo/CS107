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

> man realloc

> man free