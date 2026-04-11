# Synchronous vs Asynchronous

______________________________________________________________________

**Date:** 2026-04-11
**Tags:**[Udemy.md](tags/Udemy.md), [Fundamentals_of_backend_engineering.md](tags/Fundamentals_of_backend_engineering.md), [Backend_communication_design_patterns.md](tags/Backend_communication_design_patterns.md), [Synchronous.md](tags/Synchronous.md), [Asynchronous.md](tags/Asynchronous.md)
**URL:**

______________________________________________________________________

## Synchronous I/O

- Caller sends a request and blocks
- Caller cannot execute any code meanwhile
- Receiver responds, Caller unblocks and continues execution
- Caller and Receiver are in "sync"

### Example of an OS synchronous I/O

- Program asks OS to read from disk
- Program main thread is taken off of the CPU
- Read completes, program can resume execution

```js
// Program starts
// Program uses CPU to execute stuff
doWork();
// Program reads from disk
// program cant do anything until file loads
readFile("largefile.dat");
// program resumes execution
doWork2();
```

## Asynchronous I/O

- Caller sends a request
- Caller can work until it gets a response
- Caller either:
  - Checks if the response is ready (epoll)
  - Receiver calls back when its done
    (io_uring)
  - Spins up a new thread that blocks
- Caller and receiver are not necessarily in "sync"

### Example of an OS asynchronous I/O

- Program spins up a secondary thread
- Secondary thread reads from disk, OS blocks it
- Main program still running and executing code
- Thread finish reading and calls back main thread

```js
// Program starts
// Program uses CPU to execute stuff
doWork();
// Program requests read from disk
// program asks to callback when file loads
// Program moves on to doWork2
readFile("largefile.dat", onReadFinished(theFile));
// file is probably not read yet
// Program happy doing stuff
doWork2();
// someone just called onReadFinished, file is ready
// processing it.
-----> onReadFinished(theFile) 
```

## Synchronous vs Asynchronous in Request Response

- Synchronicity is a client property
- Most modern client libraries are asynchronous
- E.g. Clients send a http request and do work

## Synchronous vs Asynchronous in real life

- Just in case it is still confusing
- In Synchronous communication the caller waits for a response from receiver, ex: asking someone a question and waiting for an answer
- Asynchronous communication the response can come whenever. ex: sending an email and waiting for a response, you can do other things meanwhile

## Async workload is everywhere

- Asynchrnous programming (promises/futures)
- Asynchronous backend processing
- Asynchronous commits in postgres
- Asynchronous IO in linux (epoll/io_uring)
- Asynchronous replication in databases
- Asynchronous OS fsync (fs cache)

## Demo nodejs

[./code/js/node-sync-async]
