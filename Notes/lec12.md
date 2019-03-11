### How compilation and works

It normally invokes the preprocessor, which is dedicated to dealing
with #define and #includes. Then it invokes what is truly called the compiler. That’s what’s responsible for taking your `.c` files, and your `.cc` files, and generating all those `.o` files, that happen to appear in your directory, after you type make. 


They all make compilation look like it’s the full effort to **create an executable**. But once all those `.o` files are created, and one of the `.o` files has code for the main function inside, there is
this step called ** **, which is responsible for taking a list of `.o` files; stacking them on top of one another; making sure that any function call that could possibly ever happen during execution, is accounted for because there’s code for that function one of the `.o` files.


---


### Preprocessor

#### `#define`

```C
#define kWidth 480
#define kHeight 720

printf("Width is %d\n", kWidth);
int area = kWidth * kHeight;
```


When you feed, or when a `.c` file is fed to GCC or G++, there’s a component called the **preprocessor** that doesn’t do very much in terms of compilation. It doesn’t do compilation. It actually reads the `.c` file that’s fed to it, from top to bottom, left to right, like
we would read a page. 

And every time it sees a `#define`, it really only pays attention to these words. What it’ll normally do is, it’ll read in a line. And if there’s nothing that has a # at the beginning of the line, it’ll just say, “Okay, I just read this one line, and I don’t care about it. 

It internally has something like a hash set. It associates this as a key with this string of text; it doesn’t even really know that it’s a number. It just says, “Everything between the white space that comes right after width, and the white space that comes after the first token, that’s associated as a text stream with that identifier. 

<br>

```C
printf("Width is %d\n", 480);
int area = 480 * 720;
```
It is really text, at the moment – it’s just that, when it’s fed to the next phase of compilation, it’ll go ahead, and it’ll chew on those things, and recognize that they really are numbers. And that’s when it might do the multiplication.

<br>
<br>

```
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
```

<br>
<br>

```
#define NthElemAddr(base, elemSize, index) ((char*)base+index * elemSize)
```

Now the thing about this is this looks like a function call. There is really no type checking done on these things right here, so this **only works post preprocessor time**. If this gets specified to be a pointer, and these are things that can be multiplied together and ultimately be treated as an integer offset.

```C
void* vertorNth (vector* v, int position)
{ 
  assert(position >= 0);
  assert(position < v->logicLen);
  return NthElemAddr(v->elems, v->elemSize, postion);
}
```

---

### `assert` is a marko

```C
#ifdef NDEBUG
 #define assert(cond) (void) 0  
else
 #define assert(cond) (cond) ? ((void) 0) : (fprintf(stderr, "string"), exit(0));
```

---

### `#include`

```C
#include <stdio.h>
#include <assert.h>
#include "vector.h"
```


The preprocessor folds over that line `#include "vector.h";` and says, “Let me go find this file. Oh, I found it.” It removes that line `#include "vector.h";` and it replaces it with the full contents of `vector.h` file. 

And so the stream text that it builds for you as part of preprocessing, the output of preprocessing, it’s what’s called **a translation unit** where all the pound defines and all the pound includes have been stripped out. It re-creates the text that’s actually fed to the compiler.


---

`main.c`
```C
#include <stdio.h>  // printf
#include <stdlib.h>  // malloc, free
#include <assert.h>

int main(int argc, char* argv[])
{
  void* memory = malloc(480);
  assert(memory != NULL);
  printf("Yay!\n");
  return 0;
}


```

`#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>`
It’s going to run it through the **preprocessor**. You know that these three things would **be recursively replaced** to whatever extent it’s needed to build one big stream of text without any `#include` and `#define `, which at the end has this `main function` here.


Here is the full `.o` file that’s generated as the compiler digests the expansion of this to a translation unit. 
```
...
CALL <malloc>
...
CALL <fprintf> ; for assert marko 
...
CALL <printf>
...
CALL <free>
RV = 0
RET
```

link to `a.out`: 

The only requirement that’s needed past compilation:
When it tries to create an executable, you’re technically in what’s called **the link phase where it tries to bundle all the `.o` files that are relevant to each other.** 