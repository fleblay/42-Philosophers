# 42-Philosophers

Parallel/Concurrent Programming, Multi-threading, Interprocess Communication and Execution Context in C

Final Grade : [![fle-blay's 42 Philosophers Score](https://badge42.vercel.app/api/v2/clin3d2hs006008muostgzpop/project/2569230)](https://github.com/JaeSeoKim/badge42)

## Overview

This project is a version of the <a href="https://en.wikipedia.org/wiki/Dining_philosophers_problem">Dining Philosophers problem</a>.
A given number of philosophers sit around a table with a huge plate of pasta at the center. Each philosopher :
<ul>
	<li>
		May only be in one state at a time : thinking, eating or sleeping.
	</li>
	<li>
		Has only 1 fork, but needs 2 forks to eat (he will have to borrow one from either his left or right neighbor)
	</li>
	<li>
		The philosophers will follow this routine :
		<ul>
		<li>
			Grab 2 forks and start to eat for a given amount of time : tte (time to eat)
		</li>
		<li>
			Put down the 2 forks he was holding and sleep for a given amount of time : tts (time to sleep)
		</li>
		<li>
			Wake up and think if he has the time to
		</li>
		</ul>
	</li>
	<li>
		If a philosopher spends more than a given amount of time (parameter "time to die" or ttd) between the start of 2 meals, he will die starving
	</li>
</ul>

The goal is to try to have the philosophers live as long as possible. To do so, it is necessary to optimize the access to the forks. This is of course a metaphor :

<ul>
	<li>
		Each philosopher is a a thread (mandatory part) or a process (bonus part)...
	</li>
	<li>
		They share access to data (forks), but should not use it at the same time (avoid dataraces)
	</li>
	<li>
		They have to follow a pattern preventing from having everyone doing the same thing at the same time
		(synchronization with mutexes for mandatory part, mutexes AND semaphores for the bonus part)
	</li>
	<li>
		They must avoid at all cost being in a dead-end situation (deadlock). For example, if there are only 2
		philosophers and each of them grabs a fork, they will be locked in a situation where each of them tries to grab
		a 2nd fork to eat. After some time, both of them will die.
	</li>
</ul>

## Constraints

Clean code :
<ul>
  <li>
  No dataraces
  </li>
  <li>
  No memory leaks
  </li>
  <li>
  No zombie process
  </li>
</ul>

No communication between philosophers, ie one philosopher cannot ask information about the state of another, nor tell
the others philosophers that he is about to die.
<br/>
The program needs to shutdown nicely in case of error (thread creation failure, process creation failure).
<br/>

## Implemented Solution

For both of the project, the allowed functions did not include the "try" version (pthread_mutex_trylock and
sem_trywait). Moreover, for the bonus part, we couldn't use the function to get the value of the semaphore
(sem_getvalue).

<br/>

This increases the difficulty of the projet because :
<ul>
	<li>
		Mandatory part :
		<ul>
			<li>
				You cannot try to acquire access data (pthread_mutex_trylock), and change your mind. Once you demand access to it, you will
				remain blocked until it is available...
			</li>
			<li>
				But you can use a shared memory address (pointer) between the philosophers so they can write/read info about themselves/others. Threads only have their own stack and context. They do share the heap.
			</li>
		</ul>
	</li>
	<li>
		Bonus part :
		<ul>
			<li>
				You still cannot try to acquire access data (sem_trywait), and change your mind. Once you demand access to it, you will
				remain blocked until it is available...
			</li>
			<li>
				You CANNOT use a shared memory address (pointer) between the philosophers.
			</li>
			<li>
				You also CANNOT use the semaphore to share data (sem_getvalue)
			</li>
		</ul>
	</li>
</ul>

This led to the following :

Mandatory part :

<ul>
  <li>Each philosopher is a thread spawned by the main thread</li>
  <li>After spawning the last philosopher, the main thread become a monitoring thread</li>
  <li>When a philosopher dies or the meal count is achieved, the monitoring thread write this info in a memory area
  shared between all the threds so they can stop</li>
</ul>

Bonus part :

<ul>
  <li>Each philosopher is child process spawned by the main process</li>
  <li>After spawning the last philosopher, the main thread become a monitoring process</li>
  shared between all the threds so they can stop</li>
  <li>
  	Each philosopher process will spawn 2 threads :
	<ul>
		<li>A meal_goal_monitor deamon in charge of checking if the all the philosophers have eaten the requested amount of time</li>
		<li>A dead_monitor deamon in charge of checking if one of the philosophers died starving</li>
	</ul>
	Each of theses processes are in a blocked state (sem_wait) and can be unlocked by the monitoring process. Doing so
	will terminate the other deamon before rejoining the main thread and exiting nicely
  </li>
</ul>

### Schematics and Diagrams

## Usage

### Dependencies

First and foremost, make sure you have the following packages installed on your machine :

<ul>
  <li>Docker</li>
  <li>Make</li>
</ul>

### Install

Clone the repo on your machine and go to project directory :

```console
  git clone https://github.com/fleblay/42-Philosophers && cd !#:2:t
```

Go to the mandatory...:

```console
  cd philo
```

Or bonus part...:

```console
  cd philo_bonus
```

Build the executable:

```console
  make
```

Run the program with the following syntax :

```console
  ./philo number_of_philosophers ttd tte tts [number_of_times_each_philosopher_must_eat]
```
Or this one for the bonus part :

```console
  ./philo_bonus number_of_philosophers ttd tte tts [number_of_times_each_philosopher_must_eat]
```

The last parameter is optional : if all philosophers have eaten at least number_of_times_each_philosopher_must_eat times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

## ROX (Return On Experience)

### Knowledge acquired during this project

<ul>
  <li>
  	Concurrent Programming using POSIX Threads. Mutual Exclusion concept (mutexes) and deadlock.
  </li>
  <li>
  	Concurrent Programming using process. Interprocess Communication (IPC) unsing semaphores (signal and files)
  </li>
</ul>
