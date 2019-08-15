
## Imperative (Procedural Oriented) Paradigm:
C

The reason this is called imperative or procedural is that they’re focused around the verbs that come up in the paragraph description of a solution. Think about what a main function
looks like in a typical C program. You declare all your data, and then you make a series of call to these very high level functions like initialize, do it, terminate, and then return
zero or something like that. So the first thing you see associated with any C statement is usually or very often
the name of a function that gets invoked to kind of take control of 10 percent of the program or some small percent of the program just to get something done. 


Procedure, if you’re a purist about the definition, it is a block of code that gets called where you’re not concerned about a return value. Like you have to think about a procedure as a function that has as a return value.

```C
vector v;
vectorNew(&v, ...);
vectorInsert(&v, ...);
vectorSort(&v, ...);
```

## Object Oriented Paradigm 
C++

And the first thing that comes up in a statement of any particular – comes up in any one particular statement,
usually the first thing you look at is the piece of data that’s being manipulated. In this case, `v` is the data. It’s also called an `object`. Because this is up front, it looks like each statement is oriented around the object, which is why it’s called object oriented as opposed to procedurally oriented. 

```C++
vector<int> v;
v.push_back(4);
v.erase(v.begin());
```


## Concurrent Programming

There’s a little bit of a paradigm shift. You have to think a little bit differently about the problems that are gonna come up when you program in a threading environment versus a non-threading environment. A lot of the hard stuff is like figuring out how all the threads are gonna interact.
And so you’re thinking about multiple things at a time. And I’m actually like standing up a little bit more because I actually think with the back of my head when I’m programming concurrently because I’m trying to imagine all of these different scenarios of thread interaction that I have to worry about that have nothing to do with code. I actually have to see all these little players in like some thought cloud and how they might be interacting and how race conditions might come into being. And so concurrent programming and multithreading is it’s own paradigm that isn’t really
tied to any one particular language.

## Functional Paradigm

Scheme

The functional paradigm is very much oriented around the return value of functions. So let me just do an aside right here and just think of pure algebra, nothing sophisticated. But if you had a function `f(x, y)`, just think mathematical function. That looks like – it’s the name of some function that
takes two real numbers or two complex numbers or whatever, two pieces of data, and in traditional math, you know that it just returns or evaluates to something.

`f(x, y) = x ^ 3 + y ^ 2 + 7`

`g(x) = f(x, x + 1) + 8`

`h(x, y, z) = f(x, z) * g(x + y)`

What the functional paradigm approach does is it assumes you have lots of little helper functions that are interested in synthesizing one large result.


### Function

```Scheme
(define celsine->fahrenheit (temp)
    (+ 32 (* 1.8 temp)))
```

```Bash
> (celsine->fahrenheit 100)
212
```

```Scheme
> (define add (x y)
     (+ x y))
```


This is the first example of any kind of Scheme
expression we’ve dealt with so far that has some **side effect** associated with it. This purely synthetic approach where it takes the data and it synthesizes a return values
so that the overall expression evaluates to it, it does all of that **without printing to the screen or updating memory** in any way that we know about. You’re **not passing around raw pointers anywhere.** Even the lists themselves are being synthesized on your behalf. If you were trying to do the equivalent things in `C` and `C++` you would have to declare your list data structures or worse yet, you’d have to actually define a node type, and you’d actually have to thread together link lists using `malloc` or `new` and `free` or `delete` or whatever you have to do. You’d have to manage the memory allocation by yourself.
`Scheme` is so much more of a higher-level language and it’s smaller and it tries to do less
that it’s easier for it to do – take what it does and do it very, very well. The list, as
opposed to `C` or even technically `C++`, the list is a built-in data structure that’s core to the language. In purely functional languages, and we’re gonna strive for this in the `scheme` we’re gonna learn, you try to **program without side effect**.　Only to the extent necessary do you update variables by reference. I’ve certainly not done　any of that yet, okay. I’ve always just relied on what it evaluated to. 

Technically, there’s　a side effect associated with this right here in that in the global name space it associates　this `add` keyword, okay, to be associated with this functionality right here, so that from　this point on `add`, the way I’ve defined it, it actually is a built-in. It behaves more or less like a built-in just like `cons` and `car` and `cdr` and `list` and `append` all are. It’s almost as if there’s a global map of symbols mapping to
actual functions, where the functions themselves are expressed as lists, and that
map is prepopulated with code for `car` and `cdr` and `cons`. And then you can add to
it programmatically by associating this keyword with this list right here, which knows
how to behave like a function.

<br>

```Scheme
(add "Hello" "World") 
```

Think about the C and C++ equivalent. You would have had to attach data types, and you would have had to script this call up in the same file or some other file and compiled it so that at compile time it could detect that this isn’t gonna work out. There is
really very little compile time element to a Scheme interpreter. It just – when it parses the
list you type in, that’s technically compilation, but it **also evaluates it at the same time**. So there’s really very little separation between compile time and run time in Scheme, and because it’s **an active interpreter system**, we just call it the run time. So if I type this in, this would error out.

<br>
<br>
<br>

```Scheme
> (define sum_of (numlist)
    (if (null? numlist) 0
        (+ (car numlist)
            (sum_of (cdr numlist)))))

> (sum_of '(1 2 3 4))
10
```




## `Kawa` programming laugauge

```Bash
> 4
4
> "Hello"
Hello
> #f
#f
> 11.752
11.752
> 11/5
11/5
> 22/4
11/2
> (+ 1 2 3) # build list
6
> (* (+ 4 4) (+ 5 5))
80
> (> 4 2)
#t
> (< 10 5)
#f
> (and (> 4 2) (< 10 5))
#f

> (car '(1 2 3 4 5)) # List.head
1
> (cdr '(1 2 3 4 5)) # List.tail
(2 3 4 5)
> (car (cdr (cdr '(1 2 3 4 5))))
3
> (cdr '(4))
() # empty list
```


Now what’s this quote all about? If these quotes weren’t here, Scheme, then it would actually look for a function called `1`, And when it doesn’t find it, it’s gonna be like, “Whoa, I can’t apply the `1` function to `2 3 4 5`.” So it
would issue an error right there, okay. In our Kawaa environment, it’ll throw a Java exception to advertise the fact that it’s implemented in Java, but nonetheless, it will
break.

```Scheme
> (car '(1 2 3))  

;Value: 1

> (car (1 2 3))

;The object 1 is not applicable.
;To continue, call RESTART with an option number:
; (RESTART 3) => Specify a procedure to use in its place.
; (RESTART 2) => Return to read-eval-print level 2.
; (RESTART 1) => Return to read-eval-print level 1.

```

#### The quote

The quote just says basically to the parser, “Stop evaluating.”,  is the way to suppress evaluation抑制运算.
From everything from the parenthesis that I’m looking at to the parenthesis that matches it. 

`'(1 3 (4 (5)))` == `(quote (1 3 (4 (5))))`

It’s technically shorthand for this right here. It matches that, matches that, matches that, matches that, and quote is just this metafunction in place that doesn’t actually – it kind of evaluates its arguments, but as part
of the recipe for this quote function, it just doesn’t evaluate its arguments. It just takes them verbatim对他们逐字读取. 
#### Now why are they called `car` and `cdr`? 

There was a – the original implementation of either `Scheme` or `Lisp`, I’m not sure, was just on an architecture that had two exposed registers. One was called `the address register`, emphasis on the A, and one was called `the data register`.
And the `car` and the `cdr` that were most recently dealt with the addresses that implemented them were stored in the address register and the data register, and that’s where the `ar` and the `dr` come from, address register and data register. 

#### `cons` for construct

```Scheme
> (cons 1 '(2 3 4 5))
(1 2 3 4 5)
```

So `cons` is always supposed to be – take two
arguments. The first argument can pretty much be anything. The second one is supposed to be a list of the first type
because what more or less happens is that it takes this element right here. It pulls this – it effectively pulls this parenthesis in like it’s on a spring or something and drops the one in front. And whatever you get as a result of that is your resulting list. 

```Scheme
> (cons '(1 2 3) '(4 5))
((1 2 3) 4 5)
```
It’s very literal in the fact that it takes this one element, which happens to be a list one, two, three, and it kind of prepends it to the front of everything that resides in the second list. So this emphasizes a point. But lists in `Scheme` or any dialect of `Lisp` for that matter, can be **heterogeneous 异构的**.

There’s very little type-checking going on. There’s a little bit, but there’s not nearly　as much of a compile time element to `Scheme` as there is in `C` and `C++`. It just lets you
type in whatever you type in, and it’s only as it evaluates things that if it sees a typed mismatch, because you try to say add a float or a double to a string, that it’ll say, “You
know what? I can’t do that, okay.” **But it’s actually at run time when it does the required type analysis to figure out whether or not something will work out.** 

#### `append`


```Scheme
> (append '(1 2 3) '(4 5))
(1 2 3 4 5)
```
I can use a built-in called `append`. It actually does effectively remove that paren and that paren right there and build one big sequential list out of everything. But it basically just connect everything together. It’s like it removes all intervening parenthesis and whatever list is left in place is the return value. 

`append`, unlike CONS, can take an arbitrary number of arguments. You can even take one list if you want to.

```Scheme
> (append '(1 2) '(3) '(4 5) '(6 7 8))
(1 2 3 4 5 6 7 8)
```

```Scheme
> (append '(2 3) 1 '(4 5))
ERROR

> (append '(2 3) (list 1) '(4 5))
(2 3 1 4 5)
```