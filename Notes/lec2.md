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

```
s    ........ ........ 00000100 00001001 
i    00000000 00000000 00000100 00001001 
```

30：00


Suppose I did this. Short S is equal to
negative one. Totally reasonable. Do any of you have any idea what the bit pattern for
that would look like? And you can only answer if you didn't know the answer prior to
11:00 a.m. today. Okay. I want to be able to add one to negative one and get all zeros,
okay? Does that make sense? So the representation for this is actually all ones. In fact,
anytime you see all ones in a multi-byte figure, it means it's trying to represent negative
one. Why? Because when I add positive one to that, it causes this domino effect and
makes all those ones zeros. Does that make sense? So to answer your question, int i equal
to S, logically I'm supposed to get int to be this very spacious representation of negativ

one. It actually does use the bit pattern copy approach. It copies these. I've just copied all
the magnitude, okay? And by what I put down there, it's either one or – I'm sorry. It's
either a very large number or it's negative one. We're told that it's negative one right
there, okay? What happens is that when you assign this to that right there, it doesn't just
place zeros right there because then all of the sudden it would be destroying the sign bit.
It would be putting the zero in the sign bit right there. Make sense? So what it really does,
and it actually did this over here, but it was just more obvious, is it takes whatever this is
in the original figure and replicates that all the way through. If these would have
otherwise been all zeros, and I want to be able to let this one continue a domino effect
when you add a positive number to a negative number, you technically do what's called
"sign extend" the figure with all of these extra ones. So now you have something that has
twice as many dominos that fall over when you add positive one to it, okay? Does that
make sense?
Okay. So there you have that. As far as character shorts, ints, and longs, they're all really
very similar in that they some binary representation in the back representing them. They
happen to map to real numbers, for ints, longs, and shorts. They happen to pixelate on the
screen as letters of the alphabet, even though they're really numbers, very small numbers
in memory, okay? But the overarching point, and I don't want you to – I actually don't
want you to remember – memorize too much of this. Like, if you know what – if you
know that seven is one, one, one, and you know that all ones is negative one, that's fine. I
just want you to understand the concept with integers I have four bytes. I have 32 bits.
That means I have two to the thirty-second different patterns available to me to map to
whatever subset of a full integer range I want. The easiest thing to do is to just go from
two to the negative thirty-first through two to the positive thirty-first minus one, okay?
There's zero in the middle. That's why it breaks it symmetrically a little bit. When I go
and start concerning myself with floats – I – you're probably more used to doubles, but
this is just a smaller version of doubles. I have four bytes available to me to represent
floating-point numbers, integers with decimal parts following it, in any way I want to.
This isn't the way it really works, but let me just invent an idea here. Pretend that this is
how it works. We're not drawing any boxes yet. I could do, let's say I have a sign bit. I'll
represent that up here as a plus or minus. And if I have 32 bits, you'd, by default, thinking
about bits and contribution of two to the thirtieth, two to the twenty-nine, all the way
down through some contribution of two to the zero. And I'm just describing all the things
that can adopt zeros or ones to represent some number, okay? But I want floats to be able
to have fractional parts.
So I'll be moving in the fractional direction, and say, "You know what? Why don't I
sacrifice two to the thirtieth, and let one bit actually be a contribution of two to the
negative first?" I'm just making this up. Well, I'm not making it up. This is the way I've
done it the last seven times I taught this. But I'm moving toward what will really be the
representation for floating-point numbers. If I happen to have 32 bits right here. And I lay
down this right here. That's not the number seven – I'm sorry, that's not the number 15
anymore. Now, it's number seven point five. Does that make sense? Okay, well floats
aren't very useful if now all you have are integers and half-integers. So what I'm gonna do
is I'm gonna stop drawing these things above it because I have to keep erasing them. Let's

just assume that rather than the last bit being a contribution of two to the negative first, let
me let that be a contribution of negative two to the negative first, and that let that be a
contribution of two to the negative two. Now I can go down to quarter fractions. Does
that make sense? Well, what I could do is I could make this right here a contribution of
two to the zero, two to the negative one, two to the negative two, three four, five, six,
seven, eight, two to the negative nine. And if I wanted to represent Pi – I'm not going to
draw it on the board because I'm not really sure what it is, although I know that this part
would be one, one – then I would use the remaining nine bits that are available to me,
okay, to do as good a job using contributions of two to the negative first, and two to the
negative third, and two the negative seventh to come as close as possible to point one
four one five whatever it is, okay? Does that make sense to – I'm assuming? It is an
interesting point to remember that because you're using a finite amount of memory,
you're not going to do a perfect job representing all numbers in the infinite, and infinitely
dense, real number domain, okay? But you just assume that there's enough bits dedicated
to fractional parts that you can come close enough without it not really impacting what
you're trying to do, okay? You only print it out to four decimal places, or something that
just looks like it's perfect, okay? Does that make sense? It turns out if I do it that way,
then addition works fine. So I add two point five contributions and it ripples to give me a
one and I carry a one. It just words exactly the same way. Does that make sense? Okay. It
turns out that this is not the way it's represented, but it is a technically a reasonable way
to do it. And when they came up with the standard for representing floating-point
numbers, they could have gone this way. They just elected not to.
So what I'm gonna do now is I'm gonna show you what it really does look like. It's a very
weird thing. But remember that they can interpret a 32-bit pattern any way they want to,
as long as the protocol is clear, and it's done exactly the same way every single time. So
for the twentieth time today, I'm gonna draw a four byte figure. I'm gonna leave it open as
four byte rectangle because I'm not gonna subdivide it into bytes perfectly. I'm going to
make this a sign bit because I do want to represent – I want negative numbers and
positive numbers that are floating-point to have an equal shot at being represented, okay?
That's one of the 32 bits. Does that make sense? The next eight bits are actually taken to
be a magnitude only – I say it that way. I should just call it an unsigned integer – from
here to there, okay? And the remaining 23 bits talk about contributions of two to the
negative one, and two to the negative two, and two to the negative three. Okay, this right
here, I'm gonna abbreviate as EXP. And this right here, I'm just gonna abbreviate as dot
XXX XX, okay? The – what – this figure and how it's subdivided is trying to represent
this as a number. Negative one to – I'll abbreviate this as S – to S right there. One point
XXX XX times two to the one twenty-eight – I'm sorry, hold on a second. EXP minus
one twenty-seven, okay? It's a little weird to kind of figure out how the top box matches
to the bottom one. What this means is that these 23 bits somehow take a shot at
representing point zero, perfectly as it turns out, to something that's as close to point nine,
nine bar as you could possibly get with 23 bits of information. When these are all ones,
it's not negative one. It's basically one minus two to the twenty-third. Does that make
sense to every? Okay. That is added to one to become the factor that multiplies some
perfect power of two. Okay? This right here ranges between two to the eighth – I'm sorry,
255 and zero. Does that make sense?

When it's 255 and it's all ones, it means the exponent is very, very large. Does that make
sense? When it's all zeros, it means the exponent is really small. So the exponent, the way
I've drawn this here, can range from 128 all the way down to negative 127. Makes sense?
That means this right here can actually scale the number that's being represented to be
huge, in the two to the one twenty-eight domain, or very small, two to the negative one
twenty-seventh, okay? The number of added to the world down to the size of an atom,
okay? You may think this is a weird thing to multiply it by, but because this power of
two-thing right there really means the number is being represented in the power of two
domain. You may question whether or not any number I can think of can be represented
by this thing right here. And then once you come up with a representation, you just
dissect it and figure out how to lay down a bit pattern in 32 byte – 32-bit figure. Let me
just put the number seven point zero right there. Well, how do I know that that can be
represented right here? Seven point zero is not seven point zero. It's seven point zero
times two to the zeroth, okay? There's not way to get and layer that seven point zero over
this one point XXX and figure out how – what XXX should be. XXX is bound between
zero and point nine bar. But I can really write it this way. Three point five times two to
the first, rather one point seven five times two to the second. So as long as I can do a plus
or minus on the exponent, I can divide and multiply this by two to squash this into the
one to one point nine range. And just make sure that – I have to give up if this becomes
larger than 128 or less than negative 127. But you're dealing with, then, absurdly large
numbers, or absurdly small numbers. But doubles love the absurdity because they have
space for that accurate of a fraction, okay? Does that make sense to people? Okay, so this
right here happens to be the way that floating-point numbers are actually represented in
memory. If you had the means, and you will in a few weeks, to go down and look at the
bit patterns for a float, you would be able to pull the bit patterns out, actually write them
down, do the conversion right here, and figure out what it would print at. It would be a
little tedious, but you certainly could do it. And you'd understand what the protocol for
coming from representation to floating-point number would be, okay? Let me hit the last
ten minutes and talk about what happens when you assign an integer to a float, or a float
to an integer, okay? I'm gonna get a little crazy on you on the code, all right. But you'll be
able to take it.
I have this int i is equal to 35 – actually, let me chose a smaller number. Let me do just
five is fine. And then I do this. Float F is equal to i. Now you know that this as a 32-bit
pattern had lots of zeros, followed by zero, one, zero, one at the end, four plus one, okay?
Makes sense? When I do this, if I print out F, don't let all this talk about bits and
representation confuse the matter. When you print out F there, it's going to print the
number five, okay? The interesting thing here is that the representation of five as a
decimal number is very, very different than the representation of five using this protocol
right here. So every time – not that you shouldn't do it – but every time you assign an int
to a float, or a float to an int, it actually has to evaluate what number the original bit
pattern corresponds to. And then it has to invent a new bit pattern that can lay down in a
float variable. Does that make sense? This five is not – the five isn't five so much as it is
one point two five times two to the second. Okay, as far as this is concerned right here.
So that five, when it's really interpreted to be a five point zero, it's really taken to be a one
point two five – is that right? Yeah. – Times two to the second. So we have to choose

EXP to be 129 and we have to choose XXX to be point two five. That means when you
lay down a bit pattern for five point zero, you expect a one to be right there. And you
expect one – one, zero, zero, zero, zero, zero, zero, one to be laid down right there, 128
plus one. Does that make sense to people? You gotta nod your head, or else I don't know.
Okay. This is very different – and this is where things start to get wacky – and this is
what one oh seven's all about. If I do this right here, int i is equal to 37. And then I do
this, float F is equal to asterisk – you're all ready scared. Float, star, ampersand of i. I'm
gonna be very technical in the way I describe this, but I want you to get it. The example
above the double line, it evaluates i, discovers that it represents five, so it knows how to
initialize F. Does that make sense? This right here isn't an operation. It doesn't evaluate i
at all. All it does is it evaluates the location of i. Does that make sense? So when the 37,
with it's ones and zeros represented right there, this is where i is in memory. The
ampersand of i represents that arrow right there, okay? Since i is of type int, ampersand
of i is of type int, star, raw exposed address of a variable. That's four bytes that happens
to be storing something we understand to be an int. And then we seduce it, momentarily,
into thinking that it's a float star, okay?
Now, this doesn't cause bits to move around, saying, "Oh, I have to pretend I'm
something else." That would be i reacting to an operation against the address of i, okay?
All the furniture in the house stays exactly the same, okay? All the ones and zeros assume
their original position. They don't assume, they stay in their original position. It doesn't
tell i to move at all. But the type system of this line says, "Oh, you know what? Oh, look.
I'm pointing to a float star. Isn't that interesting? Now, I'm gonna be reference it." And
whatever bit pattern happened to be there corresponds to some float. We have no idea
what it is, except I do know that it's not going to be thirty-seven point zero, okay. Does
that make sense? In fact it's small enough that all the bits for the number 37 are gonna be
down here, right, leaving all of these zeros to the left of it, okay? So if I say stop and look
at this four byte figure through a new set of glasses, this is going to be all zeros, which
means that the overall number is gonna be weighed by two to the negative one twentyseven. Makes sense? There's gonna be some contribution of one point XXX, but this is
nothing compared to the weight of a two to the negative one twenty-seven. So as a result
of this right here, and this assignment, if I print out F after this, it's just gonna be some
ridiculously small number because the bits for 37 happen to occupy positions in the
floating-point format that contribute to the negative twenty-third, and to the negative
twentieth, and things like that, okay? Does that make sense to people? Okay. So this is
representative of the type of things that we're gonna be doing for the next week and a
half. A lot of the examples up front are going to seem contrived and meaningless. I don't
want to say that they're meaningless. They're certainly contrived because I just want you
to get an understanding of how memory is manipulated at the processor level.
Ultimately, come next Wednesday, we're gonna be able to write real code that leverages
off of this understanding of how bits are laid down, and how ints versus floats versus
doubles are all represented, okay? I have two minutes. I want to try one more example. I
just want to introduce you to yet one more complexity of the C and C++ type system, and
all this cast business. Let me do this. Let me do float F is equal to seven point zero. And
let me do this short S is equal to asterisk, short star, ampersand of F. Looks very similar

to this, except there's the one interesting part that's being introduced to this problem, is
that the figures are different sizes, okay? Here I laid down F. It stores the number seven
point zero in there. And that's the bit pattern for it, okay? The second line says, "I don't
care what F is. I trust that it's normally interpreted as a float, and that's why I know that
this arrow is of type float, star." Oh, let's pretend – no, it isn't any more. You're actually
pointing – that arrow we just evaluated? It wasn't pointing to a float. We were wrong. It's
actually pointing to a two byte short. So all of the sudden, it only sees this far, okay? It's
got twenty-forty vision, and this right here, this arrow, gets dereferenced. And as far as
the initialization of S is concerned, it assumes that this is a short. It assumes that this is a
short so it can achieve the effect of the assignment by just replicating this bit pattern right
there, okay? And so it gets that. Okay, and whatever bit pattern that happens to
correspond to in the short integer domain, is what it is. So when we print it out, it's going
to print something. Seven point zero means that there's probably gonna be some non-zero
bits right here. So it's actually going to be a fairly – it's gonna have ones in the upper half
of the representation. So S is gonna be non-zero. I'm pretty sure of that, okay? Does that
make sense to people?
Okay. That's a good place to leave. Come Monday, we'll start talking about – we'll talk a
little bit about doubles, not much. Talk about strucks, pointers, all of that stuff, and
eventually start to write real code. Okay