## Debuging

I want to go back a little bit and talk about debugging and in particular, give you some lighthearted examples as to **why programs crash the way they do.** It’s one thing to say they crash, that’s not very interesting and insightful. It’s like why did it crash and what happened at run-time to actually prompt the crash or the **segmentation fault** or the
bus error. 

### Segmentation faults and Bus error

- Segmentation faults:  
  `*(NULL)` wouldn’t compile, because you can’t dereference a `void*`  
  **But conceptually, if you were to actually try and jump to the `NULL` address to dereference, it issues a segmentation fault**. 
  
  It’s almost always because you have some `NULL` pointer. In theory it can be any address out of all segments, but it’s gonna be either dereferencing a `NULL` or a four or eight or some very small pointer relative address.
  
  RAM = Stack + Heap + Code Segment + Data Segment + somewhere not in Segement

  The reason that’s the case is because `NULL` is not part of any segment. The operating system can tell that. OS is not mapping the zero address `NULL` to stack or heap or code segment or data segment. It’s not the address of a local variable; it wasn’t an address that for `malloc`, so why are you dereferencing it? I’m gonna scream at you in the form of a **Segmentation faults**. It’s your fault for not mapping to a segment.
  
  
- Bus errors:  
  are actually generated **when you dereference an address that really is somewhere in one of the four segments, but it thinks it can tell that the address couldn’t possibly correspond to what you think it is.**
  
  ```C
  void* vp = an arbitrary address;
  *(short*)vp = 7;
  *(int*)vp = 55;
  ```
  If `vp` really is an address that corresponds to one of the four segments, you will not get a segmentation fault because you are hitting a segment.
   
  Because it wants to make things simpler, compilers adhere to a restriction that’s imposed by the hardware and the operation system that **all `int`s actually begin at addresses that are multiple of four**. **All `short`s begin at even addresses**, but there’s n**o restriction on `char`**, but basically just to keep things clean and to kind of optimize the hardware, it always assumes that **all figures other than shorts and bytes reside at addresses that are a multiple of four**. I 
  
  Example: You wanted to dereference some pointer in there as a `short*` and if **it happened to be an odd address**, even though the two bytes that are there really do pack in a `short`, the hardware will like to issue bus error "I don’t like that. I don’t like you dereferencing odd addresses and thinking that there are shorts there because I know that the compiler would never normally put a short at an odd based address.:
  
  
  `*(short*)vp = 7;` will throw a bus error with 50% probability. 
  
  If I were to do `*(int*)vp = 55;`, then that would throw a bus error if VP was really part of some segment somewhere, **but VP wasn’t a multiple of four**. At address 2002, it wouldn’t put an `int` there. 
  

<br>
<br>
<br>

### Using memory to form a infinite loop.

#### Example 1

```C
int main(int argc, char* agrv[])
{
  int i;
  int array[4];
  for (i = 0; i <= 4; ++i) {
    array[i] = 0;
  }
  return 0;
}
```

[Code](Notes/lec13_2.c)

|Stack Segement (begin)|--|
|---|--|

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i|
|-|---|

|add + 16|array[3]|
|-|---|

|add + 20|array[2]|
|-|---|

|add + 24|array[1]|
|-|---|

|add + 28|array[0]|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|
 
Here is **the saved PC** of whoever called `main`. It’s actually a function called start that calls `main` and starts responsibility is to pass the command line to build the `argv` array of strings, count the
number of strings and put that number in `argc`. `argv` actually puts a `NULL` after the very last string, so in case you want to ignore `argc` you can.

|Stack Segement (begin)|--|
|---|--|

<br>

`i = 0`:  

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 0|
|-|---|

|add + 16|array[3]|
|-|---|

|add + 20|array[2]|
|-|---|

|add + 24|array[1]|
|-|---|

|add + 28|array[0] = 0|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

`i = 1`:  

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 1|
|-|---|

|add + 16|array[3]|
|-|---|

|add + 20|array[2]|
|-|---|

|add + 24|array[1] = 0|
|-|---|

|add + 28|array[0] = 0|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

`i = 2`:  

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 2|
|-|---|

|add + 16|array[3]|
|-|---|

|add + 20|array[2] = 0|
|-|---|

|add + 24|array[1] = 0|
|-|---|

|add + 28|array[0] = 0|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

`i = 3`:  

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 3|
|-|---|

|add + 16|array[3] = 0|
|-|---|

|add + 20|array[2] = 0|
|-|---|

|add + 24|array[1] = 0|
|-|---|

|add + 28|array[0] = 0|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

`i = 4`:  

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 4|
|-|---|

|add + 16|array[3] = 0|
|-|---|

|add + 20|array[2] = 0|
|-|---|

|add + 24|array[1] = 0|
|-|---|

|add + 28|array[0] = 0|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

`i = 0`:  

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 0 = array[4]|
|-|---|

|add + 16|array[3] = 0|
|-|---|

|add + 20|array[2] = 0|
|-|---|

|add + 24|array[1] = 0|
|-|---|

|add + 28|array[0] = 0|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

Forever loop

EDIT from Jigao:
For me it can compile using a `gcc version 7.3.0 (Ubuntu 7.3.0-27ubuntu1~18.04)`.
It terminates itself in 4 loops. So I tried to google such thing and found:

[A Post from Stackoverflow](https://stackoverflow.com/questions/32185385/why-does-not-my-program-go-into-infinite-loop-when-array-out-of-bounds-occur-in)

> When there is **undefined behavior** you can't expect a single or particular behavior. Anything could happen.   
> What said in that video is just one of many possibilities of undefined behavior and what you are getting is another possibility. One should not rely upon any particular behavior.

> It is undefined behaviour which, amoung other, **depends on the compiler**. **When you get an infinite loop, it's probably because the memory location for `i` is located just after the array and when you write `0` beyond the array, you actually modify `i` to zero.** To be sure you must look at the assembly code generated by the compiler

[generated Assembly](lec13_2.s)
```asm
	.file	"lec13_2.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movl	%edi, -52(%rbp)
	movq	%rsi, -64(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -36(%rbp)
	jmp	.L2
.L3:
	movl	-36(%rbp), %eax
	cltq
	movl	$0, -32(%rbp,%rax,4)
	addl	$1, -36(%rbp)
.L2:
	cmpl	$4, -36(%rbp)
	jle	.L3
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L5
	call	__stack_chk_fail@PLT
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits

```

#### Example 2: depending on big/little endian

```C
int main(int argc, char* agrv[])
{
  int i;
  short array[4];
  for (i = 0; i <= 4; ++i) {
    array[i] = 0;
  }
  return 0;
}
```

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 0 = array[4]|
|-|---|

|add + 16|array[3] array[2]|
|-|---|

|add + 20|array[1] array[0]|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

`i = 4`:

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 4|
|-|---|

|add + 16|array[3] = 0  array[2] = 0|
|-|---|

|add + 20|array[1] = 0 array[0] = 0|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

in Big endian:

|add + 12|0x 00|
|-|---|

|add + 13|0x 00|
|-|---|

|add + 13|0x 00|
|-|---|

|add + 14|0x 04|
|-|---|

Loop should ends.

<br>

in Little endian:

|add + 12|0x 04|
|-|---|

|add + 13|0x 00|
|-|---|

|add + 13|0x 00|
|-|---|

|add + 14|0x 00|
|-|---|

to 

|add + 12|0x 00|
|-|---|

|add + 13|0x 00|
|-|---|

|add + 13|0x 00|
|-|---|

|add + 14|0x 00|
|-|---| 

Loop can not ends like the `int array`.

#### Example 3: from midterm

```C
int foo()
{
  int array[4];
  int i;
  for (i = 0; i <= 4; ++i) {
    array[i] -= 4;
  }
  return 0;
}
```

I don’t care that the array hasn’t been initialized. 

<br>

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|array[3]|
|-|---|

|add + 16|array[2]|
|-|---|

|add + 20|array[1]|
|-|---|

|add + 24|array[0]|
|-|---|

|add + 28|i|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

to

`i = 4`:
High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|array[3] = -4|
|-|---|

|add + 16|array[2] = -4|
|-|---|

|add + 20|array[1] = -4|
|-|---|

|add + 24|array[0] = -4|
|-|---|

|add + 28|i = 4|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

to

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC - 4|
|-|---|

|add + 12|array[3] = -4|
|-|---|

|add + 16|array[2] = -4|
|-|---|

|add + 20|array[1] = -4|
|-|---|

|add + 24|array[0] = -4|
|-|---|

|add + 28|i = 4|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|


What values of `array[4]` were they before? We have no idea. But it will certainly take whatever values happen to reside there and decrease them by four. Unfortunately, because the test is wrong, it goes up and it
decrease this `|add + 8|saved PC - 4|` numerically by four as well. So `PC` not pointing to the next instruction any more, but pointing to the same `CALL <foo>` instruction. Then `CALL <foo>` again and again and again.

That is probably the fifth example of infinite recursion we’ve seen in the last
two days. 

<br>
<br>
<br>

#### Example 4: No infinite recursion

```C
int main(int argc, char* agrv[])
{
  DeclearAndInitArray();
  PrintArray();
  return 0;
}

// declare this array, locally update it to be a counting array and then leave.
void DeclearAndInitArray() {
  int array[100];
  int i;
  for (int i = 0; i < 100; i++) {
	  array[i] = i;
  }
}

void PrintArray() {
  ini array[100];
  int i;
  for (int i = 0; i < 100; i++) {
	  printf("%d\n", array[i]);
  }
}
```

And the answer is, it is working as far as they’re concerned, **because that manages to print out
0 through 99, inclusive.**

This `DeclearAndInitArray()` right here, built-in activation record of 404 bytes, goes down and it lays down the counting array in the top 100 of the 101 slots. Returns.

`PrintArray()` calls a function that has exactly the same image for its activation record. So
it goes down and embraces the same 404 bytes. And it happens to print over the footprint of this function `DeclearAndInitArray()`. It’s not like when this thing returned it cleared out all of those bits and said we got to scramble this so it doesn’t look like a counting array. It doesn’t take the time to do that. So basically, what happens is if this is the top of the activation record and this is the bottom of the activation record, SP is descendant once, this is filled up with happy information, comes back up
after the first call returns, comes back to the same exact point, the happy face is still
there. We print over it and it happens to be revisiting the same exact memory with the
same exact activation record, so it prints everything out exactly the same. 

So you almost think of this as this abusive, perverse way of dealing with a global variable. And
you’re setting up parameters for code that needs to run later on. This example wouldn’t
exactly work that way. This isn’t a great example of that, but at least it highlights the
feature. This thing called “channeling,” that’s exactly what this thing is when really advanced
C++ programmers take advantage of this type of knowledge as to how memory is laid
out. 



<br>
<br>
<br>

## `printf`

`printf` takes anything between one and, in theory, an infinite number of arguments. What kind of prototype exists in the language to accommodate that kind of thing? Well, we always need the control string.


```C
int printf(const char* control, ...)
```
So the first argument to this thing is either a `char*` or a `const char *` that can polish
supports const. But then, there’s no data type that really has to be set in stone. There doesn’t even have to be a second argument, much less a data type attached to it. 

And the complier is actually quite promiscuous, and what it will accept is arguments two, three, and four, provided that dot `...` is there. The complier is **not obligated to do any kind of type checking** between `control` and what this evaluates to (the real values represented by `...`).

`GCC`, for quite some time, has an extension to the C that is implementing. Where it does **try to do type checking** if you mismatch, it’s doing a little bit more work at compile time than it’s really obligated
to do. It wants to make sure that this `printf` call works out. So if you were to put `%s` and `4` for value, most compliers would just let you do it and run and it would just lead to really bad things. But `GCC` will, in fact, flag it and say, you didn’t mean to do that. 


The return type `int` is the number of placeholders that are successfully bound to. It’s very unusual for `printf` to fail. `scanf`, the read in equivalent of `printf`, can fail more often.
But if, for whatever reason something goes badly, this would return `-1`. 

Do you know how `ifstreams` set the fail bit to true so that when you call the `.fail` method
inside `C++`, it’ll basically evaluate to true and that’s the way you break out of a file
reading program? Well, you’re relying on the equivalent of `printf`’s return value, which is
called `scanf` or `fscanf` to return to `-1` when it’s at the end of the file. 


The reason I’m bringing this up is because, based on what we know and the way we’ve
adopted a memory model, I now can defend **why we push parameters on the stack from
right to left**, why the 0-th parameter is always at the bottom and the 1-th parameter is
always above that. 

<br>
<br>
<br>

```C
printf("%d + %d = %d\n", 4, 4, 8);
```

Let’s just consider that call right there. The prototype during compilation just says that call is legitimate, but when it actually complies that `printf` there, it really does go and count parameters and figures out exactly how many bytes to decrement the stack pointer by for that particular call. 

High Add.

|add|8|
|-|---|

|add + 4|4|
|-|---|

|add + 8|4|
|-|---|

|add + 12|another add|
|-|---|

|add + 16|saved PC |
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

|another add|"%d + %d = %d\n"|
|-|---|

If reads a `%d` right at the front, it says, oh, the four bytes above the control string must be an `int`. And then it sees another percent D along the way. 

