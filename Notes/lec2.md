## The low-level memory mechanics
To understand how data – as simple as Booleans and integers and floating-point numbers and strucks and classes – are all represented in memory. How everything ultimately gets represented as a collection of zeros and ones. And how it's faithfully interpreted every single time to be that capital A, or that number seven, or Pi, or some struck that represents a fraction, things like that. 

The basic data types in C/C++:
- bool
- char:   1 byte
- short:  2 byts
- int:    4 bytes assumed in this lecture
- long:   4 bytes
- float:  4 bytes
- double: 8 bytes assumed in this lecture

`char`: represents one of 256 characters => 8 bit usually.
`short`, `int` and `long`: are certainly related and are intended to represent scalar numbers.
`float` and `double`: try to represent arbitrarily precise numbers
`bool`: a Boolean could just be mapped to a single bit in memory. It turns out it's not practical to do that.

---
`bool`

8 bits:
|0/1|0/1|0/1|0/1|0/1|0/1|0/1|0/1|
|---|---|---|---|---|---|---|---|
$2^8=256$

A in ACSII $(65)_{10} = (64 + 1)_{10}=2^6+2^0$
| 0 | 1 | 0 | 0 | 0 | 0 | 0 | 1 |
|---|---|---|---|---|---|---|---|

---
`short`
16 bits = 2 bytes
|0/1|0/1|0/1|0/1|0/1|0/1|0/1|0/1||0/1|0/1|0/1|0/1|0/1|0/1|0/1|0/1|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

<br>

$2^{15}$
|1|0|0|0|0|0|0|0||0|0|0|0|0|0|0|0|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

$2^{15} - 1$
|0|1|1|1|1|1|1|1||1|1|1|1|1|1|1|1|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

<br>
But if the left-most bit have nothing to do with magnitude but indicates the **sign** of this `short`.

$7$
|0|0|0|0|0|0|0|0||0|0|0|0|0|1|1|1|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

$-7$
|1|0|0|0|0|0|0|0||0|0|0|0|0|1|1|1|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|


<br>

### ones' complement 
it's not used because it not following the addition to get 0.

||0|0|0|0|0|0|0|0||0|0|0|0|0|1|1|1|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

|+|1|0|0|0|0|0|0|0||0|0|0|0|0|1|1|1|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

||1|0|0|0|0|0|0|0||0|0|0|0|1|0|0|0|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
But not $0$, when we try to $7 + (-7)$

<br>

### Two's Complement
||0|0|0|0|0|0|0|0||0|0|0|0|1|1|1|1|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

|+|1|1|1|1|1|1|1|1||1|1|1|1|0|0|0|0|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

||1|1|1|1|1|1|1|1||1|1|1|1|1|1|1|1|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

|+|0|0|0|0|0|0|0|0||0|0|0|0|0|0|0|1|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

||0|0|0|0|0|0|0|0||0|0|0|0|0|0|0|0|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

I do get a one at the end, but it doesn't count because here's no room for it. It overflows the two bytes. 

||0|0|0|0|0|0|0|0||0|0|0|0|1|1|1|1|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

|+|1|1|1|1|1|1|1|1||1|1|1|1|0|0|0|1|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|


||0|0|0|0|0|0|0|0||0|0|0|0|0|0|0|0|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

$15 + (-15) = 0$

`char`:it's one of 256 values. We can fill in the Ascii table and have some unique integer be interpreted as a character every single time. As long as it's constant and it always maps to the same exact pattern, then it's **a value mapping**. 

`short`: I would not use all 16 bits to represent magnitude, because I want there to be just as many negative numbers represented as positive numbers.
So I dedicate all of the bits from that line to the right to magnitude and use  the left-most bit to basically communicate whether the number is negative or not. 

$2^{16}$ different patterns to a two-byte figure. Rather than having it represent zero through $2^{16} - 1$, I actually have it represent $-2^{15}$ through $2^{15}-1$.

```C++
char ch = 'A';
short s = ch;
cout << s << endl;  // 65
```
You don't need a cast for that. What you're really doing is you're just setting
`s` equal to whatever number is backing in `ch`.

```
ch   ........ 01000001
s    00000000 01000001  // padded with 0 at the first byte
```

We puts a 65 down there. And all the extra space is just padded, in this case, with **zeros**. So that's how 65 goes from a one-byte representation to a two-byte representation. 

<br> <br>

```C++
short s = 67;
char ch = s;
cout << ch << endl;  // c
```

```
s    00000000 01000011 
ch   ........ 01000011
```
So when `ch` gets laid down in memory, and it's assigned to `s`, two bytes of information, and 16 bits cannot somehow be wedged economically into an eight-bit pattern. So what C and C++ do is they simply punt on the stuff they don't have room for. And they assume that if you're assigning a large number to a smaller one, and the smaller one can only accommodate a certain range of values in the
first place, that your interested in the minutia of the smaller bits. 

<br>

```C
short s =  2 ^ 10 + 2 ^ 3 + 2 ^ 0;  // pesudo code 
int i = s;
```

bit pattern representation:
```
s    ........ ........ 00000100 00001001 
i    00000000 00000000 00000100 00001001 
```

```C
short s = -1;
int i = s;
```

bit pattern representation:
```
s    ........ ........ 11111111 11111111 
i    11111111 11111111 11111111 11111111 
```
**Sign Extend 符号扩展**:
takeing whatever sign bit is in the original figure and replicates that all the way through.

---
Function Mapping for bit representation:  
For 4 Bytes Int: $2^{32}$ different patterns available to map to whatever subset of a full integer range. 
The easiest thing to do is to just go from $-2^{31}$ through $2^{32}-1$
There's zero in the middle. That's why it breaks it symmetrically a little bit. 

---
---
---
**Floats:**  
|Sign bit +/-|exp|exp|exp|exp|exp|exp|exp|exp|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x|.x| 
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
1. Bit: Sign bit
2. 8 bits: magnitude as an unsigned integer
3. 23 bits: contributions of two to the negative one, and two to the negative two, and two to the negative three.   
   
$(-1)^s * (1.x) * 2^{exp-127}$  

---
**What happens when assigning an integer to a float, or a float to an integer:**  

```C++
int i = 5;
float f = i; // 5.0   = 1.25 * 2 ^ 1
std::cout << f << std::endl;  // 5
```

bit pattern representation:
```
s    00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000101 
i    very, very different than the representation of five using this protocol 
```
Every time you assign an int to a float, or a float to an int, it actually has to evaluate what number the original bit pattern corresponds to.  And then it has to invent a new bit pattern that can lay down in a float variable. 

```C++
int i = 37;
float f = *(float*)&i;  // not 37!!!, very small number
```
It doesn't evaluate i at all. 
All it does is **it evaluates the location(pointer) of i**.
So when the 37,

`&i`(ampersand of i) represents that arrow. 
Since i is of type int, `&i`(ampersand of i) is of type `int *`, raw exposed address of a variable. That's four bytes that happens to be storing  an int. 

Then we seduce引诱 it, momentarily, into thinking that it's a `float *`.
Now, this doesn't cause bits to move around. That would be i reacting to an operation against the address of i.
**Bit not changing**


```C++
float f = 7.0;
short s = *(short*)&f; // the figures are different sizes
```

```
f    xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx yyyyyyyy yyyyyyyy yyyyyyyy yyyyyyyy 
s    xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx only the first two Bytes de-referenced
```
**Only the first two Bytes of the float will be de-referenced to a short.**