## The low-level memory mechanics
To understand how data – as simple as Booleans and integers and floating-point numbers and strucks and classes – are all represented in memory. How everything ultimately gets represented as a collection of zeros and ones. And how it's faithfully interpreted every single time to be that capital A, or that number seven, or Pi, or some struck that represents a fraction, things like that. 

---
02:11

So, for the moment, C and C++ are the same language to me. So let's just talk about this.
Let me just put a little grid up here of all the data types that you've probably dealt with.
You've probably dealt with boole. You've probably dealt with CAR. I'm sure you have.
You may not have dealt with the short, but I'll put it up there anyway. You've certainly
dealt with the int. You've certainly – well, maybe not dealt with the long, but let's just
pretend you have. You've probably seen a floats. You've probably seen doubles. And
that'll be enough for me, enough fodder for the first ten minutes here. These three things
– I'm sorry. These three things right there are certainly related. They're all intended to
represent scalar numbers. Obviously, this represents a true or a false. This represents in
our world one of 256 characters. We usually only pay attention to about 75 of them, but
nonetheless, there are 256 characters that could be represented here. These are all
numeric. These take a stab at trying to represent arbitrarily precise numbers, okay? The
character is usually one byte in memory. At least it is in all C and C++ programmers –
program compilers that I know of. This is typically two bytes. The int can actually be
anywhere between two and four bytes, but we're going to pretend in this class that it’s
always four bytes, okay? The long, for the time being, is four bytes of memory. There is
another data type, which isn't really common enough to deserve to be put on the
blackboard, called the long long, which gives you eight bytes of memory to represent
really, really large decimal numbers. They'll come up later on, but I'll talk about them if I
ever need to. The float is four bytes. It somehow tries to take four bytes of memory and
represent an arbitrarily precise number to the degree that it can, given that it's using a 

finite amount of memory to represent a number that requires and infinite amount of
precision, sometimes. And a double, I've seen them ten and twelve bytes on some
systems, but we're just gonna assume that they're eight bytes. Now, that's the most boring
spreadsheet you could possibly introduce a class with, but my motivation is that I want to
uncover what the byte is all about, and how four bytes can represent a large frame of
numbers, how eight bytes can represent a very large set of numbers, and actually do a
pretty good job at representing numbers precisely enough for our purposes.
So forget about bytes for the moment. Now, I'll go back to the boole in a second, because
it's kind of out of character as to how much memory it takes. But I'm interested, at the
moment, in what is less commonly called the binary digit, but you've heard it called the
bit. And Double E students and those who enjoy electronics think of the binary digit in
terms of transistors and voltages, high and low voltages. Computer scientists don't need
to view it that way. They just need to recognize that a bit is a very small unit of memory
that can distinguish between two different values. Double Es would say high-voltage,
low-voltage. We don't. We actually just assume that a single bit can store a zero or a one.
Technically, a Boolean could just be mapped to a single bit in memory. It turns out it's
not practical to do that. But if you really wanted to use a single bit to represent a Boolean
variable, you could engineer your compiler to do that, okay? Bits are more interesting
when they're taken in groups. If I put down eight bits here – I'm not even going to commit
to a zero or a one, but I'm gonna draw this. This isn't zero over one as a fraction, this is
me drawing eight bits – let me just draw one over here so I have some room – and put a
little box around each one of them in this binary search way, okay? And I have this big
exploded picture of what we'll draw several times to represent a single byte of memory.
Now, the most interesting thing to take away from this drawing is that this little box right
here can adopt one of two values. Independently of whatever value this box chooses to
adopt, etc. In fact, there are eight independent choices for each of the bits. I'm assuming
that makes sense to everybody, okay? That means that this, as a grouping – a byte of
memory with its eight bits that can independently take on zeros and ones can distinguish
between two to the eighth, or 256 different values. Okay, and that's why the Ascii table is
as big as it is, okay? 65 through 65 plus 25 represents the alphabet. I forget where
lowercase A starts. But every single character that's ever printed to the screen or printed
to a file is backed by some number. I know you know that. When you look in memory to
see how the capital A is represented, you would actually see a one right there – I'm sorry,
I forget where it is actually – a one right there and a one right there. I'll draw it out and
explain why that's the case. Because capital A is backed by the number 65, we don't put
things down in decimal in memory. We put them down in base two. Okay? Because that's
what – that's the easiest thing to represent in a bit-oriented system, okay? That make
sense to people? Okay. So if I say that the capital A is equal to 65, you have to stop
thinking about it as 65. You have to think it about it as some sum of perfect powers of
two. So it isn't 64 – it isn't 65 rather, it's actually 64+1. One is two to the zero. A two is
two to the first. There's none of that. Four is two to the second. Eight is two to the third.
Sixteen is four. Thirty-two is five. Sixty-four is six. This is actually two to the sixth plus
two to the zeroth. Make sense? Okay. As far as the representation in a box like this, if
you went down and actually examined all the transistors, okay? The eight that are laid
side-by-side in a single character, but byte of memory, it would look like this.