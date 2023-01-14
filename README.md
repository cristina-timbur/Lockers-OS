# :closed_lock_with_key: Lockers-OS

## :bookmark: Table of Contents

- [About](#about)
- [Usage](#usage)
- [Exampes](#examples)
  - [Resources Counter](#resources-counter)
  - [Barrier](#barrier)
  - [Reader-Writer Problem (starvation-free solution)](#rw-locker)
  - [Reader-Writer Problem (priority for writers solution)](#rw-locker-priority)
  - [Producer-Consumer Problem (finite buffer)](#producer-consumer)
  - [Dining Philosophers Problem](#dining-philosophers)
- [Bibliography](#bibliography)
  
<a name="about"/>

## :rocket: About

Lockers-OS is a C library with synchronization tools for threads. It implements basic objects such as mutexes, semaphores and reader-writer lockers. It also comes up with soltions for common sychronization problems like <i>Dining Philosophers Problem</i> or <i>Producer-Consumer Problem</i>.

<a name="usage"/>

## :gear: Usage (UNIX like systems)

Add `lib` directory to the root of your project, then inside it run the following command to build the library:

```bash
make
```
This will generate an archive file called `liblockers.a`

The libray header file is located under `lib/include/lockers.h`. Take this into consideration when using it. 

In the end, compile your sorce code telling the compiler to use the symbols from <i>lockers</i> libray.

#### Example

```bash
gcc <program_name.c> -L. -llockers -o <executable_name>
```

<b>Note:</b> in order to compile the sources from `tests` folder it's enough to run the `make` command inside it. This will generate all the executables inside the `tests/bin` folder.

<a name="examples"/>

## :flying_saucer: Examples

<a name="resources-counter"/>

### Resources Counter

#### Problem

Write a program that handles access to a finite number of resources. Multiple threads can concurrently request a part of the resorces which will be given back once the thread doesn't need them anymore. If a thread requests more resources than there are available, it will have to wait until it can gat all the needed resources. For simplicity, the resources are represented by an integer, the request resources operation is reduced to decreasing the availble resources with a whole number and the retrieving resources operation is represented by increasing the available resorces with the same number that the thred used in its request.

#### Run Our Solution

```bash
./tests/bin/count 3 4 2 5 1 3 4 5 2 3 4 5 2 3
```

#### Sample

![os_count](https://user-images.githubusercontent.com/93842197/210456427-6e60e0aa-cfd1-4cb6-9ce5-9965f748a0a3.png)

<a name="barrier"/>

### Barrier

#### Problem

Implement a Barrier. As a reminder, a barrier in a multithreaded program doesnt't let some threads enter their critical section until all threads (or only a couple of them) have reached a specific point in their execution (called barrier point). It is the generalization of Rendezvous problem, where two threads can't continue theier execution until both of them have reached to some point (rendezvous point).

#### Run Our Solution

``` bash
./tests/bin/barrier
```

#### Sample

![os_barrier](https://user-images.githubusercontent.com/93842197/210457013-d227d56c-d5f9-441e-9284-42f62d0743a0.png)

<a name="rw-locker"/>

### Reader-Writer Problem (starvation-free solution)

#### Problem

A data structure, database or file system can be read and modified by conccurrent threads. While the data structure is read, there's no need to bar other threads from reading it, but when the data structure is being modified, no other threads can read or modify it the same time (or else, reader threads may get inconsistent data and writer threads may interrupt the modification in profgress). To put it more formally, any number of reader threads may be in the critical section at the same time, but writers must have exclusive access to their critical section.

#### Run Our Solution

``` bash
./tests/bin/readers-writers r r r r r w w r w r w r r w w r r w 
```

#### Sample

![os_readers-writers](https://user-images.githubusercontent.com/93842197/210458210-1d889d36-0dff-4d22-8368-1fd0bb20a42d.png)

<a name="rw-locker-priority"/>

### Reader-Writer Problem (priority for writers solution)

#### Problem

Same problem as before, but this time writers have priority. In other words, once a writer arrrives, no readers are allowed to enter their critical section until all witers have left. Note that if too many writers, readers may starve.

#### Run Our Solution

``` bash
./tests/bin/readers-writers-priority r r w w w w w w w w r w w w 
```

#### Sample

![os_readers-writers-priority](https://user-images.githubusercontent.com/93842197/210459387-072ff507-5a17-4902-b72e-6170922a664a.png)

<a name="producer-consumer"/>

### Producer-Cosnumer Problem (finite buffer)

#### Problem 

Some threads are producers and some threads are consumers. Producers create items of some kind and add them to a data structure. Consumers remove the items and process them.

#### Run Our Solution

``` bash
./tests/bin/producer-consumer p c c p p p c p p p p p p p c c c c c c c p p p p c c c c c
```

#### Sample

![os_producer-consumer](https://user-images.githubusercontent.com/93842197/210472341-ff552a38-6077-42cf-af2e-b8640645ff31.png)

<a name="dining-philosophers"/>

### Dining Philosophers Problem

#### Problem

We have a table with five plates, five forks and a big bowl of spaghetti. Five philosophers, who represent interacting threads, come to the table and execute the following loop: 

```bash
while(true){
  think()
  get_forks()
  eat()
  put_forks()
}
```

The forks represent resources that the threads have to hold exclusively in order to make progress. The philosophers need two forks to eat, the one at their left and the one at their right, so a philosopher might wait for a neighbour to put down a fork.

#### Run Our Solution

``` bash
./tests/bin/dining-philosophers
```

#### Sample 

![os_dining-philosophers](https://user-images.githubusercontent.com/93842197/210473057-cbed8399-928c-4c63-af20-8d061f95bbb3.png)

<a name="bibliography"/>

## :book: Bibliography

- <i>The Little Book of Semaphores</i> \- Allen B. Downey <br/>
- <i>Operating System Concepts</i> \- Avi Silberschatz, Greg Gagne and Peter Baer Galvin

## Copyright © 2022-2023

<p><a href="https://github.com/cristina-timbur">@cristina-timbur</a><a> &nbsp;</a><a href="https://github.com/vl4dio4n">@vl4dio4n</a></p>

***
*<p align="center"><a>FMI UniBuc 2022-2023</a></p>*

<p align="right">(<a href="#top">Back to Top</a>)</p>
