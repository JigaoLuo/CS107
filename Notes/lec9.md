
### Assembly

#### Register
We’re gonna actually assume that all processors have **32 of registers**, where a register is just **a general purpose 4 byte figure** that we happen to have **really fast access** to. 

|A|Reg32| 4 B|----- connected|
|---|--|--|--|

|.|Reg31| 4 B|----- with|
|---|--|--|--|

|L|Reg...| 4 B|----- R|
|---|--|--|--|

|.|Reg2|4 B|------ A|
|---|--|--|--|

|U|Reg1|4 B|----- M|
|---|--|--|--|

**The registers are electronically in touch with all of RAM**.
Every single register can technically, draw and flush information to and from to and from RAM. 

The 16 or the 32 registers **are also electronically in touch with the electronics** called the **“arithmetic logic unit” or the ALU**, that is the piece of electronics that’s responsible for emulating what we understand to be addition and multiplication, and left shifting and right shifting of bits, and masking it. 

It is electronically in touch only with the register set **(MIPS)**. I’m going with a processor architecture where the ALU is just in touch with these things right here, and **it’s not directly in touch
with general RAM**. The implications of that is that all meaningful mathematical operations have to actually be done using registers. 

The alternative is for you to get this right here to electronically
in touch with all of RAM and that would be either prohibitively expensive, or it would be prohibitively slow. 


<br>
<br>

#### Exmaple1

```C
int i;
int j;

i = 10;
j = i + 7;
j++;
```

|Stack Segement (begin)|--|
|---|--|

|&i|10|
|---|--|

|&j|18 pointed by `Reg1`|
|---|--|

|Stack Segement (end)|--|
|---|--|


Let’s just assume, not surprisingly, that `i` and `j` are packed together as some 8-byte activation record and they reside somewhere in the stack
segment. 

<br>

|`Reg1`|&j|
|---|--|

I’m going to assume, is **stored in one of those 32 registers** `Reg1`. So `Reg1` is a general purpose 4-byte bit pattern store-er that actually happens to **store the base address of the activation record that makes up this code snippet.** 

<br>
<br>
<br>

```C
// compiled assembly 
M[Reg1 + 4] = 10;  // store operation

Reg2 = M[Reg1 + 4];  // load operation
Reg3 = Reg2 + 7;  // ALU operation
M[Reg1] = Reg3; // store operation

Reg2 = M[Reg1];
Reg2 = Reg2 + 1;
M[Reg1] = Reg2;
```
That capital M, it’s like it’s the name of all of RAM. 

<br>
<br>
<br>

#### Exmaple2 - take lower 2 Bytes

```C
int i;
short s1;
short s2;

i = 200;
s1 = i;
s2 = s1 + 1;
```

|Stack Segement (begin)|--|
|---|--|

|&i|200|
|---|--|

|&s2 pointed by `Reg1`|s2 (2B) s1 (2B)|
|---|--|

|Stack Segement (end)|--|
|---|--|

<br>

|`Reg1`|&s2|
|---|--|

<br>

```C
// compiled assembly 
M[Reg1 + 4] = 200;  // store operation

Reg2 = M[Reg1 + 4];  // load operation
// M[Reg1 + 2] = Reg2; wrong: write a 4 Byte into M[Reg1 + 2]
M[Reg1 + 2] = .2 Reg2; // take lower 2 Byte

Reg2 = .2M[Reg1 + 2];
Reg3 = Reg2 + 1;
M[Reg + 2] = .2 Reg3;
```

<br>
<br>

#### Exmaple3 - `for` loop

```C
int array[4];
int i;
for (i = 0; i < 4; i++){
  array[i] = 0;
}
i--;
```

|Stack Segement (begin)|--|
|---|--|

|array|array[3]|
|---|--|

|array + 4|array[2]|
|---|--|

|array + 8|array[1]|
|---|--|

|array + 12|array[0]|
|---|--|

|&i|-|
|---|--|

|Stack Segement (end)|--|
|---|--|

<br>

|`Reg1`|&i|
|---|--|

We assume that R1, as a register, stores the base address.

<br>

```C
// compiled assembly 
M[Reg] = 0;  // i = 0;

Reg2 = M[Reg1];  // i
BGE Reg2, 4, PC + 4 * 10// Branch operation: B, GE: greater than or equal
// Jump to Reg2 = M[Reg1]; -- end of the loop
Reg3 = M[Reg1];  // i
Reg4 = Reg3 * 4; // sizeof(int) = 4
Reg5 = Reg1 + 4; // offset
Reg6 = Reg4 + Reg5;  // baseAdd  + offset
M[Reg6] = 0;

Reg2 = M[Reg1];
Reg2 = Reg2 + 1;
M[Reg1] = Reg2;  // i++ in loop

JMP PC - 4 * 10;  //  jump to Reg2 = M[Reg1]; -- next loop

// i--
Reg2 = M[Reg1];  // i
Reg2 = Reg2 - 1;
M[Reg1] = Reg2;  // i--
```

`PC` stands for **program counter**, and **it stores the address of the currently  executing instruction**. 

<br>
<br>

#### Exmaple4 - `struct` and cast

```C++
struct fraction {
  int num;
  int denum;
};

fraction pi;
pi.num = 22;
pi.denum = 7;
((fraction*) &pi.demon)->demon = 451;
```

|Stack Segement (begin)|--|
|---|--|

High Add.
|pi.denum + 4|451|
|-|---|

|pi.denum|7|
|-|---|

|pi.num|22| <- &pi is pointing here|
|---|---|---|
Low Add., **base pointer of pi is pointing here**

|Stack Segement (end)|--|
|---|--|

<br>

|`Reg1`|&pi, pi.num|
|---|--|

We assume that R1, as a register, stores the base address.

<br>

```C
// compiled assembly 
M[Reg1] = 22;

M[Reg1 + 4] = 7;

M[Reg1 + 4 + 4] = 451;
```

`PC` stands for **program counter**, and **it stores the address of the currently  executing instruction**. 

<br>
<br>
<br>
<br>

#### Instruction Encoding - 4B

Let me explain a little bit how 32 bits are usually just enough for you to encode an instruction, just to understand what encoding is like.
When a 4-byte figure is understood to be an assembly code instruction, it’s typically subdivided into little packets or little sub-packets. 

Let’s say that I’ve decided that my assembly code language has 59
different types of instructions. I have to somehow encode in this four bytes right here, which of the 59 instructions we’re actually dealing with. Well, 59, unfortunately, isn’t a perfect power of two, so if I really want to be able to distinguish between 59 patterns, I have to be able to distinguish between, it turns out, 64 different patterns.



**The first six bits (an operation code, or an op code):**  
be the part that the hardware looks at to figure out what type of instructions should be executing.

**Five bits:**  
to encode which register gets the update

**Five bits:** 
which determines the base address and the memory offset. 

**The remaining 16 bits:**  
could be used to express a signed offset from this right here.