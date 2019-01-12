**Syllabus** 
- C,
- Assembly, 
- C++, 
- Concurrent programming – that’s not a language, that’s just a paradigm,
- Scheme 
- Python

The real intellectual value in learning all of these languages is to really study the paradigms that they represent. Programming in C or C++ is a diff. process when programming with scheme and Python.

**Concurrent programming**: Think about you have a Wells Fargo
checking account. Just imagine two people are using ATM machines and you have a $100 in it and you share your PIN with your best friend and you go up to
a neighboring ATM machines and you make as much progress as possible to withdraw
that $100 and then you both press okay to try and get $200 collected. 

That is not a sense sensible example because both of those machines are basically  computers, need to access the same master account balance to confirm that $100 is available and in this **transactional** – **transactional** in terms of concurrent programming – you have to make sure that the $100, being identified as the
account balance, is maintained with **some atomic flavor** so that if you have two people trying to withdraw $100 at the same time that only one person gets away with it. That $100 account balance is the shared resource that two different processes have access to.


Scheme: LISP, which it’s certainly related to. This is a representative of what is called the **functional paradigm**. When you program using the functional
paradigm you always rely on the **return value of a function to move forward**. **You program without side effects**. 

But when you code in C and C++, it’s very often all about side effects. The return value doesn’t always tell you very much. It’s a number or it’s a boolean, but when you pass in a data structure by reference to a function and you update it so that when the function returns, the original data structure has been changed. That’s programming by side effects.

The idea with Scheme and particularly the functional paradigm is that you don’t program with side effects. You don’t have that at all. You always synthesize the results or partial results that become larger partial results that eventually become the result that you’re interested in and only then are you allowed to print it to the screen to see what the answer to the problem was.

Python: You’ve probably heard of a language called Perl. It’s not a very pretty language. You can just think about, in some sense, Python being a more modern, object oriented version of Perl. It actually has a lot of good libraries
for dealing with web programming. You dynamically generate web pages
based on content and databases and things like that and Python, being a scripting
language, which means its interpretive and you can type in stuff as you go, it recognizes and reads and executes the stuff as you type, it’s very good for that type of thing

---
Handout

## CS107 Course Outline
### Rough Outline of What To Expect:

Lower-level C constructs
- Data types and representation, binary math galore大量的，很多.
- Pointers, references. dynamic memory allocation, the heap and its implementation, C-style strings.
- Computer architecture, the stack frame, function/method call and return, the compilation and linking processes.
- Why C fails at generic programming, polymorphism, and runtime type
identification.
- What C++ learned from C’s mistakes.
  
High-level C++ Constructs
- Memory management, constructors, destructors, assignment operators.
- Templates, template specializations. custom STL-like iterators.
- Operator overloading, function objects.
  
Concurrent Programming
- How multiple executables appear to run simultaneously.
- How multiple functions appear to run simultaneously within the same executable.
- Synchronization issues that arise when concurrently running functions access shared resources.
- How to overcome these synchronization issues using locks and semaphores.

Better Type Safety and Type Identification in Scheme
- Mechanics of Scheme.
- Programming without side effects
- Functions as data, lambda expressions, and Scheme's ability to build functions (and programs!) on the fly.
- Run-time environments and its implementation, Scheme memory management.
  
Web Programming with Python
- Scripting languages, what they’re good for, and what they’re not good for.
- Basic syntax, lists, dictionaries, functions, classes.
- Imperative, object-oriented, and functional language features.
- Client-server paradigm, basics of web programming, HTTP protocol, cookies, sessions, database access.
- Python library support for processing HTML and XML.