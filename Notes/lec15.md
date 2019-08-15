```C
int main() {
    int numAgents  = 10;
    int numTickets = 150
    for (int i = 0; i < numAgents; i++) {
        sellTickets(i, numTickets / numAgents);
    }
    return 0;
}
```

I have this really simple program, int main – and there’s no threading. I’m actually thinking about ticket
agents who answer the telephone at United or some other airline that still hasn’t declared
bankruptcy yet. In fact, let’s say that there are 150 seats on the flight. The idea of “sell tickets” is that it’s supposed to be in place to simulate the requirement that some ticket agent actually sells 15 tickets before his or her job is done. 

As long as these run in sequence, eventually you’ll get to the point where you actually sell 150 tickets in this really rude but simple simulation. The problem here is that in the real world, it’s just fine for all ten of these agents to be answering telephones simultaneously. 

The idea that we’re going to be able to get that `sellTicket` function right there to run in ten different threads. In other words, I’m going to spawn off ten different threads. All of them are going to be following the same exact recipe, where each one has to sell 15 tickets, and when a particular thread exists, we just know because of the way we code it up that 15 tickets have been sold. 

<br>
<br>

```C
void sellTickets(int agentId, int numTicketToSell) {
    while (numTicketToSell > 0) {
        printf("Agent %d sells a ticket\n", agentId);
        numTicketToSell--;
    }
    printf("Agent %d All done!\n", agentId);
}
```

So you know that the print out of this would have a 150 lines, 15 lines per ticket
agent, but they would be sorted all ticket agent number one followed by an all done
comment. 

This is a fairly compelling example where if we’re really trying to model the simulation of an actual airline ticketing room that you want to see all of these ticket agents running simultaneously and working, not competing, but working collaboratively to sell all 150 tickets at the same time. 


<br>
<br>

### Multi thread Version 1

```C
int main() {
    int numAgents  = 10;
    int numTickets = 150

    /// false: not print any debug information
    limitThreadPackage(false);

    for (int i = 0; i < numAgents; i++) {
        /// create a name that’s unique to a particular ticket agent.
        char name[32];

        sprintf(name, "Agent %d returned\n", i);
        ThreadNew(name, sellTickets, 2/*# paramter*/, i, numTickets / numAgents);
    }
    RunAllThreads();
    return 0;
}
```


`sprintf`: I’m gonna do that by declaring this in place buffer – not to the console, but to a character buffer `name`. I echo the characters in place to a character array `name` and make sure it turns out to be a C string.

`ThreadNew()`: 
 - `name` actually serves as the name of the thread and it’s also helpful for debugging purposes should you be passing a true to that thread package. 
 - And then you go ahead and you pass the address of the function that you’d like to execute in a single thread of execution (here `sellTickets`). This right here is just an arbitrary function pointer. 
 - You have to tell it how many arguments are expected of this particular function. We’re gonna have scenarios where the thread functions don’t need to take any arguments. We’re gonna have a scenario like we have right here where sell tickets takes two of them.
 - Beyond the `2`, you pass in `i`, `numTickets / numAgents`.
 
Now, this does not actually prompt `sellTickets` to start executing. All it does is it lines it up at a gate. lay down gates one through 10, all of these things that are gonna follow, the sell tickets recipe, that’s what they need to follow in order to run from the starting gate to the finish line and then 

`RunAllThreads` basically sounds the bell, fires the gun. Before you call `RunAllThreads` you have to call the `limitThreadPackage()` just exactly once and you have to set up all the threads, whether it’s directly in `main` or through `subfunction` to set up all of the dogs. This actually fires the gun and tells all the threads to start running. It turns
out that as threads execute, as part of their implementation, they can themselves call
`RunAllThreads`. 

The threads themselves can spawn their own child threads, grandchildren threads. 

<br>

### Multi thread Version 1

```C
void sellTickets(int agentId, int numTicketToSell) {
    while (numTicketToSell > 0) {
        printf("Agent %d sells a ticket\n", agentId);
        numTicketToSell--;
        if (RandomChance(0.1))
            threadSleep(1000);
    }
    printf("Agent %d All done!\n", agentId);
}
```

`threadSleep` basically says that as part of execution if a thread is running and it executes the thread sleep function that it pulls itself off the processor for at least, in this case, a second, that number that passes as an argument is expressed in milliseconds. So this means that every time it flips a biased coin and it comes up heads with probability of 10 percent rather, it’ll force
it to halt. 


<br>

What type of `print out` you might actually get in response to this thread implementation:

One possiblity:
```
Agent 1 sells a ticket
Agent 1 sells a ticket
Agent 2 sells a ticket
Agent 3 sells a ticket
Agent 3 sells a ticket
Agent 3 sells a ticket
Agent 3 sells a ticket
Agent 3 sells a ticket
Agent 3 sells a ticket
...
Agent ７ All done!
...
```

`numTicketToSell--;` not an atomic
operation. It looks like it’s atomic because it’s written in one statement right here, but
what happens is that this really corresponds to probably what we would as a local
variable, it would be a **three-assembly code statement.** So it’s gonna basically load `numTicketToSell` and do a registered decrement it by one and flush it back out to `numTicketToSell`. 

So when it gets swapped off a processor, it could be right before the first instruction of the three that this compiles to. It could actually finish right
after the third of the three, or it could be pulled off the processor 33 percent or 66.7 percent of the way through the code block that this compiles to. 


You feel every single stall time in sequence. If you
use threading and you spawn off 12 download from BBC server threads, all of them make enough progress, all of them try to open the connect and because that’s considered a block at the kernel level在内核级别中有阻塞机制, it’s pulled off the processor线程失去对处理器的控制. It’s a much more harsh version of　thread sleep. But it sleeps for a meaningful reason because it really can make a good　progress and the thread and the thread manager线程和线程控制器 realizes that so it pulls it off
a processor失去对处理器的控制权 while it’s waiting for the connection to be established. 

Well, imagine that all happening with 12 threads, all of those dead times that are associated with the network connection线程死亡时间总和网络链接有关, they all align and overlap and pipeline排队依次进入管道 in this
way that really saves us a lot of time.

HUGE WIN of performance: Save time at network connection, which is always needed and necessary for each thread to wait to be connected. So it is much faster to let all the thread to wait to be connected simutaneously.


<br>
(Back to tickets exampe)  

So what I want to do here is I want to complicate this problem a little bit, but complicate it in a meaningful way. In a real world simulation, it might be the case that you have **two ticket agents**, and you have to sell 10 more tickets and if somebody’s stuck on the phone because they want to buy the ticket or not, so the other ticket agent should be able to sell
all 10 tickets while the other is blocked with some time consuming customer. 


### Multi thread Version 2: not thread safe

```C
void sellTickets(int agentId, int* numTicketToSell) {
    while (*numTicketToSell > 0) {
        printf("Agent %d sells a ticket\n", agentId);
        (*numTicketToSell)--;
    }
    printf("Agent %d All done!\n", agentId);
}

int main() {
    int numAgents  = 10;
    int numTickets = 150

    /// false: not print any debug information
    limitThreadPackage(false);

    for (int i = 0; i < numAgents; i++) {
        /// create a name that’s unique to a particular ticket agent.
        char name[32];

        sprintf(name, "Agent %d returned\n", i);
        ThreadNew(name, sellTickets, 2/*# paramter*/, i, &numTickets);
    }
    RunAllThreads();
    return 0;
}
```

What I want to do with parameter `int* numTicketToSell`: is I want each agent to
know what their badge number is, but I also want them to be able to go back to the main function and find the copy of the number of variables that are remaining.

**Suppose each thread on its own function with each own stack fram with its local copy, now all of them maintain a pointer to that what is now have 150 from the begining of the execution.**

10 Thread stack pointing to the `main`'s variable `numTickets`.
 
<br>

Now, the problem and this is actually not even the full
problem, but I’ll simplify the problem to make it seem like it’s easily solved, is that I, as
**ticket agent1**, might come through and I might commit to that test and say, “Oh, wow,
there is, in fact, one ticket left, that’s greater than zero, so I’m gonna commit to selling this last ticket.”

And then boo-hoo, it gets swapped off the processor right after the curly sellin它买完票就暂停占用处理器了, but before anything associated with the `numTickets` minus minus还没来得及减一. So it gets swapped off the processor处理器被其他的线程占用 and **thread agent2** comes in and
executes the same test. “Oh, look, there’s one ticket left, I’m gonna sell this last ticket,” and it comes
in and it commits to trying to sell it, but it gets swapped off the processor. Same thing for
thread three, thread four, it could be this diabolical situation where everybody is really excited to sell the one remaining ticket. 

These threads don’t go back and recheck the test after they get the processor back, that’s not what is probability encoded, so they’re all gonna
try and decrement this shared global variable `numTickets` and so `1`, could, potentially, go down to `-9`. 

But you can understand the type of concurrency problem that exists here. They’re all depending on the same shared piece of data, and if they’re not careful in the way they manage the shared data and if it’s partway through the execution and it makes decisions based on information that will become immediately stale if its pulled off the processor then the integrity of the global data can actually be mucked with and be compromised. 

<br>

```C
    while (*numTicketToSell > 0) {
        printf("Agent %d sells a ticket\n", agentId);
        (*numTicketToSell)--;
    }
```

So, at the very least, we want this
all the way through that to, more or less, be executed in full. This region is what’s called a **critical region**. It’s, like, once I enter this region, no one else is supposed to be in here while I’m doing surgery on that global variable. 

Now, there’s nothing in the code that actually says, “Please other threads, don’t come in here because I am,” there have to be some directives that are put in place to block out other threads. This is the situation where you’re really glad that the bathroom door locks because if you’re in there, you don’t want them to have the privilege of just walking in because they’re running in their own little thread. You actually have to have a directive in place, this thing called a lock, I’m gonna frame it as a binary lock, I think for obvious
reasons, because you only want one person in the bathroom or in the critical region at any one moment. 

<br>
<br>
<br>

### Multi thread Version 3: Semaphore

```C
int main() {
    int numAgents  = 10;
    int numTickets = 150

    Semaphore lock = Semaphore new (-, 1);

    /// false: not print any debug information
    limitThreadPackage(false);

    for (int i = 0; i < numAgents; i++) {
        /// create a name that’s unique to a particular ticket agent.
        char name[32];

        sprintf(name, "Agent %d returned\n", i);
        ThreadNew(name, sellTickets, 3/*# paramter*/, i, &numTickets, lock);
    }
    RunAllThreads();
    return 0;
}
```

In a programming sense what a `semaphore` really functions as is **non-negative integer**, that as a data type has functionality that **supports atomic plus plus and atomic minus minus**. 

```C
semaphoreWait(lock);
semaphoreSignal(lock);
```
Now, those are functions that, behind the scenes,
emulate minus minus and plus plus, but they just figure out using special hardware or special instructions of the assembly code language to actually take the integer that’s wrapped around by the semaphore, in this case, what’s initially a one, and provide atomic
minus minus. 

The reason that `semaphoreWait(lock);` is the verb here is because we’re gonna generalize a little bit. **Think about the semaphore as tracking a resource.** In this case, there’s exactly one person allowed in the bathroom or there’s one person allowed into the critical region, which is why that’s a one in the
first place and you acquire that resource or you wait for that resource to be available and
when you don’t need it anymore, you signal it or you somehow release the lock. 


There’s one key point – is that because the `semaphore` integers in our world are **never allowed to go from non-negative to negative**, there’s a one special scenario that’s handled by `semaphoreWait(lock);`. If a `semaphoreWait(lock);` is passed to semaphore, that at the moment, it analyzes it and is surrounding a zero, **it doesn’t decrement it to negative one; it’s not allowed to do that**. That’s just the definition of what a `semaphore` is. If it detects that it’s a zero, it actually does what is called **`block`** and it blocks on that `semaphore`. It actually pulls itself off the processor暂停占用处理器资源 because it knows that it’s obviously waiting, presumably for some other thread to signal that thing before it could ever pass through that `semaphoreWait` call. 

Basically, if I’m jiggling敲门 the door for the bathroom, like we always do at restaurants to wonder
whether somebody is really in there or not, okay, you need, before you can really pass in there, **you need someone else to release the lock, some other thread or some other agent in the form of a semaphore signal call** before you really can go and open that door and
then you can look it yourself.

```C
/// the semaphore type is actually a pointer to an incomplete type. It’s not copies, it’s actually share some kind of strut behind the scenes that tracks the integer inside of it.
void sellTickets(int agentId, int* numTicketToSell, Semaphore lock) {
    while (true) {
        semaphoreWait(lock);
        /// Critical Region starts[
        if (*numTicketToSell == 0) break;
        (*numTicketToSell)--;
        /// Critical Region ends]
        printf("Agent %d sells a ticket\n", agentId);
        semaphoreSignal(lock);
    }
    printf("Agent %d All done!\n", agentId);
    semaphoreSignal(lock);  /// for case after break
}
```

1. while true, 
2. `semaphoreWait` for the lock. As a thread, I have no business following that pointer and looking at its value
and comparing it, using it in any sense, even comparing it to zero, because as I advance through the execution随着执行的深入, I can’t trust that that comparison is actually meaningful, if at any point during progression, it actually gets swapped off the processor and other threads can go and muck with that shared variable. 

So what I　want to do is I want to wait on the locked bathroom door and if I happen to be the one that first detects that it’s unlocked and I can go in and, in this atomic manner, actually do a decrement. So as I detect that it’s been promoted from zero to one, I actually take it from one down to zero and actually pass through this `semaphoreWait` call

3. 
```C
        if (*numTicketToSell == 0) break;
        (*numTicketToSell)--;
```

4. then I want to `semaphoreSignal` lock. The one thing I want to do here is that if, as a thread, I
acquire the lock and I notice that there are no more tickets to be sold, when I break out I don’t want to forever hold the lock on the bathroom. Okay. If you can programmatically unlock the door from afar you’re no longer in the critical region, but you still somehow
manage to unlock the bathroom door. 

<br>
<br>
<br>
<br>

I initialize the `semaphore` to `1`. That basically functions as a true for `1`. It basically says that the resource is available to exactly one thread and the first thread to get here actually does manage to, in an atomic way, take the one and do a minus minus on a down to zero because it actually committed to the
minus minus, takes the zero back to a one one. It may come back around and take the one back down to a zero.

But it’s always, like, lock, unlock, lock, unlock, lock, and maybe it actually gets swapped off the processor right after the `semaphoreWait`. That would normally be dangerous except that it’s leaving the semaphore in a state that it surrounds a zero. So there’s some other threads that the processor and it certainly will then they come here and they, basically, are blocked by a zero semaphore. 

<br>
<br>
<br>

### Multi thread Version 4: Wrong Semaphore

```C
int main() {
    int numAgents  = 10;
    int numTickets = 150

    Semaphore lock = Semaphore new (-, 0);

    /// false: not print any debug information
    limitThreadPackage(false);

    for (int i = 0; i < numAgents; i++) {
        /// create a name that’s unique to a particular ticket agent.
        char name[32];

        sprintf(name, "Agent %d returned\n", i);
        ThreadNew(name, sellTickets, 3/*# paramter*/, i, &numTickets, lock);
    }
    RunAllThreads();
    return 0;
}
```

Imagine a scenario where I accidentally – and this is actually the type of thing you have to be careful about because it’s so easy to type a zero versus a one
when you’re typing `Semaphore lock = Semaphore new (-, 0);`. 

If I do that right there, this creates a situation that you really have to be worried about when you’re dealing with concurrency and threads, is that if I accidentally lock the bathroom door before anyone comes to the party, everybody’s gonna be blocked and no one is in a position to actually unlock it. At least not the way
I’ve coded things up right here. 

If I make the mistake of putting a
zero up there `Semaphore lock = Semaphore new (-, 0);`, then every single thread will get this far and they’re all gonna be thinking that someone else is already locked that semaphore, so all 10 of them are pulled off
the processor and everybody’s just waiting. 

DEAD LOCK

<br>
<br>
<br>

### Multi thread Version 5: Wrong Semaphore

```C
int main() {
    int numAgents  = 10;
    int numTickets = 150

    Semaphore lock = Semaphore new (-, 2);

    /// false: not print any debug information
    limitThreadPackage(false);

    for (int i = 0; i < numAgents; i++) {
        /// create a name that’s unique to a particular ticket agent.
        char name[32];

        sprintf(name, "Agent %d returned\n", i);
        ThreadNew(name, sellTickets, 3/*# paramter*/, i, &numTickets, lock);
    }
    RunAllThreads();
    return 0;
}
```
If I have the opposite error and I do that right
there, from a programmatic standpoint, if it’s gonna be two, it might as well be 10. If you’re gonna let two people in the bathroom why not let all 10? If you’re gonna actually let two people go into the critical region and muck with global data at the same time, then
you have the potential for having two threads deal with a shared global variable in a way that they really can’t trust each other.


<br>
<br>
<br>

The semaphore – it is more less like a – basically a **synchronized counter variable** that is **always greater than or equal to zero**. 
- And so if I construct a semaphore around the number one, and I  levy a `semaphoreWait` call against the semaphore, this as a function figures out how to atomically reduce this one to a zero. And so this is basically equivalent to the **minus minus** but it does the **minus minus** in such a way that it actually fully commits to the demotion of the number to one that’s one lower than it. 

- 1 -> 0

<br>
<br>

- `semaphoreSignal` on the same exact semaphore would actually bring this back up to a one. 
- 0 -> 1

<br>
<br>

### Case with two many `semaphoreWait`

semaphore value|1|
|-|---|

```C
semaphoreWait(lock);
semaphoreWait(lock);
```

Something more interesting happens, where this one right here decrements the one down to a zero. Then this one right here would have a very hard time.
Because semaphores at least in our library – this isn’t the case in all systems, but our semaphores are **not allowed to go negative**. So, when you do a `semaphoreWait` against a `zero variable`, then this thread actually says, **I can’t decrement that, at least not now.** I need somebody else **in another thread** to actually `plus plus (call semaphoreSignal(lock))` this so that I can actually pass through a `minus minus` **without making the number negative**. 

So programmatically, the implementation of `semaphoreWait` is in touch with the thread library and so it actually when it’s attached to a `zero` behind the
scenes, it immediately says ok, **I can’t make any progress right now**. It pulls itself off the processor它自行关闭处理器. It records itself as something that’s called `blocked`记录成阻塞的状态 and it puts it in this cue of　threads are not allowed to make progress until some other thread `signals a semaphore` they’re waiting on.不允许继续运行，知道其他的线程call `semaphoreSignal`


`semaphore value` is not constrained to go between one and zero (binary). It can – this can be set to either be zero or one or five or ten. The only example we’ve seen so far is where the semaphore that’s coming in is initialized to surround the `one` because we really want it to function not so much as one as we want it to function as a true, and **it’s basically a light switch that goes on and off, on and off, and on and off**, and it’s used. And, we use `semaphoreWait` and `semaphoreSignal` against that semaphore to protect access to the `numTickets` variable that we have
been addressed to. 

<br>
<br>
<br>

## Register Behavior as threads get swapped off the processor

Somebody asked a very good question at the end of a lecture on Monday and I think I
want to go over it. Some people were concerned with the case where `numTickets` takes
minus minus, takes a one down to a zero, I don’t mean the lock, I mean the actual number of tickets and they thought that was the one problem we were worrying about.

The answer is that’s not the case and I can actually tell you a little bit more about what happens **as threads get swapped off the processor and where all of their data gets stored线程交换处理器的时候　数据是如何存储的,** and show you that if the number of tickets is originally 100, there is as much of a **race condition** without the `semaphoreWait` and `semaphoreSignal` calls in the `minus minus` bringing the 100 down to a 99, as there is in bringing a one down to a zero. So, this is
what would happen and this `(*numTicketToSell)--;` is going to be the most important line to concern ourselves with. 

<br>
<br>

Just think about the scenario where there’s two agents. It is subdivided:


|Stack|-|
|-|---|

|...|-|
|-|---|

|...|-|
|-|---|

|`the main stack frame`主堆栈结构. |-|
|-|---|

|&numTickets|numTickets = 100|
|-|---|

|...|-|
|-|---|

|...|-|
|-|---|

|`stack frame` Ticket Agent1|-|
|-|---|

|...|&numTickets|
|-|---|

|...|-|
|-|---|

|`stack frame` Ticket Agent2|-|
|-|---|

|...|&numTickets|
|-|---|

|...|-|
|-|---|

<br>
<br>
<br>

### Register Behavior (Not Thread safe)


- Agent1 has processor:  
So, in a local register set: 

|R1|&numTickets|
|-|---|

|R2|100|
|-|---|

after `--`:

|R1|&numTickets|
|-|---|

|R2|99|
|-|---|

But now you’re swapped off the processor交换处理器. So, you’ve actually committed to the sale of a ticket. But, you’re swapped off the processor. **The 100 still resides in `numTickets` because we didn’t successfully flush back.**只是在寄存器里面进行了减法，还没来得及写进原变量，就失去了处理器 What happens is that the entire register state, **all 32 registers, including the PC and the SP and the RV registers, if this is the binary state of all those registers, it’s actually copied to the bottom of the thread that’s being swapped out, little stack frame.**


|`stack frame` Ticket Agent1|-|
|-|---|

|...|&numTickets|
|-|---|

|R1|&numTickets|
|-|---|

|R2|100|
|-|---|

|Other Registers from 32 Register|-|
|-|---|

|...|-|
|-|---|

<br>
<br>

- After swapping off the processor, Agent2 has processor:  
So, in a local register set: 

|R1|&numTickets|
|-|---|

|R2|100|
|-|---|

after `--`:

|R1|&numTickets|
|-|---|

|R2|99|
|-|---|

The 100 still resides in `numTickets` because we didn’t successfully flush back. We didn’t get to the point where we actually update it to be the decremented value.

`Agent2` is prepared to flush with a 99 back to the
global `numTickets` when this `Agent1` gets the processor back; `Agent1` going to flush a 99 back to the same space. So, this 99 from `Agent1` is designed to override a 100. Btut is this `Agent2` is going to override a 100 and one of the 99s is going to overwrite the other thread’s 99.

SO: `Agent1` sold 1 ticket and `Agent2` sold one ticket, BUT `numTickets` = 99 => not thread safe

<br>

### Register Behavior (Thread safe with semaphore)

**Only one** thread is allowed to go through and actually pull the global value into the local register, decrement it locally, and then flush it back out to the global integer, the thing that functions as a global integer, because everybody has a pointer to the same integer. **Before any of the thread is allowed to do any part of that**, so that is what I mean when I say that this is more or less committed to
**atomically**. 

Now, that is the overarching principle that is in place when you have threads, and in particular, you have threads accessing shared information. This is the programmatic equivalent of the two ATM machines where me and my best friend try to take out the last remaining $100.00 in my account at the same time, thinking we’re going to get $200.00. 