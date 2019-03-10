# CS107_Stanford

Here would be some lecture notes and assignments coming. I would try my best to take the note seriously with detail. If that is combined with some chinese characters, don't be bothered. If you also would like to attend this lecture, I hope my note could be helpful.

这里是我给CS107留下的电子版笔记， 我会努力做的比较可读并且保留足够的内容性。如果你也上这门课，希望我能帮到你。

---
[Lecture_1: Syllabus](Notes/lec1.md)
- Lower-level C constructs
- High-level C++ Constructs
- Concurrent Programming
- Better Type Safety and Type Identification in Scheme
- Web Programming with Python

[Lecture_2: The low-level memory mechanics](Notes/lec2.md)
- Data Types
- Binary Representation
- Big / Little Endian
- Pointer

[Lecture_3: Struct and Array in Memory](Notes/lec3.md)
- Struct
- Array
- Structs with Arrays
- Pointer Arithmetic
- Heap
- `strcpy`
  
[Lecture_4: Generics in C](Notes/lec4.md)
- `memcpy`
- `void swap(int* ap, int* bp)`
- `void swap(void* vp1, void* vp2, int size)`
- `strdup`
- `int linearSearch(int key, int array[], int size)`
- `void* linearSearch(void* key, void* base, int n, int elemSize)`
- `void* linearSearch(void* key, void* base, int n, int elemSize, int (*cmpFunc)(void*, void*))`
- `int IntCmp(void* elem1, void* elem2)`
- `void* binarySearch(void* key, void* base, int n, int elemSize, int (*cmpFunc)(void*, void*))`
- `int StrCmp(void* vp1, void* vp2)`

[Lecture_5 + 6: Int Stack & Generics Stack](Notes/lec5&6.md)
- `Stack.h` for `int` 
- `Stack.c` for `int`
- `Stack.h` for `void*` 
- `Stack.c` for `void*`
- Upgraded Stack with `freeFun`

[Lecture_7: rotate, quickSort](Notes/lec7.md)
- `rotate`
- `qsort`

[Lecture_8: malloc, realloc, free](Notes/lec8.md)
- RAM = heap + stack +...
- Heap
- `malloc`

[Lecture_9: Assembly](Notes/lec9.md)
- Assembly (MIPS like)

[Lecture_10: Assembly Call Convention](Notes/lec10.md)
- Assembly Call Convention
- Parameter in Stack
- `CALL`, `RET`
- `SP`
- Recursion. [See in PDF, page 350](../CS107.pdf)

