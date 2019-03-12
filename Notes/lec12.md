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
  free(memory);
  return 0;
}
```

`#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>`
It’s going to run it through the **preprocessor**. You know that these three things would **be recursively replaced** to whatever extent it’s needed to build one big stream of text without any `#include` and `#define `, which at the end has this `main function` here.


Here is the full `.o` file that’s generated as the compiler digests the expansion of this to a translation unit. 

preprocessor to `main.o`:
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


<br>
<br>
<br>
<br>

[Code Example, try to comment out the `#include` and compile it](Notes/lec12.c)

<br>

#### No `#include <stdio.h>`
`main.c`
```C
// #include <stdio.h>  // printf
#include <stdlib.h>  // malloc, free
#include <assert.h>

int main(int argc, char* argv[])
{
  void* memory = malloc(480);
  assert(memory != NULL);
  printf("Yay!\n");
  free(memory);
  return 0;
}
```

> gcc lec12.c

> lec12.c: In function ‘main’:  
lec12.c:9:3: warning: implicit declaration of function ‘printf’   [-Wimplicit-function-declaration]  
  printf("Yay!\n");  
   ^~~~~~  
lec12.c:9:3: warning: incompatible implicit declaration of built-in   function ‘printf’  
lec12.c:9:3: note: include ‘<stdio.h>’ or provide a declaration of ‘printf’

> ./a.out  
> Yay!

If you were to say that this would generate compiler errors because it doesn’t know about this `printf` function, you would **be right but only for some compilers**. As a result of comment that `#include <stdio.h>` out there, you know enough to know that **the translation unit** that’s built during **pre-processing won’t make any mention whatsoever of `printf`.** So when time comes to actually check to see whether or not `printf`  was being called correctly, **it doesn’t have the information** it normally has. Many compilers would be "what are you doing? I’ve never heard of this function `printf`, I don’t like calling functions that I don’t see prototypes for and some might issue an error". 

`GCC` des not. We’ll talk about `printf` in a little bit more, but if during compilation it sees what appear to be a function call, what it’ll do is `GCC` **will just infer the prototype based on the call**. It sees that a single string is being passed in here, compilation would **issue a warning** saying no prototype for `printf`  found, but **it doesn’t block you from compiling it still with generated `.o` file.** We are calling it correctly as it turns out. As long as you pass in at least one string, `printf` is able to do the job and if there’s no place holders in here, so it’s not gonna have any problems executing.

<br>

It does run beautifully, the question is compiler doesn’t understand
why it runs beautifully. All `.h` does is it tells the compiler what the prototype are. There’s no information in `.h` files about where the code lives.

**The link phase is responsible for finding the standard libraries**. That’s where `malloc` and `free` and `printf` are defined. It’s not as if there are hooks, that are planted in the `.o` file to get the standard library because of what the `#includes` used to be. It’ll still have these assembly and so when we **link against the standard library** then set, it’ll try and patch these against these three things again. The same symbols that exist in the `.o` files and it’ll create the executable files.


**By default, when `GCC` infers a prototype, it always infers a return type of `int`**. Turns out that is the real return value of `printf` , we normally ignore it, but the return value is always equal to the number of placeholders that are successfully matched to. This particular call would return zero, but I’m not concerned about the actual return value. I’m concerned about the return type; it happens to match up with what’s inferred by the `GCC` compiler.

It’s gonna compile here. What is the `.o` file going to look like? It’s
going to **look exactly the same**. The `.h` file just consolidates data structure definitions and prototypes. There’s no code emitted on its behalf; all it does is it kind of trains the compiler to know what’s syntactically okay and what is syntactically not okay.

Every time you try to build an executable using the `GCC` system, technically you’re using `GCC` but you’re using a tag called LD, which is just for **link**. It always **searches in the standard libraries whether or not there were warnings during compilation or not**. `printf` sits as a code block in **all of those standard libraries**, so it happens to be present during the link phase, even though we never saw the prototype for it. The presence of a `#include` has nothing to do and makes no guarantee about whether or not the implementation of any functions defined in that thing or available at link time. **If something is defined in the standard library set it’s always available, whether or not we behave with the prototypes.**

<br>
<br>
<br>

#### No `#include <stdlib.h>`
`main.c`
```C
#include <stdio.h>  // printf
// #include <stdlib.h>  // malloc, free
#include <assert.h>

int main(int argc, char* argv[])
{
  void* memory = malloc(480);
  assert(memory != NULL);
  printf("Yay!\n");
  free(memory);
  return 0;
}
```

> gcc lec12.c  
lec12.c: In function ‘main’:  
lec12.c:7:18: warning: implicit declaration of function ‘malloc’   [-Wimplicit-function-declaration]  
   void* memory = malloc(480);  
 .............................                 ^~~~~~  
lec12.c:7:18: warning: incompatible implicit declaration of built-in   function ‘malloc’  
lec12.c:7:18: note: include ‘<stdlib.h>’ or provide a declaration of   ‘malloc’  
lec12.c:10:3: warning: implicit declaration of function ‘free’   [-Wimplicit-function-declaration]  
   free(memory);  
   ^~~~  
lec12.c:10:3: warning: incompatible implicit declaration of built-in function ‘free’  
lec12.c:10:3: note: include ‘<stdlib.h>’ or provide a declaration of ‘free’

> ./a.out   
> Yay!

It doesn’t have official prototypes for `malloc` or `free`. preprocessor expands `#include <stdio.h>` to a bunch to a prototypes and data definitions, expands `#include <assert.h>` to at least the definition for assert. 

It’s fine with that, looks at this as well, I have no idea what `malloc` is. We’re calling this like **a function assuming and inferring its prototype to be something that takes an `int` and returns an `int`**. **Compiler does not actually look how it’s being used in the assignment statement to decide what the return type is**. 

So compiler will **issue two warnings** in response to `void* memory = malloc(480);`. It’s inferring the prototype and then you’re assigning a pointer to be
equal to a `void*`, what’s supposed to be an`int`. 

Compiler looks at `free(memory);` and it doesn’t like `free(memory);` either because it hasn’t a prototype. It infers one prototype and **issues a warning** saying it’s inferring a prototype for `free` assumeing it takes a `void*` returns an `int`, **which is not true**; **but we’re not looking for a return value, so it’s not a problem.**

So this would also generate the same exact `.o` file. It would issue three warnings: two for missing prototypes and one for incompatibility between L value and assigned value from `void* memory = malloc(480);`; but it would create this `.o` file and when we link it. Then there’s no record in here that some `.h` wasn’t around and that there were warnings. It’s just that
there’s a little bit of risk in what’s happening here, but I generated it because this as a `.o` file is consistent with the way you wrote the code here. So it goes on and links and when we run this, it runs beautifully.

<br>
<br>
<br>

#### No `#include <stdlib.h>` and `#include <stdlib.h>`
`main.c`
```C
// #include <stdio.h>  // printf
// #include <stdlib.h>  // malloc, free
#include <assert.h>

int main(int argc, char* argv[])
{
  void* memory = malloc(480);
  assert(memory != NULL);
  printf("Yay!\n");
  free(memory);
  return 0;
}
```

I get a total of four warnings, but it still generates the same `.o` and it generates the executable file that still runs.

<br>
<br>
<br>

#### No `#include <assert.h>`
`main.c`
```C
#include <stdio.h>  // printf
#include <stdlib.h>  // malloc, free
// #include <assert.h>

int main(int argc, char* argv[])
{
  void* memory = malloc(480);
  assert(memory != NULL);
  printf("Yay!\n");
  free(memory);
  return 0;
}
```

If I comment this `#include <assert.h>` out, we have a completely different scenario.
I don’t know what `assert` is, so look at it. You would say oh, that must
be a function that **takes a bool and returns a void**. But we know that it really isn’t that because we know how it’s officially defined in a `assert.h` but because this has been commented out, it looks at this and it’s not like the word replaced by marko `assert`. **Compiler actually assumes that this is a function call**. `CALL <assert>` would now appear in the `.o` file.

Then we compile fine and generates `.o`, the link phase would actually fail and the reason it would fail is because `assert` doesn’t exist anywhere in the standard library. 

`assert` was just this quick and dirty macro that was defined in `assert.h`

<br>
<br>
<br>

### The prototypes

The prototypes are really in place so that caller and callee have
complete agreement on how everything above the saved PC in the activation record is set up. 

The prototype only mentions parameters. The parameters are housed by the space in an activation record above the saved PC. Everything below the saved PC is the callee’s business, but when I call `printf` and I jump to `printf` code, we have to make sure that the callee and the caller are in agreement with how information was stacked at the top portion of an activation record, because it’s just this trust where if I’m calling `printf` and I lay down a string constant right here, and I say,
take over, `printf` has to be able to print, I just assumed that there’s a string above the saved PC.

I hope so, because there are problems, if it is not a string. 

<br>
<br>
<br>

#### Example `strlen`

```C
int main()
{
  int num = 65;
  int length = strlen((char*)&num, num);
  printf("length = %d\n", length);
  return 0;
}
```

I’m seeing a function call `strlen`, I haven’t seen a prototype for that, but because of the way I’m calling it, I’m going to assume it takes a `char*`, followed by
an `int`, and I’m gonna assume it returns an `int`, because I always do that for prototypes that I make, that I infer information about. The assignment works fine. It prints out whatever length it happens to be bound to, and then returns. So this, if were to compile this, it would only issue one warning. 

**But `strlen` just takes normally one argument.**

|add|num = 65|
|--|--|

|add + 4|length|
|--|--|

|add + 8|65|
|--|--|

|add + 12|add = &num <- SP pointing| (char*))&num |
|--|--|-|

`SP = SP - 8`

When this generates a `.o` file, all it has inside that’s of interest to the linker is the call to `strlen`. You may think that during link phase that it’s gonna freak out because it’s gonna somehow know that `strlen` only takes one argument. **That is not true, there’s no information, there’s no direct information inside the `.o` files about how many parameters something is supposed to take.** You can look at the `.o` file and you can see `SP = SP - 8;` **but it might be one 16 by struct**. When it does the linking it just looks for the name, it doesn’t do any checking  on parameter type.

All that it looks for during the link phase is for something that responds to the name `strlen` and that’s exactly what happens. So when this executes and it jumps to `strlen` using the record before.

|add|num = 65|
|--|--|

|add + 4|length|
|--|--|

|add + 8|65|
|--|--|

|add + 12|add = &num|
|--|--|-|

|add + 16|saved PC <- SP pointing|
|--|--|-|

The real implementation of `strlen` was written and compiled to only have one `char*` parameter. So its assembly code only goes this high to `|add + 12|add = &num|`. So the 65 is more or less ignored because the real implementation of `strlen` can not reach so high.

So as it turns out, this will compile with one warning.

<br>

```C
int strlen(char* s, int len); 

int main()
{
  int num = 65;
  int length = strlen((char*)&num, num);
  printf("length = %d\n", length);
  return 0;
}
```

[Code Example, try to compile it](Notes/lec13.c)

If I want to turn off the warning like above. I can actually manually prototype it at the top.

When it runs, it calls `strlen`. It actually
treats `|add + 12|add = &num|` as a `char*`, it has no choice but to do that, we even coached it to think that it’s a `char*` for the call. 

> gcc lec13.c
lec13.c:1:5: warning: conflicting types for built-in function ‘strlen’ [-Wbuiltin-declaration-mismatch]  
 int strlen(char* s, int len);  
 .....    ^~~~~~  
lec13.c: In function ‘main’:  
lec13.c:7:3: warning: implicit declaration of function ‘printf’ [-Wimplicit-function-declaration]  
   printf("length = %d\n", length);  
   ^~~~~~  
lec13.c:7:3: warning: incompatible implicit declaration of built-in function ‘printf’  
lec13.c:7:3: note: include ‘<stdio.h>’ or provide a declaration of ‘printf’

> ./a.out  
> length = 1


<br>

**It would be 0 on big endian systems. It would actually print out 1 on little endian systems.**

- On big endian system:
  
|add|num = 65 = 0x 00 00 00 41|
|--|--|

|add + 4|length|
|--|--|

|add + 8|65|
|--|--|

|add + 12|add = &num|
|--|--|-|

|add + 16|saved PC <- SP pointing|
|--|--|-|

or in detail:

|add|0x 00|
|--|--|

|add + 1|0x 00|
|--|--|

|add + 2|0x 00|
|--|--|

|add + 3|0x 41|
|--|--|


`(char*)&num` points to `0x 00`.  
`strlen((char*)&num) = 0`

<br>

- On little endian system:
  
|add|num = 65 = 0x 41 00 00 00|
|--|--|

|add + 4|length|
|--|--|

|add + 8|65|
|--|--|

|add + 12|add = &num|
|--|--|-|

|add + 16|saved PC <- SP pointing|
|--|--|-|

or in detail:

|add|0x 41|
|--|--|

|add + 1|0x 00|
|--|--|

|add + 2|0x 00|
|--|--|

|add + 3|0x 00|
|--|--|


`(char*)&num` points to `0x 41`.  
`strlen((char*)&num) = 1`

I mainly prototyped as `int strlen(char* s, int len); `. Try not to do that, because it allows things like this to happen. It turns out it **doesn’t cause any run-time problems**, it’ll actually **execute properly and because this happens to point to either** 0x 41 00 00 00 or 0x 00 00 00 41, both of those can be interpreted as c-string. One happens to be
the empty string, the other one happens to be the capital A followed by a backslash zero. It will just have some response, even though if it’s a little weird.

<br>
<br>
<br>

#### Example `memcmp`

```C
int memcmp(void* v1);

{
  int n = 17;
  int m = memcmp(&n);
}
```

I just want to see what happens if we call a function with only one argument that really expects three in standard library `int memcpy(void* v1, void* v2, int size)`.

|add|n = 17|
|--|--|

|add + 4|m|
|--|--|

|add + 8|add = &n: as the `single parameter` for our `memcmp`|
|--|--|

|add + 12|saved PC <- SP pointing|
|--|--|-|

But when we transfer execution to the real `memcmp` that has a completely
different idea of what the parameter list looks like, it inherits that as saved PC and it would have **12 bytes** of information as parameter above saved PC. So it just accesses them. `|add + 8|add = &n|` overlays the space for `v1`. `|add + 4|m|` uninitialized value overlays the space that’s
used for `v2` and it happens to inherit `|add|n = 17|` 17 for the size and it executes according to those three parameter. 

It would compile, it would execute, it probably would crash. It’s probably the case because `|add + 4|m|` is uninitialized.

### Function overloading in `C` and `C++`

In C++ You can actually define a function with the same name as long as it has different parameter types. You can even have the same number of parameters as long as it can disambiguate between two different versions based on the signatures of the two calls. 

You can’t do function overloading in pure c.   
If I have `memcmp` as a function name, then I can only use it once.

What `C++` does is very clever. When it compiles it, it actually doesn’t tag the first instruction of a function with just the name of the function, it actually uses the name of the function and the data types in the order they’re presented on the argument list to assemble a more complicated function symbol. 

C: `CALL <memcmp>`  
C++: `CALL <memcmp_void_p>`

C++ would lead not to the linking problems. So C++ is a little safer in that regard as well.