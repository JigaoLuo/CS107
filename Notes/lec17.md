## FTP

FTP downloads. I know FTP is kind of this 1990’s technology, but we all still use it. We actually go and fetch files. We actually use programs that use FTP, but let me just assume the existence of this function.

```C
int DownloadSingleFile(const char* server, const char* path);
```

`server` is basically about the computer that’s
**hosting the file that you’re trying to get** and `path` is basically relative to where the web directory or the FTP server is behind the scenes, how to get to the particular file of interest想要的文件.

```C
int DownloadAllFiles(const char* server, const char* files[], int numFiles);
```
I want it to return the total number of bytes that were downloaded as basically the sum of all the
sizes of the files that are downloaded. I’ll call it download all files. I’m gonna assume all the files are hosted on the same server.

What you’d rather do is you’re rather spawn off `numFiles`
different threads. `numFiles` threads where each one is dedicated to pulling over one of these files. So I’m gonna assume that run all threads has already been called and this is running as part of some thread that was spawned in the main function, okay. So I’m already dealing with a multithreaded environment. 

<br>

### `DownloadAllFiles` 1.0

```C
int DownloadAllFiles(const char* server, const char* files[], int numFiles) {
    int totalBytes = 0;
    Semaphore lock = 1;  /// shorthand　缩写

    for (int i = 0; i < numFiles; i++) {
        ThreadNew(-, DownloadHelper /*proxy pattern, muss have void as return type*/, 4, server, files[i], &totalBytes, lock);
    }

    return totalBytes;
}
```

You’re not necessarily sure how that’s updated yet but you can be sure that each of the `numFiles` threads that I’m gonna spawn off, to download a single file is gonna somehow do a plus
equals against `totalBytes` right here. So what has to happen is that I have to spawn off `numFiles` threads in addition to passing the `server` and one of the file names `files` to the
thread, so that the thread can call this function `DownloadSingleFile`.
I also want to pass the address of this integer `totalBytes`.
And a `lock` that’s designed to protect access to this because you’re gonna have several threads trying to plus equals potentially at the same time.

```C
void DownloadHelper(const char* server, const char* path, int* totalBytes, Semaphore lock) {
    int bytesDownloaded ＝ DownloadSingleFile(server, path);
    semaphoreWait(lock);
    totalBytes += bytesDownloaded;
    semaphoreSignal(lock);
}
```

The reason you catch the return value of `DownloadSingleFile()` is because after you let `DownloadSingleFile()` run and do its work, as a **side effect** of this function you’re just supposed to assume it’s on file and full appeared on your host machine假设文件已经下载到主机上了.

But afterwards what you want to do is you want to `semaphoreWait(lock);` on the lock and all the other threads are quite possibly waiting on because once you acquire that lock you are the blessed thread that’s allowed to plus equals against the single integer that you have a pointer to.

And so what I really want to happen is this is a child thread of `main thread` to really be spawning grandchildren threads. If run all threads is already – has already been called and
this is running, as soon as this is called it actually sets this up for all – or all `numFiles` of these up
on the ready queue immediately so that they start running immediately.

<br>

- Problem of `DownloadAllFiles`:  

The way this is technically coded up at the moment it says, “Okay, I’m gonna declare a local variable. I’m gonna share a lock with all of my underlings over here so that they all have atomic synchronized access to the shared global right here.”

And I spawn them off and then I return immediately, with all spawned thread unfinished. It’s quite possible that I would
return a `0` because I may advance from the very last iteration of the for loop to this `return` statement. Before any one of these spawned threads makes any partial progress whatsoever. Presumably this `DownloadHelper` is a time consuming function, like seconds. It would take seconds for it to advance from the very last thread to `return`. 

It’s quite possible that maybe even one or two time slices all of these threads could be spawned off and it could `return` before spawned threads makes any work whatsoever.

So what really has to happen is right there I really need the manager thread, the download all files thread, to really block and not go anywhere and certainly not advance to the
`return` statement until it has verification that all of these guys have completed downloading. Because if they’ve completed then the manager knows that this thing has really been built up to have the true return value `totalBytes`. 



<br>

### `DownloadAllFiles` 2.0

```C
int DownloadAllFiles(const char* server, const char* files[], int numFiles) {
    Semaphore childrenDone = 0;  /// shorthand　缩写

    int totalBytes = 0;
    Semaphore lock = 1;  /// shorthand　缩写

    for (int i = 0; i < numFiles; i++) {
        ThreadNew(-, DownloadHelper /*proxy pattern, muss have void as return type*/, 5, server, files[i], &totalBytes, lock, childrenDone);
    }

    for (int i = 0; i < numFiles; i++) {
        semaphoreWait(childrenDone);
    }

    return totalBytes;
}
```

Well, not surprisingly you use concurrency and you use
`semaphore`s. So basically **implant thread communication植入线程通信**, **thread-to-thread communication**. 

The **reader-writer** example we had the reader and the writer
communicating in this **binary rendezvous** way. When I do this I’m talking about the crisscross of semaphore signal and semaphore wait calls so that each one can tell the other one that there was an empty buffer or a full buffer.

`Semaphore childrenDone = 0;`: So what I’m basically doing is I’m giving, like, it’s almost like a little baby monitor to each of the threads, okay, that I’m spawning off. And when each one of them is done they
go, “Wah,” into the baby monitor in the form of a semaphore signal and when I hear `numFiles` of those. I know that all of the threads have completed.

```C
void DownloadHelper(const char* server, const char* path, int* totalBytes, Semaphore lock, Semaphore parentToSignal) {
    int bytesDownloaded ＝ DownloadSingleFile(server, path);
    semaphoreWait(lock);
    totalBytes += bytesDownloaded;
    semaphoreSignal(lock);
    semaphoreSignal(parentToSignal);
}
```


`semaphoreSignal(parentToSignal);` is the “wah” into the baby monitor and this thing is actually aggressively for looping.
Programmatically each thread signals this thing exactly one time. So I expect this thing to be signaled exactly `numFiles` times. I need all `numFiles` of those signals in order for this thread to know that it’s done. 