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

||0|0|0|0|0|0|0|0||0|0|0|0|0|1|1|1|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

|+|1|0|0|0|0|0|0|0||0|0|0|0|0|1|1|1|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

||1|0|0|0|0|0|0|0||0|0|0|0|1|0|0|0|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
But not $0$, when we try to $7 + (-7)$


The reason it's not represented this way is because we want addition and subtraction to actually follow very simple rules.

16:23

Let's put a
four in there. Let's put an eight in there. If I followed the same rules – and think about – I
mean it's not like – the hardware is what's propagating electrons around and voltages
around to emulate addition for us. If we want to follow the same rules, we would say,
"Okay. Well, that's naught two. Carry the two. That's three. Carry the one. That's that."
Let me just make sure I don't mess this up. Seven plus seven is fourteen, so it would be
that right there. Okay. And then you'd have 11 zeros followed by a one. If I really just
followed the algorithm I did up there blindly, that's how I would arrive at zero. Okay.
And that's obviously not right. If I were to argue what representation that is, if this is
negative seven, then this has to be negative fourteen. That's not what 7 plus negative 7 is.
Okay. So that means that this right here, as negative number, has to be engineered in such
a way that when you add this to this using normal binary ripple add pattern, okay, that
you somehow get 16 zeros right here, okay? It's easier to not worry about how to get all
zeros here. It's actually easier to figure out how to get all ones here. So if I put down four,
five, six, seven, eight. One, two, three, four, let's mix it up. Let's put the number 15 down.
And I want to figure out what number – or what bit pattern, to put right here to get all
ones right here, then you know you would put a bit pattern right there that has a one
where there's a zero right here and a zero where there's a one up here, okay? This right
here is basically one mile away from turning over the odometer, okay? Does that make
sense? Okay. So what I want to do is I want to recognize this as 15 and wonder whether
or not this is really close to negative 15.
And the answer is, yes, it is because if I invert – take this right here and I invert all of the
bits, if I add one to that number right there, do you understand I get this domino effect of
all of these ones becoming zeros? I do get a one at the end, but it doesn't count because
