# `Python`

scripting language: imperative, object oriented, functional

```Python
>>> z = 5
>>> z
5
>>> None
```

But do you understand that as opposed to pure `Scheme` where everything evaluates to something, `>>> z = 5` right there didn’t evaluate to anything. It had the **side effect** of actually associating the number `5` with `x`, but `>>> z = 5` didn’t print anything out. 

That’s because it evaluates to this `None`, which is basically the equivalent of `void` from C and C++, which means there’s really **no side effect or nothing** of interest in the evaluation of it. And `None` really doesn’t mean very much except that it doesn’t have anything printable about it.  

### list
List is in `python` mutable.

### `gatherDivisors()` function

```Python
>>> gatherDivisors(24)
[1, 2, 3, 4, 6, 8, 12, 24]

def gatherDivisors(number):
    divisors = []
    for div in range(1, number + 1):
        if number % div == 0:
            divisors.append(div)
    return divisors
```

### Use a `.py` file

```Python
>>> import divisors %% load and interrepter File: divisors.py

>>> divisors.gatherDivisors(24)
[1, 2, 3, 4, 6, 8, 12, 24]
```

OR

```Python
>>> from divisors import gatherDivisors
>>> gatherDivisors(24)
[1, 2, 3, 4, 6, 8, 12, 24]
```

### `dictionary`

```Python
>>> student = {}
>>> student
{}
>>> student['name'] = 'Linda'
>>> student['gpa'] = 3.90
```

The reason I’m talking like this is because the `dictionary` is easily the most malleable,
easily manipulated data structure in `Python` as opposed to lists in `Scheme` where you pass
around lists and you functionally manipulate them to create new lists. 

I say objects loosely. Anything, any aggregate data structure like a `list` or a `tuple` or a `dictionary`, they’re all **passed around by reference**. So if you write a function that takes a `dictionary`, it just makes **a shallow　copy** of it. Any changes that you make within the function are reflected in the original.