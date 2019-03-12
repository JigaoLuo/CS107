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
int main(int argc, char* agrv[])
{
  int array[4];
  int i;
  for (i = 0; i <= 4; ++i) {
    array[i] -= 4;
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

Then `CALL <foo>` again and again and again.