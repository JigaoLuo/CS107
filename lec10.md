### Call Convention

```C++
int main(int argc, char** argv)
{
  int i = 4;
  foo(i, &i);
  return 0;
}
```

Before `int i = 4;`:  
|Stack Segement (begin)|--|
|---|--|

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC <- pointed by `SP`: Stack Pointer|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>
<br>

After `int i = 4;`: 
|Stack Segement (begin)|--|
|---|--|

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 4 <- pointed by `SP`: Stack Pointer|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

```
SP = SP - 4;
M[SP] = 4; // int i = 4;
```

<br>
<br>

`foo(i, &i);`: 
|Stack Segement (begin)|--|
|---|--|

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 4 <- `main` until here|
|-|---|

|add + 16|add + 12 = &i|
|-|---|

|add + 20|4|
|-|---|

|add + 24|saved PC <- pointed by `SP`: Stack Pointer|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

```
SP = SP - 8;
Reg1 = M[SP + 8]; 
Reg2 = SP + 8;
M[SP] = Reg1;  // i as the first parameter
M[SP + 4] = Reg2;  // &i as the first parameter

CALL <foo>  // JUMP to and JUMP back later
SP = SP + 8 // after back: clean up para.
```

<br>
<br>
<br>

```C++
void foo(int bar, int* baz)  // para laid in Stack from right to left
{
  char snink[4];
  short* why;

  why = (short*)(snink + 2);
  *why = 50; 
}
```

Before **clean up**:
|Stack Segement (begin)|--|
|---|--|

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 4 <- `main` until here|
|-|---|

|add + 16|add + 12 = &i|
|-|---|

|add + 20|4|
|-|---|

|add + 24|saved PC|
|-|---|

|add + 28 = snink|snink[3] snink[2] snink[1] snink[0]|
|---|---|---|

|add + 32|why = snink[2] <- pointed by `SP`: Stack Pointer|
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|

<br>

```
foo:

SP = SP - 8; // for local varaible: snink and why

Reg1 = SP + 6;  // (snink + 2)
M[SP] = Reg1  // why = (short*)(snink + 2);

Reg1 = M[SP];  // *why
M[Reg1] = .2 50;  // *why = 50; 

SP = SP + 8  // clean up

RET
```

<br>

Before **clean up**:
|Stack Segement (begin)|--|
|---|--|

High Add.

|add|argv|
|-|---|

|add + 4|argc|
|-|---|

|add + 8|saved PC|
|-|---|

|add + 12|i = 4 <- `main` until here|
|-|---|

|add + 16|add + 12 = &i|
|-|---|

|add + 20|4|
|-|---|

|add + 24|saved PC <- pointed by `SP`: Stack Pointer|
|-|---|

|add + 28 = snink|snink[3] snink[2] snink[1] snink[0]: but cleaned up|
|---|---|---|

|add + 32|why = snink[2]: but cleaned up  |
|-|---|

Low Add.

|Stack Segement (end)|--|
|---|--|


`RET` for return, is understood to be an assembly code instruction that pulls `saved PC` out, places it – basically populates the true PC register with what this is, brings the SP register up four bytes and then carries on as if it were executing at PC + 4 all along. 




`RV` which is this 4B register dedicated to communicating **return values** between caller and callee functions.


<br>
<br>
<br>

### General Activation Record

|param.s|allocated and initialized by caller|
|-|---|

|Saved PC||
|---|---|---|

|local|allocated and initialized by callee|
|-|---|

<br>
<br>
<br>

### Recursion  - factorial 

```C
int fact(int n)
{
  if(n == 0) 
    return 1;
  return n * fact(n - 1);
}
```

```C
R1 = M[SP + 4];  // n
BNE R1, 0, PC + 12;  // branch, not equal | 12 = 3 * 4 
RV = 1;
RET;

R1 = M[SP + 4] ; // n
R1 = R1 - 1;  // n - 1 | frac(n) until here  
SP = SP - 4;  // one para.
M[SP] = R1; // para = n - 1
CALL <fact>;

SP = SP + 4;
R1 = M[SP + 4] ;
RV = RV * R1;
RET;
```

<object data="1.pdf" width="1000" height="1000" type='application/pdf'/>



 
Let me just draw a general activation record. Here are all the params. Here's the safe PC
where the return link, and here are all the locals. These are allocated and admitted by the
person calling the function. These are allocated and initialized by the actual function
that's being called, the callee. So the entire picture is the activation record that has to be
built in order for the code inside a function to execute and have access to all the variables.
It may seem a little weird that this part, and technically everything through that right
there, but that it's set up and initialized by the person calling the function, and the rest of
it is set up and initialized by the function itself. Why is there this separation of
responsibility between actually building the entire thing? Why can't the caller build the
entire thing? Do you understand what I mean when I say that? Why couldn't main set
aside space for all the variables?
Why couldn't foo actually set aside space for all the variables? The reason is that the
caller has to be involved in setting at least this portion up because only it knows how to
actually put meaningful parameter values in there. Does that make sense? Who else is
going to put the four and the ampersand of I in there other than the caller. Make sense to
people? This right here can't be set up by the caller because the caller has no idea how
many local variables are involved in the implementation of, in this case, foo. Okay. Does
that make sense?
So the separation of responsibility really is the most practical and sensible thing to do.
The caller knows exactly how many parameters there are, can look at the prototype to
tell, and it knows how to initialize it. That's why the top half is set up by the caller. The
bottom half, the caller doesn't even know how many local variables there are much less
how to manipulate them.
So you have to rely on the callee function, this is foo in this case, to go ahead and
complete the picture by deallocating SP to make space for the local variables. Now I put
this right here. When I say call foo right there, it's really a jump instruction that's
associated with the address of this first instruction right here. RET is an instruction to
basically jump back to whatever address happens to occupy the saved link. Okay. Sit well
with everybody? Okay, very good.
Let me write a little bit more of a practical function just so you understand how this
return register and call and return all work in a case of a recursive function. Okay. So let's
write a function that's a little more familiar to us. I want to write this function called
factorial which is framed in terms of a single parameter called N, and I'm not gonna have
any local variables. If it's the case that N double equals zero, I just want to go ahead and
return 1.
Otherwise, I want to go ahead and return m times whatever the recursive call to factorial
of M – 1 returns. I'm not concerning myself with M being negative. I don't care about the
error checking. I want to concern myself with how this translates to assembly code in our
little language. Okay. Let me erase this. Yep?
Student:Okay, in the first [inaudible] of foo, you substituted [inaudible], I mean are you
insinuating that you can substitute [inaudible]?
Instructor (Jerry Cain):In other words, do one variable at a time?
Student:Yes.
Instructor (Jerry Cain):Yeah, you could do that. It wouldn't be incorrect. Compilers
when they're generating this code, they can look at the full set of local variables that are
declared at the top, and it can compute the sum of all the sizes, and reduce the allocation
of all of them to one assembly code instruction instead of several.
Student:So it's more efficient, right?
Instructor (Jerry Cain):Well, yes, technically. I don't want to say that it's – that's not a
top priority, but real compilers would just use that right there because they could very
quickly add up all the variable sizes, and just know that they're gonna be packed together,
and just do this on one little swoop. Okay. Now as far as this factorial function is
concerned, I want this to label the first assembly code instruction that has anything to do
with this as a function.
It has to assume this as a picture. It has to assume that some value of N has already been
passed in. Okay. That whoever called it, laid down a call to the fact function, and as a
result of that, laid down some safe PC so that it knows where to jump back to after
factorial computes its answer. Does that sit well with everybody? We can momentarily
forget about the fact that function, call, and return is involved because the first few
statements right here are just normal little C code. Okay.
What I want to do up front is I want to load the value of N into a register, SP + 4 is the
address of M in this case. Okay. And I want to pull the 4 into a register called R1 because
I want to conditionally branch around this return statement if some test passes. Okay.
Well, I want to branch – I'll leave those open for the moment, depending on whether R1
and zero basically mismatch. If it's equal to zero, I just want to fall to the return statement
and scram, but if they're not equal, which is why I will put M and E right there.
If they're not equal, then I'm doing a little transition of this, then I want this to not execute
and it to come down here. So all I'll do is I'll put PC +, and I'll leave this open because I
don't know how many assembly code instructions I'm jumping forward yet until I execute
the code for that. Okay. If this test fails, it's because this test passes and I want to
basically populate RV with a 1 and then call return. So I would do this return value is
equal to 1 and then I would return. Okay. Does that make sense to people?
If this doesn't happen – I'm sorry, if this test fails, I better jump beyond the return
statement, which means that this should be a 12. Okay. And the next instruction I'm
gonna draw, has to start on the computation of this thing right here. Now I have no reason
to load M into a register because I'm only going to potentially clobber that register when I
call this function recursively.
So what I need to do is I have to prepare the value of M – 1, push that onto the stack
frame in preparation for a recursive call to factorial, let the recursive call do what it needs
to do, assume it puts an answer in the RV cubbyhole, and then use that answer
immediately to multiply it by what my local value of N is to figure out what RV should
not be populated with. So I will do this, R1 = M of SP + 4, that loads in into R1. I will
compute what
N 1 is.
And now I have all the information I need to make this recursive call. So I'm going to set
up the partial activation record. I'm gonna do SP = SP - 4. Okay. I'm gonna write to SP,
this value of R1. So what just happened here? I decremented – this is where SP is
pointing right now, I decremented to a point right there. That right there is the original
activation record. I'm in the process of building the activation record for the next call. I
lay down a 3 right there, because that's what R1 has at the moment. It had a 4, but I just
pulled it down to a 3. Okay. Make sense?
And then I call on the factorial function. In response to that, this is decremented four
more bytes, the address of this instruction is placed in the safe PC, but execution jumps
back to follow this recipe all over again, but on behalf of a second activation record. Does
that sit well with everybody? Yes? No? Okay.
So it's as if the primary call here – you just can think of it as suspending, it's not really
what's happening, but access to this activation record is temporarily suspended while the
recursive call deals with these addresses downward. You just have to assume by protocol,
that the recursive call is the person you're in cooperation with and the recursive call is
placing money in the RV locker, okay, so that when you pick up execution right here,
you know that the RV register has something meaningful. Okay.
Well, when you get right here, you have to clean up space for the local parameters. The
return statement that brings us back here, gets rid of this, and then that SP is equal to SP
+ 4, brings this arrow back up here, and that was the picture that was in place before we
involved any type of recursive function call or any function call at all. Okay.
We know that RV has – it would be six if factorial was really doing its job. So I'll
actually put that. It has a six in it, if we just take the leap of faith, but at the assembly
code level that the recursion is working, and then once I clean these up right here, I have
to reload M, so R1 now has a 4 in it. I can do this. That emulates that multiplication right
there. Okay.
RV has the return value of this. R1 has the value of that. I have no local parameters to
clean up. RV has the register that I need to communicate back to whoever called me, so I
will just return. Okay. Does that make sense to people? Question right there?
Student:[Inaudible]
Instructor (Jerry Cain):The PC + 12, where did I put that? I'm drawing a blank. Oh, this
right here? This is normally, if there are no branch instructions, you know how PC is the
address of the currently executing instruction, and each instruction is 4 bytes wide. So by
default with each clock cycle, the PC value was updated to be 4 bytes larger than it was
before. So normally it executes things in order.
If this is PC, this is PC + 4, PC + 8, PC + 12. If this branch instruction actually passes
then I don't want to fall to this, you specify as the third argument the actual address of the
instruction you should jump to relative to the current PC value. Okay. Now in some real
systems, PC's already been promoted by four, so this could be PC + 12 on some other
systems, but in our little world I'm just assuming that PC retains its value during the full
execution of this thing right here. Okay. And it's just identifying that right there. Were
there other questions? Yeah?
Student:What it be alright to do something less arbitrary –
Instructor (Jerry Cain):Yeah, then it's a little different. I'm gonna just constrain all our
examples that deal with things that are exactly 4 bytes. I'm sorry, 4, 2 or 1 bytes; things
that can fit into an RV register. MIPS as an architecture, I know that one best of all of
them, there are two return value registers and it usually only uses one of them unless
you're returning like a double or a long, long, which is an 8 byte integer, and it would use
both of them.
If you're returning a struct with 12 or more bytes in it, then what it'll do, is it'll actually
place the address of a temporary struct somewhere in memory, and just assume that the
caller knows that a struct is being returned and will take the RV register and dereference
it to go to the thing that's actually a struct. Does that make sense? So they figured it out.
I'm just not gonna deal with that particular stuff because it's more minutia to worry about.
Yeah?
Student:[Inaudible] I didn't quite get that.
Instructor (Jerry Cain):That's emulating that multiplication right there. At the C level
you know you have to spin on the return value from the cursive call, and multiply it by
the current local value of M. Right?
Student:Right.
Instructor (Jerry Cain):So this right here, just stores the current value of M and RV
right there, stores the result of the recursive call. Okay. Does that make sense? Now I
have the first piece of evidence as to why I always want you to reload variables. Do you
understand that right there I absolutely had, absolutely had to reload the value of M.
Okay. You may say, well I did it right there. Okay.
And even if I didn't do that right there, I did this with R2, I'd absolutely have to reload the
value of M because I have no idea how complex and how motivated this as a recursive
function, or any function at all, is to actually clobber all of the register values. All of the
function calls are using the same exact register set. Does that make sense? Okay. So that's
why I want you to get in the habit of reloading all your variables as you advance from
one C statement to the next one. Okay. Does this make sense to people? Yeah. Question
right there? Student:
Why do [inaudible] right after the return statement above?
Instructor (Jerry Cain):Because – this right here, for the same reason basically. I want
all code emission to be context insensitive and I don't want it to leverage off of things that
happened in prior C statements. Real compilers would do that. I just don't want us to do
it. I'm not adamant about it, but I just think that there's a nice clean formula about always
generating things from scratch because if this code changes because the C code statement
prior it changed, this code still does the right thing for the statement that it's being
admitted for. Does that make sense? Okay.
So I don't want to kill this example yet. I want to run through an animation, which is why
I have my computer here just so you see how the stack grows and shrinks in response to
assembly code instructions actually running. Yep?
Student:[Inaudible] why, for example, isn't there a PC - 12? Or not 12, PC – [inaudible]
Instructor (Jerry Cain):Oh, I see what you're saying. When the program is actually
loaded, this actually would be replaced by PC - 28 or whatever it is. These are just
placeholders at the moment because these are easier to deal with. Think of these as like
go-to labels. That's really what they function as at the assembly code level. A real
assembler or linker would in fact go through all the dot O files, and replace these things
right here with the actually PC-relevant addresses. It usually will defer it until link time
because it wants to be able to do the same replacement of these things with PC-relevant
addresses between functions that are actually in different dot O files. Okay.
And I'll talk a little bit more about that on Monday, and probably Wednesday about how
all the dot O's are basically assembled into an A dot out file or a 6 dash degrees or what
have you. Okay. Okay. So give me two seconds to do this. Okay. We should be good to
go. Create some mood lighting. Let's see how well this translates to the screen. Okay. So
this is more or less – it's an exact replica except for spacing of the function I just wrote on
the board. Okay. And, oh, that's not showing up. That's not good.
Can I get it to show up? It shows up over there. That's great. Oh, it's really up there. That
stinks. That's never happened before. Hold on a second. Maybe it'll just readjust to the
screen size. Stalling, stalling, stalling, stalling. It has no input so something's weird going
on. Okay. Actually, I have an idea. Yeah, but that's still not really very good. Let me do –
let me just run it in place. Okay, you guys can all see this right here. Let's make sure the
full picture can be seen. What's up?
Okay, there's that. It's actually pretty close. I don't think it zooms very well when it's
driving both this computer and that monitor. Okay. That's actually not bad. The return
type of fact is int. Okay. It's being clipped up by that mushroom that's on the screen up
there. So this right here is the code – the C code obviously. This right here is the
assembly code I more or less just wrote on the board. Okay. This is the animation, and I'll
try and do this this way. Let me do next slide. Yeah, this'll work. There's that. Okay.
So the original call is the factorial of 3 and it's just gonna follow these instructions one by
one and use the accumulation of the stack to just compute what 3 factorial is supposed to
be. Now it checks immediately. It loads in. It checks to see whether or not N = 0. It's not,
so it actually does branch forward and it prepares for the recursive call. Does this make
sense to people? Okay. It has to assemble the recursive value, has to make space on the
stack frame for that new – the recursive value of M, it has to write to there.
So you how we're basically in the process of building the activation record for the
recursive call and then we have to call factorial and some things are updated. The little
circle that's been populated in the safe PC portion, it actually has as a piece of popcorn, a
pointer to the next instruction so it knows where to carry forward when the recursive call
returns. Does that make sense? Okay. Question?
Student:So we aren't required to store PC?
Instructor (Jerry Cain):No, it actually happens – the call instruction does that for us.
Okay. Do this – comes back up here – the original call doesn't forget where it should
carry forward when the recursive call returns because we have that piece of popcorn in
the stack frame. Okay. So there is this is exactly the same thing where N = 2 and N = 3.
Notice that both recursive calls have as their safe PC the instruction after the recursive
call. That should make sense. They all need to know where to carry forward from after
the call returns.
It just happens to be the same place both times. It does go through one more recursive
call, lays down a zero, one more, and then finally it comes to an invocation. It's following
the recipe for a fourth time. The first three times it's suspended execution, but this time
this branch instruction is going to fail, so it's going to go and actually populate RV –
actually I can't – this right here has been updated with a 1, and so it's gonna go ahead and
return these second to last recursive call is like, oh, I'm active again, I'm going to
continue carrying on from where I was left off before.
I immediately look into the RV register and spin RV 1 x 1 stays the one, but as things
unwind, the all carry off from here, the 1 in RV becomes a 2 – this isn't working very
well – the RV becomes a 6, and finally I return to whoever made the original call which
is probably – or which may not be a factorial. Does that make sense to people? Yep?
Student:Those arrows that were pointing down onto the screen, where those –
Instructor (Jerry Cain):They were the safe PC registers. The safe PC blocks. You
remember the 4 bytes that existed between parameters and locals? Well there is a real
number that's placed there. It happens to be the address of the instruction that comes after
the call statement so it knows where to jump back to. It's almost as if it pretends that it
wishes the call thing were just one assembly code instruction, but since it isn't and it
jumps away only to come back later on, it needs to know how to come back. Okay. Okay.
So does that sit well with everybody?
So that was a breeze through that, but I have a little bit of stuff to talk about come Friday.
The one thing I will tell you is that C++, if you code in pure C++, you're probably
programming in the object-oriented paradigm so your object and data focused as opposed
to function and verb focused. In C, which we're dealing with right now, you always think
about the function names, the data is incidental, you always pass the data in its
parameters as opposed to having the data being messaged by method calls.
We're gonna see on Friday, and this will be an aside, I won't actually test you on this part
on the mid-term, but you'll see that C++ method calls and C function calls really translate
to the same type of assembly code that everything is emulated by a function call and
return with call and return thing at the assembly code level, and it's really just an
adaptation of either object orientation or imperative procedural orientation to assembly
code to get it to run, emulate either C code or C++ code. Okay. And that's what we'll
focus on on Friday. Okay. Have a good night and I will see you on Friday