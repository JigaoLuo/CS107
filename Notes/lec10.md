### Call Convention

```C++
int main(int argc, char** argv)
{
  int i = 4;
  foo(i, &i);
  return 0;
}
```

Before `int i = 4;`:  
|Stack Segement (begin)|--|
|---|--|

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC <- pointed by `SP`: Stack Pointer|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>
<br>

After `int i = 4;`: 
|Stack Segement (begin)|--|
|---|--|

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 4 <- pointed by `SP`: Stack Pointer|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

```
SP = SP - 4;
M[SP] = 4; // int i = 4;
```

<br>
<br>

`foo(i, &i);`: 
|Stack Segement (begin)|--|
|---|--|

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 4 <- `main` until here|
|-|---|

|add + 16|add + 12 = &i|
|-|---|

|add + 20|4|
|-|---|

|add + 24|saved PC <- pointed by `SP`: Stack Pointer|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

```
SP = SP - 8;
Reg1 = M[SP + 8]; 
Reg2 = SP + 8;
M[SP] = Reg1;  // i as the first parameter
M[SP + 4] = Reg2;  // &i as the first parameter

CALL <foo>  // JUMP to and JUMP back later
SP = SP + 8 // after back: clean up para.
```

<br>
<br>
<br>

```C++
void foo(int bar, int* baz)  // para laid in Stack from right to left
{
  char snink[4];
  short* why;

  why = (short*)(snink + 2);
  *why = 50; 
}
```

Before **clean up**:
|Stack Segement (begin)|--|
|---|--|

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 4 <- `main` until here|
|-|---|

|add + 16|add + 12 = &i|
|-|---|

|add + 20|4|
|-|---|

|add + 24|saved PC|
|-|---|

|add + 28 = snink|snink[3] snink[2] snink[1] snink[0]|
|---|---|---|

|add + 32|why = snink[2] <- pointed by `SP`: Stack Pointer|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

```
foo:

SP = SP - 8; // for local varaible: snink and why

Reg1 = SP + 6;  // (snink + 2)
M[SP] = Reg1  // why = (short*)(snink + 2);

Reg1 = M[SP];  // *why
M[Reg1] = .2 50;  // *why = 50; 

SP = SP + 8  // clean up

RET
```

<br>

Before **clean up**:
|Stack Segement (begin)|--|
|---|--|

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 4 <- `main` until here|
|-|---|

|add + 16|add + 12 = &i|
|-|---|

|add + 20|4|
|-|---|

|add + 24|saved PC <- pointed by `SP`: Stack Pointer|
|-|---|

|add + 28 = snink|snink[3] snink[2] snink[1] snink[0]: but cleaned up|
|---|---|---|

|add + 32|why = snink[2]: but cleaned up  |
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|


`RET` for return, is understood to be an assembly code instruction that pulls `saved PC` out, places it – basically populates the true PC register with what this is, brings the SP register up four bytes and then carries on as if it were executing at PC + 4 all along. 




`RV` which is this 4B register dedicated to communicating **return values** between caller and callee functions.


<br>
<br>
<br>

### General Activation Record

|param.s|allocated and initialized by caller|
|-|---|

|Saved PC|
|---|---|---|

|local|allocated and initialized by callee|
|-|---|

<br>
<br>
<br>

### Recursion  - factorial 

```C
int fact(int n)
{
  if(n == 0) 
    return 1;
  return n * fact(n - 1);
}
```

```C
R1 = M[SP + 4];  // n
BNE R1, 0, PC + 12;  // branch, not equal | 12 = 3 * 4 
RV = 1;
RET;

R1 = M[SP + 4] ; // n
R1 = R1 - 1;  // n - 1 | frac(n) until here  
SP = SP - 4;  // one para.
M[SP] = R1; // para = n - 1
CALL <fact>;

SP = SP + 4;1 = M[SP + 4] ;
RV = RV * R1;
RET;
```

[A very great example: See in PDF, page 350](../CS107.pdf)