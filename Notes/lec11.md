
## `swap`

### C `swap` using Pointers
```C
void foo()
{
  int x;
  int y;

  x = 11;
  y = 17;
  swap(&x, &y);
}
```

Before **foo**:
|Stack Segment (begin)|--|
|---|--|

High Add.

|add|saved PC <--- pointed by SP|
|-|---|

Low Add.

|Stack Segment (end)|--|
|---|--|

<br>

```C
SP = SP - 8;

M[SP + 4] = 11;
M[SP] = 17;

Reg1 = SP;  // &y
Reg2 = SP + 4;  // &x
SP = SP - 8;
M[SP] = Reg2;
M[SP + 4] = Reg1;

CALL <swap> 
SP = SP + 8;

SP = SP + 8;
RET;
```

<br>

After `SP = SP - 8; M[SP + 4] = 11; M[SP] = 17;`:
|Stack Segment (begin)|--|
|---|--|

High Add.

|add|saved PC|
|-|---|

|add + 4|x = 11|
|-|---|

|add + 8|y = 17 <--- the last Byte pointed by SP|
|-|---|

Low Add.

|Stack Segment (end)|--|
|---|--|

<br>

Before `CALL <swap>`:
|Stack Segment (begin)|--|
|---|--|

High Add.

|add|saved PC|
|-|---|

|add + 4|x = 11|
|-|---|

|add + 8|y = 17|
|-|---|

|add + 12|add + 8 = &y|
|-|---|

|add + 16|add + 4 = &x <--- the last Byte pointed by SP|
|-|---|

Low Add.

|Stack Segment (end)|--|
|---|--|

<br>
<br>

```C++
void swap(int* ap, int* bp) 
{
  int temp = *ap;  // copied
  *ap = *bp;  // *ap: L-value, the space addressed by ap
  *bp = temp;
}
```

<br>

High Add.

|add|saved PC|
|-|---|

|add + 4|x = 11|
|-|---|

|add + 8|y = 17|
|-|---|

|add + 12|add + 8 = &y = bp|
|-|---|

|add + 16|add + 4 = &x = ap|
|-|---|

|add + 20|saved PC <--- the last Byte pointed by SP|
|-|---|

Low Add.

|Stack Segment (end)|--|
|---|--|

<br>

```C
<swap>:

SP = SP - 4; // for temp

Reg1 = M[SP + 8];  // &x or ap
Reg2 = M[Reg1];  // x or *ap
M[SP] = Reg2;  // temp = x;

Reg1 = M[SP + 12]; // &y or bp
Reg2 = M[Reg1];  // y or *bp

Reg3 = M[SP + 8];  // &x or ap
M[Reg3] = Reg2;  // x or *ap = y or *bp;

Reg1 = M[SP];  // temp = x
Reg2 = M[SP + 12];  // &y or bp
M[Reg2] = Reg1;  // y or *bp = temp = x

SP = SP + 4;
RET
```
<br>

After `SP = SP + 4;`:
High Add.

|add|saved PC|
|-|---|

|add + 4|x = 17|
|-|---|

|add + 8|y = 11|
|-|---|

|add + 12|add + 8 = &y = bp|
|-|---|

|add + 16|add + 4 = &x = ap|
|-|---|

|add + 20|saved PC  <--- the last Byte pointed by SP|
|-|---|

|add + 24|temp|
|-|---|

Low Add.

|Stack Segment (end)|--|
|---|--|


### C++ `swap` using References

```C
void foo()
{
  int x;
  int y;

  x = 11;
  y = 17;
  swap(x, y);
}
```

You’re not putting the * in front of those `x` and `y`, doesn’t mean that compiler is blocked from passing their addresses.

This `x` has to be an **L value**; this `y` has to be an **L value**, as well. It means it actually has to **name a space in memory**, that can be involved in an update or an exchange by reference. 

```C++
void swap(int& a, int& b) 
{
  int temp = a;  // copied
  a = b;
  b = temp;
}
```

Algorithmically, it looks similar, and it even sort of has the same side
effect. But you may question how things are actually working behind the scenes, **in order to swap things by reference**, as by opposed address. 

<br>

High Add.

|add| the thing that’s referred to as b|
|-|---|

|add + 4| the thing that’s referred to as a|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|temp|
|-|---|


Low Add.

|Stack Segment (end)|--|
|---|--|

Behind the scenes, what happens is that this **lays down two pointers**.
You don’t have to use the word pointer to understand **references**; you can just say it’s magic and somehow, it actually does **update the original values**. It lays down the address of these things. The assembly code that is generated for this function, it’s **passing around pointers wherever references are expected**. And so it **automatically does the dereferencing of the raw pointers that implement those references**. The assembly code for **this function is exactly the same as the C Code**, down to the instruction, down to the offsets; everything is exactly the same.  

Just because you don’t put * ampersands in there, doesn’t leave the compiler clueless because it knows, from the prototype, that it’s expecting references. It’s just this **second flavor of actually passing things around by address.**

It knows because of the way you declared these, that every time you refer to `a` and `b` in the code, that you’re really referring to whatever is accessible from the address that’s stored inside the space for `a`.  



```C
int x = 17;
int y = x;

int& z = y;  // (1)
int* z = &y; // (2)
```

`(1), (2)` from **an assembly code standpoint**, are precisely the same. At the C and C + + language level, you’re required to put the explicit asterisk in `(2)`.

The thing is, with **references**, you can’t rebind the references to new L values. It’s a technical way of saying you can’t reassociate z with x, as
opposed to y. Once it’s bound as a reference to another space, that’s in place forever. 

So you don’t have the flexibility to change where the reference points to. You do have that ability with raw pointers. So you could not very easily build a link list, if you just had references. 




### Class and Struct in C++
Turns out that structs and classes – just looking at this from an “under the hood” perspective – structs and classes are laid down in memory exactly the same way. 
In C++, structs can have constructors, destructors, and methods, as can classes. 
Classes aren’t required to have any methods. 

**The only difference between structs and classes**, in C++, is that the
**default access modifier for classes is private**, and **the default access modifier for structs is public**. 

So the compiler actually views them as more or less the same thing.

```C++
class binky {
  public:
    int dunky(int* x, int y);

    char* minky(int *z) {
      int w = *z;
      return slinky + dunky(winky, winky);
    }

  private:
    int winky;
    char* blinky;
    char slinky[8];
};

int n = 17;
binky b;
b.minky(&n);  // binky::minky(&b, &n);
```
`binky b;`: you really get **a blob of memory that packs all the memory needed for these three fields into one rectangle**. And it uses exactly the
same layout rules because of all the fields, `blinky` is declared before.

|add|slinky[7] slinky[6] slinky[5] slinky[4]|
|-|---|

|add + 4|slinky[3] slinky[2] slinky[1] slinky[0]|
|-|---|

|add + 8|blinky|
|-|---|

|add + 12|winky|
|-|---|

`b.minky(&n);  // binky::minky(&b, &n);`:  
The `this` pointer is always passed around as **the 0th argument**, where
everything is slided over to make space for the this pointer. It’s always **implicitly passed** on your behalf.

They just don’t bother being explicit about it in C++ because if they know that you’re using this type of – if you’re using that type of notation,
you’re really dealing with object orientation. What really happens, on your behalf, is that it basically calls the `minky` function, not with one argument, **but with two**. It actually **the address of something that has to be a `binky` class, or a `binky` struct**,** as the 0th argument.**

The reason that C++ compilers can just be augmented, at least the code admission port of it, can be augmented to really accommodate some parts
of C++ fairly easily, references and traditional method calls against an object.

<br>

**The activation record:**

|add|z = &n|
|-|---|

|add + 4|`this` pointer -> pointing to the last byte of the Obj == last byte of winky|
|-|---|

|add + 8|saved PC -> pointing to next LOC|
|-|---|

|add + 12|winky|
|-|---|


<br>
<br>
<br>

## Static


```C++
class fraction {
  public:
    fraction(int n, int d = 1);

    void reduce();  // 4/8 = 1/2

  private:
    static int gcd (int x, int y);
};
```
Low Add., **base pointer of pi is pointing here**
|pi.num|22| <- &pi is pointing here|
|---|---|---|

|pi.denum|7|
|-|---|
High Add.


`static int gcd (int x, int y);`: 
Greatest Common Divisor, it might only be relevant to your fraction class. And you actually pass in these two parameters and it really just behaves as a function because it doesn’t need the this pointer, in order to compute the greatest divisor that goes into `x` and `y`.

`static`. What that means in the context of a C++ class, is that you **don’t need an instance of the class to actually invoke it**. You can
actually just invoke it **as a stand-alone function**. In fact, it really is a regular function **that happens to be scoped inside the definition of a class** as if the class has a name space around it.

Static methods because they **don’t have this invisible `this` pointer** being passed around, they really are **internally recognized as regular functions**. 