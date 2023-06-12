# 42-Philosophers

Parallel/Concurrent Programming, Multi-threading, Interprocess Communication and Execution Context in C

Final Grade : [![fle-blay's 42 Philosophers Score](https://badge42.vercel.app/api/v2/clin3d2hs006008muostgzpop/project/2569230)](https://github.com/JaeSeoKim/badge42)

## Overview

This project is a version of the <a href="https://en.wikipedia.org/wiki/Dining_philosophers_problem">Dining Philosophers problem</a>
A given number of philosophers sit around a table with a huge plate of pasta at the center.
Each philosopher :
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
		If a philosophers spends more than a given amout of time between the start of 2 meals, he will die starving : ttd (time to die)
	</li>
</ul>

The goal is to try to have the philosophers live as long as possible. To do so, it is necessary to optimize the access to the forks
This is of course a metaphor :

<ul>
	<li>
		Each philosopher is a a thread (mandatory part) or a process (bonus part)...
	</li
	<li>
		They share access to data (forks)
	</li
	<li>
		They have to follow a pattern preventing from having everyone doing the same thing at the same time
		(synchronization with mutexes for mandatory part, mutexes and semaphores for the bonus part)
	</li
	<li>
		They have must avoid at all cost being in a dead-end situation (deadlock). For example, if there are only 2
		philosophers and each of them grabs a fork, they will be locked in a situation where each of them tries to grab
		a 2nd fork to eat. After ttd, both of them will die.
	</li
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

## Implemented Solution

Mandatory part :

Bonus part :

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
OR

```console
  ./philo_bonus number_of_philosophers ttd tte tts [number_of_times_each_philosopher_must_eat]
```

The last parameter is optional :
If all philosophers have eaten at least number_of_times_each_philosopher_must_eat times, the simulation stops.
If not specified, the simulation stops when a philosopher dies.

## ROX (Return On Experience)

### Knowledge acquired during this project

<ul>
  <li>
	Concurrent programming
  </li>
</ul>

### Challenges faced

<ul>
  <li>
	Bonus much more difficult
  </li>
</ul>
