## Reader Writer Example with Semaphore

I’m trying to model right now the Internet, where in all the world there’s only server that serves up all the web pages and you have the only other computer with the only browser in the world.

I know you know enough about the HTML server process. You may not know all
the mechanics at the low level, but fundamentally, you know that you request a web page from another server. It serves up text in the form of HTML normally. It could be XML, but normally it’s HTML, and as the HTML comes over, it does a partial and eventually a full rendering of the HTML in your browser page.

You know and you felt this before where a page is loaded like 70 percent but it’s not
quite done yet, you see the progress bar at the bottom, the bottom right, where it’s like three-fourths of the way through and you know there’s more to come. That’s usually because the server has only delivered 75 percent of the content and so this thing has to block in much the same way that the threads up there block, this has to block and stop its rendering process until it gets more data from the server.

<br>
<br>

### Reader Writer (not thread safe)

```C
char buffer[8];

int main() {
    /// false: not print any debug information
    limitThreadPackage(false);
    ThreadNew("Writer", Writer, 0);
    ThreadNew("Reader", Reader, 0);
    RunAllThreads();
    return 0;
}
```
So, just use that as a guiding principle for this example. I’m going to insult all the Internet and I’m going to reduce it to a character buffer of size eight. And what I want to do is I want to **write a program that simulates the writing and the reading process** and I’m just going to reduce the server to something that has to populate that buffer as a **ring buffer**. 

In other words, it’s **going to write 40 bytes of information, but it’s going to cycle through the same array five to five times** and **I’m going to write another thread that consumes the information by cycling through the same array five times and digesting all the characters that are written there**. 

<br>

It is trying to emulate the fact that the `server` and the `client` as computers are
both running at the same time. Programmatically I want the `writer thread` to cycle through and write 40 characters to the Internet. I want this `reader thread` to consume the 40 characters that are written in the Internet. 


```C
void Writer() {
    for (int Writer_i = 0; Writer_i < 40; Writer_i++) {
        char c = PrepareReadChar();
        buffer[Writer_i % 8] = c;
    }
}
```

I write this with hopes that it writes data down before the reader consumes it but it doesn’t go so far that it clobbers data that has yet to be read. 我希望它在读取之前被写入一些，但不会走那么远，因为写如超过８个就会覆盖自己原来没有被读取的数字


```C
void Reader() {
    for (int Reader_i = 0; Reader_i < 40; Reader_i++) {
        char c = buffer[Reader_i % 8];
        ProcessChar(c); /// Do not care this function
    }
}
```

Now, you know, without concurrency, you know exactly how you want writer and reader to behave, so that everything is preserved and the data integrity is respected, and that the reader processes all the character data in the order that the writer writes it.

<br>

Start:  
|buffer[0]|-|
|-|---|

|buffer[1]|-|
|-|---|

|buffer[2]|-|
|-|---|

|buffer[3]|-|
|-|---|

|buffer[4]|-|
|-|---|

|buffer[5]|-|
|-|---|

|buffer[6]|-|
|-|---|

|buffer[7]|-|
|-|---|

<br>

`Writer` starts to write:

|buffer[0]|A|
|-|---|

|buffer[1]|2|
|-|---|

|buffer[2]|#|
|-|---|

|buffer[3]|-  <- Writer_i|
|-|---|

|buffer[4]|-|
|-|---|

|buffer[5]|-|
|-|---|

|buffer[6]|-|
|-|---|

|buffer[7]|-|
|-|---|

<br>

`Reader` gets a time slice:

|buffer[0]|A (readed)|
|-|---|

|buffer[1]|2 <- Reader_i|
|-|---|

|buffer[2]|#|
|-|---|

|buffer[3]|-  <- Writer_i|
|-|---|

|buffer[4]|-|
|-|---|

|buffer[5]|-|
|-|---|

|buffer[6]|-|
|-|---|

|buffer[7]|-|
|-|---|

`ProcessChar` is a little bit more time consuming. It actually has to like open a
file or a network connection or whatever it has to do, just pretend that it actually is slower so it only really consumes that `A`. 

I think it’s pretty clear that if the writer is able to make more progress per time slice than the reader, then there’s the danger that this might happen: `Writer` overwrites data that the `Reader` has yet to deal with. 

The idea that someone is providing content and someone else is digesting it, that’s not an unfamiliar one with large systems. It’s also in theory possible.
Just because I spawn off and set up `Writer` to be the first thing that runs and this the second thing that runs, it might be the case that the `Reader` gets the processor first. In which case, it will be digesting information that has never even been written or created 还没写，就开始读取.

<br>
<br>
<br>
<br>

### Reader Writer (Binary Rendezvous Pattern)

```C
char buffer[8];

Semaphore emptyBuffers(8);  /// 8 time to write
Semaphore fullBuffers(0);   /// nothing to read

int main() {
    /// false: not print any debug information
    limitThreadPackage(false);
    ThreadNew("Writer", Writer, 0);
    ThreadNew("Reader", Reader, 0);
    RunAllThreads();
    return 0;
}
```

I’m going to declare two semaphores here. I’m going to call one `emptyBuffers` and I’m going to call one `fullBuffers`. And, I’m going to let them actually manage integers that are almost always, but we’re going to pretend always, are always in sync with the **number of slots that can be written to and the number of slots that can be read from**. I also want to enforce that the `Writer` is also just a
little bit ahead of the `Reader` in terms of thread progress and that the `Reader` can get and catch up to the `Writer`, but it can’t pass them, and that the `Writer` can’t get so far ahead of the `Reader` that he actually is more than a cycle ahead of him.

<br>

```C
void Writer() {
    for (int Writer_i = 0; Writer_i < 40; Writer_i++) {
        char c = PrepareReadChar();
        semaphoreWait(emptyBuffers);  /// I Write 1
        buffer[Writer_i % 8] = c;
        semaphoreSignal(fullBuffers);  /// You can read 1
    }
}
```
For whatever reason and the `Writer` makes so much more progress than the `Reader` that he gets really far ahead, **this eight will have been demoted to a seven, to a six, to a four, to a two, to a one**, and it really is just about the clobbered data that has yet to be consumed没被读取的覆盖数据. It will be waiting on something that will have been demoted so many　times that it’s actually zero. So, it will be a victim of its own aggression and it will be blocked out and be pulled off the processor, so that the reader can actually do some work. 


The balance here is a `semaphoreSignal(fullBuffers);` call but it’s not against the same semaphore. After you write something down, you want to communicate to the `Reader` that there is **even one more piece of information that it’s allowed to consume**. 

<br>

```C
void Reader() {
    for (int Reader_i = 0; Reader_i < 40; Reader_i++) {
        semaphoreWait(fullBuffers);   /// I read 1
        char c = buffer[Reader_i % 8];
        semaphoreSignal(emptyBuffers);   /// You can write 1
        ProcessChar(c); /// Do not care this function
    }
}
```

The pattern over here `Reader()` is somewhat symmetric.
It’s like each thread has a little buzzer信号器. Each of them are twittering each other　as far as when they’re allowed to proceed to read or write information. 


`semaphoreSignal(fullBuffers); in Writer()` + `semaphoreWait(fullBuffers); in Reader()`is sending a little buzzer that allows that to execute and return with
much more likelihood. 


`semaphoreWait(emptyBuffers); in Writer()` + `semaphoreSignal(emptyBuffers); in Reader()` is really communicating to the thread at that point and promoting `fullBuffers` so that the `Writer` can actually write down more data, if `Writer` was　previously blocked (== 8 new writer data is not readed). 

<br>

So, imagine the scenario where the `Reader` actually gets the processor first. It’s going to execute this much. It’s going to declare `i`, it’s going to consider to zero. It’s going to pass the test. It’s going to come down here `semaphoreWait(fullBuffers);` and it’s going to be immediately blocked from this line right here
because it’s going to be waiting on something that is in fact 0 (`fullBuffers` has the value 0). So the `Reader` thread is actually being blocked right up front just like we want it to be. 

The other scenario is that the `Writer` thread really fast and very efficient, it
actually cycles through this thing **eight times** and then it hits a wall. Okay, so pair the character out before it actually went to bother on waiting on the lock, but – and it blocks here, it’s because it’s been a processor hog and it’s actually done a lot of work whereas the `Reader` hasn’t really been able to do much at all. Okay, or at least comparatively. 


The ticket agents example where it uses a `semaphoreWait` and a balance `semaphoreSignal` on exactly the same semaphore, and it brackets this thing called a critical region, that semaphore pattern or that semaphore is being used
as a binary lock. Okay, binary meaning it’s toggling between zero and one, true and false;

However we want to think about it. That’s not the pattern that’s being used here. We
certainly have thread communication. We use the semaphore for rudimentary thread
communication, okay, but right here what’s happening is we’re actually using these as
basically two telephone calls. Okay, between the two threads, okay, this one calls this one whenever it can make more progress. This one calls this one whenever the writer can make more progress. That is a pattern; it’s what called a rendezvous pattern. Like I’m syncing up with you. This is what’s called **binary rendezvous** which really just – one thread to one thread communication.

`emptyBuffer` basically say as this type of `semaphoreWait` call means I cannot make any progress until some other thread makes some required amount of progress in order for me to move forward. 

`fullBuffer` does the same thing on behalf of this semaphore. This says that I have to wait for some other thread that makes enough progress in order for me to
pass, or else the work I will be doing will be meaningless.

<br>
<br>

### Reader Writer (Binary Rendezvous Pattern) with `Semaphore emptyBuffers(4);`

```C
char buffer[8];

Semaphore emptyBuffers(4);  /// 4 time to write
Semaphore fullBuffers(0);   /// nothing to read

int main() {
    /// false: not print any debug information
    limitThreadPackage(false);
    ThreadNew("Writer", Writer, 0);
    ThreadNew("Reader", Reader, 0);
    RunAllThreads();
    return 0;
}
```


so what I want to do is I want to just experiment. What happens if I make that a
`4`: `Semaphore emptyBuffers(4); `? 

It doesn’t change the correctness of the code or it depends on how you define correctness, but you will not get deadlock. And you will not have any integrity
data issues, all you’re constraining is that the `Writer` and `Reader` stay within more of a delta of one another than they would have been able to otherwise. When it was an `8`, it **allowed some more degrees of freedom**. It allowed the `Writer` to go much further ahead if that’s just the way thread scheduling worked out. When I made it a `4`, it just means that the **`Writer` can be no more than half of an Internet ahead of the `Reader`**.

<br>
<br>

### Reader Writer (Binary Rendezvous Pattern) with `Semaphore emptyBuffers(1);`

```C
char buffer[8];

Semaphore emptyBuffers(1);  /// 1 time to write
Semaphore fullBuffers(0);   /// nothing to read

int main() {
    /// false: not print any debug information
    limitThreadPackage(false);
    ThreadNew("Writer", Writer, 0);
    ThreadNew("Reader", Reader, 0);
    RunAllThreads();
    return 0;
}
```

If I do that right there `Semaphore emptyBuffers(1); `, I’m really pushing the limits of what’s useful from a threading standpoint. I also will just **actually write the reading and writing in the same function and have it alternate between read and write**二元交替进行.

And really if it tries to like run forward two slots, it’ll be blocked by a `semaphoreWait` call.

<br>
<br>

### Reader Writer (Binary Rendezvous Pattern) with `Semaphore emptyBuffers(0);`

```C
char buffer[8];

Semaphore emptyBuffers(0);  /// 1 time to write
Semaphore fullBuffers(0);   /// nothing to read

int main() {
    /// false: not print any debug information
    limitThreadPackage(false);
    ThreadNew("Writer", Writer, 0);
    ThreadNew("Reader", Reader, 0);
    RunAllThreads();
    return 0;
}
```

If I do this `Semaphore emptyBuffers(0);` and I have a different form of `deadlock`, but deadlock is deadlock. I have a `Reader` saying I can’t do anything because I have no place to read from. The `Writer` says well, I can’t do anything because I have no place to write to.

So, if you have `deadlock`, everything seemingly stops. You get nothing
published to the console at all. It doesn’t return. You don’t get your command line
prompt back, so things just expand and then you go okay, that probably means that two
threads are waiting on each other. Or that nobody released a lock or something like that. 

<br>
<br>

### Reader Writer (Binary Rendezvous Pattern) with `Semaphore fullBuffers(1);`

```C
char buffer[8];

Semaphore emptyBuffers(8);  /// 8 time to write
Semaphore fullBuffers(1);   /// 1 time to read

int main() {
    /// false: not print any debug information
    limitThreadPackage(false);
    ThreadNew("Writer", Writer, 0);
    ThreadNew("Reader", Reader, 0);
    RunAllThreads();
    return 0;
}
```

```C
char buffer[8];

Semaphore emptyBuffers(7);  /// 7 time to write
Semaphore fullBuffers(1);   /// 1 time to read

int main() {
    /// false: not print any debug information
    limitThreadPackage(false);
    ThreadNew("Writer", Writer, 0);
    ThreadNew("Reader", Reader, 0);
    RunAllThreads();
    return 0;
}
```
But if you do that, that actually allows the `Reader` thread to get one hop ahead of the `Writer`. So that’s a kind of contrived example, but nonetheless that’s exactly what it will be permitted to do. It doesn’t mean it would actually happen, but it means programmatically it’s possible.

<br>
<br>

### Reader Writer (Binary Rendezvous Pattern) with `Semaphore emptyBuffers(16);`

```C
char buffer[8];

Semaphore emptyBuffers(16);  /// 16 time to write
Semaphore fullBuffers(0);   /// nothing to read

int main() {
    /// false: not print any debug information
    limitThreadPackage(false);
    ThreadNew("Writer", Writer, 0);
    ThreadNew("Reader", Reader, 0);
    RunAllThreads();
    return 0;
}
```



Another scenario: To put a `16` in `Semaphore emptyBuffers(16);` means that the `Writer` is allowed to make two loops and take two tracks or two loops on the `Reader` thread and that’s not allowed. It’s supposed to be at most eight slots ahead of, not 16 slots ahead of the `Reader`.


<br>
<br>


We usually try to **maximize throughput** and you choose your semaphore values accordingly (in this case `8`).


<br>
<br>
<br>
<br>
<br>

A lot of times when you are loading a web
page, the HTML is often being sourced from multiple servers. Okay, maybe the primary
HTML is being served from facebook.com, but all the images actually reside on some other servers that are residing elsewhere. And so all the information is pulled simultaneously.


Imagine the scenario where you have not one of these things right here but you have three `Writer`s and they’re all kind of competing to write and prepare the information
that’s actually sent over the wire to the single client. Well, then all of a sudden, you
would would still want to use this empty buffers thing but then you would
have to have an extra global, okay, or either that or something that is declared in main
and shared with all the writers, so that they can all agree on what the next insertion point
into the Internet is. And you have to
have some kind of binary lock in place to make sure that no one, no two threads try to
write to in this race condition matter to the same slot in the Internet or some piece of data will be lost.


In the scenario where you have multiple readers consuming the information, maybe it’s not really a web page but maybe it’s just content that’s being sent up from an FTP server and it’s being handled by threads
and that actually takes several different files simultaneously. You could use the same
exact type of thing for the readers. 

<br>
<br>
<br>
<br>
<br>

## Dining Philosophy Problem

![Dining_Philosophy_Problem](Pic/Dining_Philosophy_Problem.png)

![Dining_Philosophy_Problem](Pic/Dining_Philosophy_Problem_2.png)

Surrounding the table are five **philosophers**. And, in between each philosopher is a **fork** and every single philosopher follows the same **formula** every single day. They wake up, they think for a while and they eat and they think for a while and they eat and they think for a while and they eat, and of course they
think for a while before they go to bed.

Eat with two forks (left fork and right fork)

Two neighboring philosophers decide to stop thinking at the same time and will eat but I can tell you right now that two neighboring philosophers will never be eating at the same time, because they both demand the fork in between them to be in his hand, in their hands before they actually eat spaghetti.

<br>
<br>

### Dining Philosophy Problem 1.0 (Trivial)

```C
Semaphore forks[] = {1, 1, 1, 1, 1};  /// shorthand　缩写

void philosophers(int id) {
    for (int i = 0; i < 3; i++) {
        Think();
        semaphoreWait(forks[id]);
        semaphoreWait(forks[(id + 1) % 5]);
        Eat();
        semaphoreSignal(forks[id]);
        semaphoreSignal(forks[(id + 1) % 5]);
    }
    Think();
}
```

`philosophers0`: `Think()` and get `fork[0]`, then he swapped by processor

`philosophers1`: `Think()` and get `fork[1]`, then he swapped by processor

`philosophers2`: `Think()` and get `fork[2]`, then he swapped by processor

`philosophers3`: `Think()` and get `fork[3]`, then he swapped by processor

`philosophers4`: `Think()` and get `fork[4]`, then he swapped by processor

<br>

`philosophers0`: want get `fork[1]`, which is acquired by `philosophers1`

all the same ==> DEADLOCK

Everybody is holding a right fork that is somebody else’s left fork. You have this mutual deadlock among all five threads because every single one of them is waiting on some resource held by the philosopher’s to his left. 

Remember earlier I said I wanted to do the minimum amount of work to prevent deadlock? Okay, I want to implant the least amount of functionality to make something possible while still maximizing the amount of thread, amount of work that any particular thread
can do during its time slice. 


Well, I could actually make the whole `for-loop` a critical region and that would be really **rude** because that would require that at most one philosopher can eat at any one moment, and that in itself is rude because you’re not supposed to eat while others
cannot.

<br>

#### A Try:

I actually know that given that there are **five forks**. BUT ten forks need to be held in order for 5 philosophers to eat, I can tell that at most two philosophers are going to be allowed to eat at any one time (5 / 2 = 2). Because if three philosophers are eating, that requires six forks to be held and we just don’t have that many. 


Okay, we could also say that as long as we just
prevent one philosopher from grabbing any forks whatsoever, it’s technically possible to let four philosophers grab forks. Three of them may be blocked, but since I’m only allowing four philosophers to grab forks, exactly one of them will be able to grab two forks as opposed to one. 

Now, 
1. two being allowed to eat, 
2. allowing four to try to eat, 

There are **two different heuristic**s for solving the deadlock problem. I think both of them are very clear, are wiseheuristics to go with. 

You may think it’s really weird to allow four to try but all I’m trying to do is **remove the deadlock**. I technically will remove the deadlock if I limit the
number of philosophers trying to simultaneously eat, to not be five, but to be four.

Normally what you do is you **implant the minimum amount of work to remove the possibility of deadlock**. 

<br>
<br>

### Dining Philosophy Problem 2.0 (Try 1: two being allowed to eat)

```C
Semaphore forks[] = {1, 1, 1, 1, 1};  /// shorthand　缩写
Semaphore numAllowedToEat(2);

void philosophers(int id) {
    for (int i = 0; i < 3; i++) {
        Think();
        semaphoreWait(numAllowedToEat);

        semaphoreWait(forks[id]);
        semaphoreWait(forks[(id + 1) % 5]);
        Eat();
        semaphoreSignal(forks[id]);
        semaphoreSignal(forks[(id + 1) % 5]);

        semaphoreSignal(numAllowedToEat);
    }
    Think();
}
```

`semaphoreWait(numAllowedToEat);`: "Please wait for me to be **one of the two that is allowed to eat**."

Scoop of `numAllowedToEat` is critical region. It’s not the same type of critical region we saw before. Critical region normally means at most one because they are concurrency – there are race condition possibilities. This is a different type of critical region. In fact, most people wouldn’t call it a critical region but I’ll call it that. 


<br>
<br>

### Dining Philosophy Problem 3.0 (Try 2: allowing four to try to eat, )

```C
Semaphore forks[] = {1, 1, 1, 1, 1};  /// shorthand　缩写
Semaphore numAllowedToEat(4);

void philosophers(int id) {
    for (int i = 0; i < 3; i++) {
        Think();
        semaphoreWait(numAllowedToEat);

        semaphoreWait(forks[id]);
        semaphoreWait(forks[(id + 1) % 5]);
        Eat();
        semaphoreSignal(forks[id]);
        semaphoreSignal(forks[(id + 1) % 5]);

        semaphoreSignal(numAllowedToEat);
    }
    Think();
}
```

The reason the `4` works is because as long as somebody’s prevented from grabbing either fork then there’s **at least one `philosopher` thread that’s capable of grabbing two forks**. Maybe the other three or blocked. But it’s always the case that exactly – at least one philosopher will be able to grab two forks. 

That’s the minimum amount of a fix that I need to implant into the code to make sure I don’t have deadlock. So concurrency and multithreading purists usually like to do the
minimum amount of work to prevent deadlock. There is a reason for that, because the minimum amount that you implant there – you remove the deadlock but you still grant the thread manager as much flexibility in how they schedule threads as possible.

When I put a `2` there I’m taking more control over how threads are scheduled. That means up to three threads can block on this line right here as opposed to just one. 

If you make this a 4 that means that up to four threads can make as much progress as is really possible before they’re blocked and pulled off the processor, whereas if I make it a 2 we’re blunting some threads prematurely.
