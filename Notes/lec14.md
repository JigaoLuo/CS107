## Process and MMU
 
What I want to do is I want to talk about how two applications on the　same processor simultaneously, and then use ideas from that to defend how two functions　within a process can seemingly run simultaneously. 

Let’s just　think about one of the pods. This is the virtual address base, virtual meaning the allusion
of a full address base that make has wireless running无限长. You don’t think about `Make` as an
application, but it certainly is. It’s designed to read in the data file that you call a `Make file` to figure out how to invoke GCC and G++ and the linker and purifying and all of those things, to build an instrument and executable. 

`make` process 1:
|Stack|-|
|-|---|

|4000|-|
|-|---|

|Heap|-|
|-|---|

|Code Segment|-|
|-|---|

While `make` is running, it’s probably the case that `GCC`, as an executable, is running several
times, but we’ll just talk about the snapshot or time slice where **just one GCC is running**.

<br>
<br>

`GCC` process 2:
|Stack|-|
|-|---|

|Heap|-|
|-|---|

|Code Segment|-|
|-|---|

`GCC` is an executable. You’re first C complier was was written in Assembly, but then it kept bootstrapping on the original compiler to build up more and more sophisticated compliers. So the C complier was written in C. 


<br>
<br>

I can tell you right now that they’re not both all in the same place. They do not share the stack and they do not share the heap and they do not share the code. 

This virtual picture was in place so that `make` can just operate **thinking** it owns all of memory. And it lets the smoke and mirrors that the OS managers, to map stack, heap and code segement to real addresses. It just wants to be insulated from that. 

<br>
<br>

`FireFox` process 3:
|Stack|-|
|-|---|

|Heap|-|
|-|---|

|Code Segment|-|
|-|---|

`Clock` process 4:
|Stack|-|
|-|---|

|Heap|-|
|-|---|

|Code Segment|-|
|-|---|

Maybe it’s the case that you have `Firefox` up and running on one of the Linux boxes, has the same picture. And then you have some other application, like `Clock` or whatever you have, up there and it has the same exact picture. 

Those are four virtual address bases that all seem to be active at the same time. 
But on a single processor machine, there is only one address base. 


`Real`:
|...|-|
|-|---|

|60000|-|
|-|---|

|...|-|
|-|---|

This is the real deal right there. That’s physical memory. And this has to somehow host all the memory that’s
meaning to `GCC` and `Make` and `Clock` and `Firefox`, or I should say the processes that are
running according to the code that’s stored in those executables. Does that sit well with
everybody? Well, it turns out that this and that right there, they may have the same virtual
address, but they can’t really be the same physical address. The space has to be truly
owned, or the values there in virtual space have to be truly owned by this process one. 


So what the operating system will do is it will invoke what’s called
the memory management unit MMU to basically build a table of process and something related to an address. And it’ll actually map it to a real address somewhere in memory. 

(# Process, virtul address) -> (physical address)  
(1, 4000) -> (60000)

Any request to manipulate or deal with `virtual address 4,000` is somehow **translated** to a request to deal with the `physical address at address 600,000`. Any type of load or store or access of this right here has to somehow be proxied or managed by this daemon process (MMU Translation) behind the scenes, this thing
that just runs in the background all the time, to actually map virtual addresses to physical addresses. 

And it knows that this `virtual address 4,000` is the virtual address that `process1` owns, so it just
has this little map of information – I’ve drawn it has a map of pairs to physical addresses
– so it knows exactly where to look on behalf of this process. It stores them in address 4,000; it updates the four bytes that resides at address 600,000. Now, it doesn’t really clip these things down at the four-byte level. Normally, what will happen is it’ll allocate things – I don’t mean allocate in a malik sense – it’ll just associate very large blocks in virtual address space with very equally large blocks in physical address space. So this might be some 1K or 8K block of memory (A Page). 

And if it’s ever used, then it’s sure to map the same size block or adopt the same size block (A Page) in physical memory so that virtual address 4,000 through address 8,000, would map to physical address 60000 
through 64,000. So there’s some contiguous nature going on between all the things in this virtual address base and what it maps to in physical space.

In theory, you can run 40 applications. Usually it’s the case that the `stack segment` is never really that big. It’s initially **slotted** off to be fairly small for most applications. Because unless you’re going to call Fibonacci of a billion, it’s probably not going to have a call depth greater than 50 or 60. In fact, when you have a stack called `f` of 50 or 60, it usually means a lot of things are happening. The
distance down from `main` to the `subhelper` to the 59th power function. I don’t want to say
that’s unusual – maybe 100 is normal, maybe 200 is normal; 2 billion is not. 

You can share address spaces across applications, but you have to use
advanced unit directives to do that. 

This is multiprocessing. Several processes are seemingly running at
the same time, and each process has its own heap and its own stack and its own code
segment, and its virtual space.

<br>
<br>
<br>

## Thread

The part that is not clear, and this is going to become more clear, hopefully, next week and the Monday after it, is how the applications seemingly run at the same time, when there’s really only one register set, one processor digesting instructions at a time. 

What really happens if that Make makes a little bit of progress, and then GCC
makes a little bit of progress, Make makes a little bit, and this just all happens in this
interlay fashion, so fast that you don’t see any one lagging over the other one.


Slightly different to multiprocessing, but certainly related, is the idea that two functions in the same process, one code segment, one heap segment, technically one `stack segment`. We’re curious as to
whether or not it’s possible for two functions to seemingly run at the same time inside a
single process. 


iTunes, you buy an album of 13 songs and you check the actual download screen and three songs at a time are actually downloading. Not really. And it’s pulling things in in little time slices, but it happens so fast and the time slices are so small compared to what we can detect, that it looks like all three songs are being downloaded simultaneously. 
When you’re downloading music, there’s one process that’s doing it, it’s iTunes. And internally, it has
some function related to the downloading of a single song that at any one moment it’s allowing to run, seemingly three at the same time. So just imagine the scenario where there are two songs downloading at
the same time. In that case, they both would be following the same assembly code block. They have the same recipe for downloading a song, for the most part. In that case, the `stack segment` itself could be subdivided into smaller substacks, where the stack frame for song one could be right there, and the stack frame for the downloading of song two is in this completely unrelated space but in the same `stack segment`, but far enough away that you’re not going to have one stack run over the top of another one. 

So when the process has the processor, it also subdivided its
time to switch back and forth between the two threads. Two unrelacted blocks in the same `stack segment` switch to have time to download songs. And it keeps doing that until one or both of them ends. 

It shares the heap. So they all share the same call to `malloc` and they all draw
their memory from the same memory pool.
There’s only one copy of the code. You only need one copy of the code. It’s read only.

It’s fine for this stack and this stack to both be guided by the same set of assembly code
instructions, as long as each one has some kind of thread identifier. 


```C
int main() {
    int numAgents = 10;
    for (int i = 0; i < 10; i++) {
        sellTicket(i, 15);
    }
}
```

I have
this really simple program, int main – and there’s no threading. I’m actually thinking about ticket
agents who answer the telephone at United or some other airline that still hasn’t declared
bankruptcy yet. In fact, let’s say that there are 150 seats on the flight. The idea of “sell tickets” is that it’s supposed to be in place to simulate the requirement that some ticket agent actually sells 15 tickets before his or her job is done. 

As long as these run in sequence, eventually you’ll get to the point where you actually sell 150 tickets in this really rude but simple simulation. The problem here is that in the real world, it’s just fine for all ten of these agents to be answering telephones simultaneously. 

The idea that we’re going to be able to get that `sellTicket` function right there to run in ten different threads. In other words, I’m going to spawn off ten different threads. All of them are going to be following the same exact recipe, where each one has to sell 15 tickets, and when a particular thread exists, we just know because of the way we code it up that 15 tickets have been sold. 