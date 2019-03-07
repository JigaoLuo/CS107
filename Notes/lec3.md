## Struct and Array in Memory
### Struct
```C++
struct fraction {
  int num;
  int denum;
};

fraction pi;
pi.num = 22;
pi.denum = 7;
```
Low Add., **base pointer of pi is pointing here**
|pi.num|22| <- *pi is pointing here|
|---|---|---|

|pi.denum|7|
|-|---|
High Add.

<br>

```C++
(fraction*)&(pi.demon);
```
Low Add.
|pi.num|22|
|---|---|

|pi.denum|7| <- &(pi.demon) is pointing here|
|-|---|---|

|pi.denum + 4|unknown value|
|-|---|
High Add.

**pi.denum + (pi.denum + 4)**  addressing  eight-byte picture that overlays that space treated as a fraction.

<br>

```C++
(fraction*)&(pi.demon)->num = 12;
(fraction*)&(pi.demon)->denum = 33;
((fraction*)&(pi.demon))[0].denum;  // to get access to the last denum
```
Low Add.
|pi.num|22|
|---|---|

|pi.denum|12| <- &(pi.demon) is pointing here|
|-|---|---|

|pi.denum + 4|33|
|-|---|
High Add.


---
### Array
```C++
int array[10];
```
It allocates 40 bytes of memory for the ten units of a int array. 

**A little theorem**:  
$array \equiv \&array[0]$  
$array +k \equiv \&array[k]$  
$*array \equiv array[0]$  
$*(array +k) \equiv array[k]$  



**No bounce checking on raw array like Java:**
```C++
array[0] = 44;
array[9] = 100;
array[5] = 45;

// here attention
// can not guarantee it can succeeds 
array[10] = 1;  // 4 * 40 offset(displacement) on the base address array[0]
array[25] = 1;
array[-1] = 1;  // move to another direction
// same as *(array - 1) = -1;
```

Low Add.
|array[-1]|1|
|---|---|

|array[0]|44| <- &array[0] or array is pointing here|
|-|---|---|

|array[1]|-|
|-|---|

|array[2]|-|
|-|---|

|array[3]|-|
|-|---|

|array[4]|-|
|-|---|

|array[5]|45|
|-|---|

|array[6]|-|
|-|---|

|array[7]|-|
|-|---|

|array[8]|-|
|-|---|

|array[9]|-100| <- last allocated byte|
|-|---|---|

|array[10]|1|
|-|---|
High Add.

<br>
<br>
<br>

<br>

```C++
int arr[5];
arr[3] = 128;
```

Low Add.
|array[0]|44| <- &arr[0] or arr is pointing here|
|-|---|---|

|array[1]|-|
|-|---|

|array[2]|-|
|-|---|

|array[3]|128 = 00000000 00000000 00000000 10000000|
|-|---|

|array[4]|-|
|-|---|
High Add.

<br>

```C++
(short*)arr[6] = 2;
std::cout << arr[3] << std::endl; // 512 + 128 = 740
```
Low Add.
|array[0]|-| <- &arr[0] or arr is pointing here|
|-|---|---|

|array[1]|-|
|-|---|

|array[2]|-|
|-|---|

|array[3]|00000000 00000010 00000000 10000000|
|-|---|

|array[4]|-|
|-|---|
High Add.

Now, `arr` is brainwashed momentarily into thinking that it addresses a short. And there is space for ten shorts there.

<br>
<br>


```C++
(  (short*)( ((char*)(&arr[1]) ) + 8)  )[3] = 2;
std::cout << arr[3] << std::endl; // 512 + 128 = 740
```
Low Add.
|array[0]|-|
|-|---|

|array[1]|-| <- &arr[1] or (char*)(&arr[1]) pointing here|
|-|---|---|

|array[2]|-|
|-|---|

|array[3]|00000000 00000010 00000000 10000000| <- ((char*)(&arr[1]) ) + 8) or (  (short*)( ((char*)(&arr[1]) ) + 8)  ) pointing here|
|-|---|---|

|array[4]|00000000 00000000 00000000 00000010| <- (  (short*)( ((char*)(&arr[1]) ) + 8)  )[3] is the last 16 bits|
|-|---|---|
High Add.

---
--- 
### Structs with Arrays
```C++
struct student {
  char* name;
  char suid[8];  // unique id
  int numUnits;
};

student pupils[4];  // 64 Byte
pupils[0].numUnits = 21;
pupils[2].name = strdup("Adam");  // string duplicate. dynamically allocates enough space to store the string and returns the address of the first char
pupils[3].name = pupils[0].suid + 6;
```
**in Stack:**  
Low Add.
|pupils[0].name|-|
|-|---|

|pupils[0].suid|-|-|-|-|-|-|- (in add_2 ($6^{th}$ in suid))|-|
|-|---|---|---|---|---|---|---|---|

|pupils[0].numUnits|21|
|-|---|

|pupils[1].name|-|
|-|---|

|pupils[1].suid|-|-|-|-|-|-|-|-|
|-|---|---|---|---|---|---|---|---|

|pupils[1].numUnits|-|
|-|---|

|pupils[2].name|add_1 (pointing at Heap's add_1)|
|-|---|

|pupils[2].suid|-|-|-|-|-|-|-|-|
|-|---|---|---|---|---|---|---|---|

|pupils[2].numUnits|-|
|-|---|

|pupils[3].name|add_2 (pointing at Stack's add_2 in pupil[0].suid $6^{th}$)|
|-|---|

|pupils[3].suid|-|-|-|-|-|-|-|-|
|-|---|---|---|---|---|---|---|---|

|pupils[3].numUnits|-|
|-|---|
High Add.

**in Heap:**
|add_1|"Adam\0"|
|-|---|

<br>
<br>

```C++
strcpy(pupils[1].suid, "40411xx")
strcpy(pupils[3].name, "123456")
std::cout << pupils[3].name << std::endl;  // 123456
```
`pupils[3].name` is the address of something it trusts to be a `char` followed by  another `char` followed by yet another `char`. It just crawls over consecutive bytes of memory until it incidentally finds one `char` with `\0` in it.

**in Stack:**  
Low Add.
|pupils[0].name|-|
|-|---|

|pupils[0].suid|-|-|-|-|-|-|1 (in add_2 ($6^{th}$ in suid))|2|
|-|---|---|---|---|---|---|---|---|

|pupils[0].numUnits|3 4 5 6 $\equiv$ 0x03040506 (不是三千多, int: 4Byte vs Char: 1Byte)|
|-|---|

|pupils[1].name|\0|
|-|---|

|pupils[1].suid|4|0|4|1|1|x|x|\0|
|-|---|---|---|---|---|---|---|---|

|pupils[1].numUnits|-|
|-|---|

|pupils[2].name|add_1 (pointing at Heap's add_1)|
|-|---|

|pupils[2].suid|-|-|-|-|-|-|-|-|
|-|---|---|---|---|---|---|---|---|

|pupils[2].numUnits|-|
|-|---|

|pupils[3].name|add_2 (pointing at Stack's add_2 in pupil[0].suid $6^{th}$)|
|-|---|

|pupils[3].suid|-|-|-|-|-|-|-|-|
|-|---|---|---|---|---|---|---|---|

|pupils[3].numUnits|-|
|-|---|
High Add.

**in Heap:**
|add_1|"Adam\0"|
|-|---|



<br>
<br>

---

**Little about Heap:**  
**in Heap:**
|add|"Colleen\0"|
|-|---|

```C++
std::cout << add << std::endl;  // Colleen
std::cout << (add + 2) << std::endl;  // lleen
std::cout << (add + 5) << std::endl;  // en
std::cout << (add + 7) << std::endl;  // nothing printed
```