### `map`

```Scheme
> (double_all '(1 2 3 4))
(2 4 6 8)

> (increse_all '(1 2 3 4))
(2 3 4 5) 

> (define (double x) (* x 2))

> (define (incr x) (+ x 1))

> (map double '(1 2 3 4))
(2 4 6 8)

> (map incr '(1 2 3 4))
(2 3 4 5) 

> (map car '((1 2 ) (4 8 2) (11)))
(1 4 11)

> (map cons '(1 2 8) '((4) () (2 5)))
((1 4) (2) (8 2 5))

> (map + '(1 2) '(3 4) '(6 10))
(10 16)

> (define (my_map fn seq)
    (if (null? seq) '()
        (cons (fn (car seq) (my_map fn (cdr seq))))))
```

<br>
<br>
<br>

### `eval`

Tthe interpreter actually reads input, tokenizes it, recognizes that most of them are integers, and it somehow, figures out how to invoke the `plus function`, based on what I typed in. 

`eval` recognizes the fact that everything, at least from – whether it’s sort of a file, or it’s typed in at the shell, comes in as a stream of text that needs to be tokenized and evaluated. 

```Scheme
> '(+ 1 2 3) # with that quote there, that suppresses evaluation.
(+ 1 2 3)

> (eval '(+ 1 2 3)) 
6
```

Whatever its 1 argument is, it evaluates it. In this case, it evaluates just to the list constant plus 1 2 3 because it’s quoted, and then it evaluates it as if it were typed
in at the command prompt in the interpreter.



<br>
<br>
<br>

### `apply`

```Scheme
> (apply + '(1 2 3)) 
6
```

`apply` does is it always takes exactly 2 arguments. 
- The first one is always supposed to identify some block of Scheme code. 
- The second always supposed to be a list of
the type of data that can actually be levied against by this type of operation.

So what `apply` does is, it effectively takes the `plus`; it cons it onto the front of this liast and then, it evaluates it in this `eval` sense. So this would come back with a 6. 
 
```Scheme
(define (average num_list)
    (/ (apply + num_list)
       (length num_list)))
```

<br>
<br>
<br>

### `flatten`

`((1 2) ((3) ((4) 5)) 10)` => `((1 2) (3 4 5) (10))`

```Scheme
(define (flatten seq)
    (if (not (list? seq) (list seq)
        (apply append (map flatten seq)))))
```

### `translate`

```Scheme
> (translate '(2 5 8 11 25) 100)
(102 105 108 111 125)

> (define (translate points delta)
    (map (lambda (x) (+ x delta)) points))

> (define (translate points delta)
    (define (shift_by x) (+ x delta))
    (map shift_by points))
```

<br>
<br>
<br>

```Scheme
(define (sum x y) (+ x y))
```

is equal as
```Scheme
(define sum (lambda (x y ) (+ x y)))
```