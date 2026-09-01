*This project has been created as part of the 42 curriculum by mbahri.*

# Codexion

## Description

Codexion is a multithreaded scheduling and synchronization project inspired by the Dining Philosophers problem.

Each coder is represented by a thread. Coders repeatedly try to compile, debug, and refactor. In order to compile, a coder must acquire the two dongles adjacent to it. Dongles are shared resources and may only be owned by one coder at a time.

The project adds scheduling constraints on top of the classical shared-resource problem:

- requests for a dongle are managed by a custom priority queue;
- the scheduler can use FIFO or EDF ordering;
- EDF prioritizes the coder whose burnout deadline is closest;
- dongles may enter a cooldown period after being released;
- a dedicated monitor thread detects coder burnout;
- all logs are serialized so that output lines never overlap;
- the simulation stops when a coder burns out or when every coder has reached the required number of compiles.

The project focuses on thread synchronization, deadlock avoidance, starvation prevention, deterministic scheduling, timing, resource ownership, and safe program shutdown.

## Instructions

### Compilation

Compile the project with:

```sh
make
```

The project is compiled with:

```text
-Wall -Wextra -Werror -pthread
```

The generated executable is:

```text
codexion
```

Useful Makefile rules:

```sh
make
make clean
make fclean
make re
```

### Execution

The program takes exactly eight arguments:

```sh
./codexion number_of_coders time_to_burnout time_to_compile \
    time_to_debug time_to_refactor number_of_compiles_required \
    dongle_cooldown scheduler
```

Arguments:

- `number_of_coders`: number of coder threads and dongles.
- `time_to_burnout`: maximum time in milliseconds between compile starts before a coder burns out.
- `time_to_compile`: compile duration in milliseconds.
- `time_to_debug`: debug duration in milliseconds.
- `time_to_refactor`: refactor duration in milliseconds.
- `number_of_compiles_required`: minimum number of compiles every coder must complete. It must be at least `1`.
- `dongle_cooldown`: time in milliseconds during which a released dongle remains unavailable.
- `scheduler`: either `fifo` or `edf`.

Example:

```sh
./codexion 5 2000 200 200 200 10 0 fifo
```

### Scheduler behavior

#### FIFO

FIFO grants a waiting request according to arrival order. Older requests have higher priority.

#### EDF

EDF means Earliest Deadline First. A coder's scheduling deadline is based on:

```text
last_compile_start + time_to_burnout
```

The request with the earliest deadline has higher priority. A deterministic sequence number is used as a tie-breaker when deadlines are equal.

### Reference tests

Examples used during validation include:

```sh
./codexion 1 800 200 200 200 10 0 fifo
./codexion 5 2000 200 200 200 10 0 fifo
./codexion 5 2000 200 200 200 7 0 edf
./codexion 5 500 200 200 200 10 0 fifo
./codexion 5 3000 200 200 200 10 400 fifo
./codexion 5 3000 200 200 200 10 800 fifo
./codexion 5 3000 200 200 200 10 800 edf
```

Concurrency can also be checked with tools such as:

```sh
valgrind --tool=helgrind ./codexion 5 2000 200 200 200 3 0 fifo
valgrind --tool=drd ./codexion 5 2000 200 200 200 3 0 fifo
```

Memory can be checked with:

```sh
valgrind --leak-check=full --show-leak-kinds=all \
    ./codexion 5 2000 200 200 200 3 0 fifo
```

## Blocking cases handled

The implementation explicitly addresses the main concurrency and shutdown cases required by the project.

### Shared dongle ownership

Every dongle has its own mutex. Its mutable state, including ownership, cooldown information, request queue, and related scheduling data, is accessed under synchronization so that the same physical dongle cannot be owned by two coders at the same time.

### Two-dongle acquisition

Compilation is allowed only after both required dongles have been granted to the coder. Pair acquisition is coordinated so that a coder does not begin compiling with only one valid resource.

### Single-coder case

With one coder, both logical sides refer to the same physical dongle. The coder can take the single available dongle but can never obtain two distinct dongles, so it cannot compile and eventually burns out.

### Scheduler contention

Each dongle request participates in the custom priority-queue scheduler. FIFO uses request arrival order, while EDF uses the coder's burnout deadline and a deterministic sequence tie-breaker.

### Initial equal EDF deadlines

All coders begin from one common simulation start time. Initial request ordering is handled deterministically so that equal initial EDF deadlines do not create an avoidable startup convoy.

### Dongle cooldown

When dongles are released, their next available time is updated. A coder cannot acquire a dongle before its cooldown deadline has elapsed.

### Burnout detection

A separate monitor thread continuously checks active coders. A coder is considered burned out when the current time reaches its deadline before a new compile starts.

Coders that have already reached the required number of compiles are no longer considered candidates for burnout.

### Terminal burnout logging

Burnout is treated as a terminal event. Logging and simulation-stop synchronization are coordinated so that the `burned out` line is the last line printed.

### Serialized logging

All output is protected by a dedicated log mutex. The two `has taken a dongle` messages associated with a successful pair acquisition are printed together so that another coder cannot interleave an unrelated state line between them.

### Simulation startup barrier

Coder threads and the monitor wait for a common start signal. The simulation start timestamp and every coder's initial `last_compile_start` are initialized consistently before runtime work begins.

This prevents thread-creation time from unfairly consuming a coder's burnout window.

### Clean shutdown

When the simulation stops, sleeping threads are notified through a shared condition variable. Threads are joined before mutexes, condition variables, coder arrays, dongle arrays, and priority-queue storage are destroyed.

### Partial initialization and thread-creation failure

Initialization functions clean only resources that were successfully initialized. If thread creation fails after some coder threads already exist, the created threads are stopped and joined before final cleanup.

### Request lifetime

Heap entries store pointers to request objects. A request is removed from its queue before the owning function returns, preventing queues from retaining pointers to expired stack objects.

## Thread synchronization mechanisms

### `pthread_mutex_t`

Mutexes are used to protect shared mutable state.

- Each dongle has its own mutex protecting its ownership, cooldown, and request queue.
- Each coder has a state mutex protecting fields such as `last_compile_start` and `compile_count`.
- The simulation state mutex protects global simulation state such as the stop/start state and scheduler-wide sequencing data.
- The log mutex serializes terminal output.

When two dongle mutexes must be held together, they are always locked in a deterministic order and unlocked in reverse order to avoid lock-order deadlocks.

### `pthread_cond_t`

A simulation condition variable is used for coordinated startup, interruptible phase waiting, and clean shutdown.

`pthread_cond_wait` and `pthread_cond_timedwait` are always used with the mutex associated with the protected simulation state.

Dongles also have condition-variable infrastructure available for resource-state coordination.

### Threads

- One thread is created for every coder.
- A separate monitor thread handles burnout and global completion detection.
- All created threads are joined before shared synchronization objects are destroyed.

### Custom priority queue

Each dongle owns a custom heap of requests.

The heap uses one centralized priority comparison:

- FIFO: earlier sequence has higher priority.
- EDF: earlier deadline has higher priority; sequence resolves ties deterministically.

This also keeps the scheduler easy to modify during the recode exercise. Turning FIFO into LIFO only requires reversing the sequence comparison in the scheduler logic rather than changing the heap implementation or faking output order.

## Resources

Resources useful for this project include:

- POSIX threads documentation for `pthread_create`, `pthread_join`, mutexes, and condition variables.
- Binary heap / priority queue references for `push`, `pop`, sift-up, sift-down, and arbitrary request removal.

### Use of AI

AI tools were used as a discussion and review aid during development. They were used to:

- discuss concurrency architecture and synchronization choices;
- suggest test scenarios and help interpret Helgrind/DRD results;

The implementation was developed and tested as part of the project, with concurrency behavior validated through runtime tests and thread-analysis tools.
