*This project has been created as part of the 42 curriculum by afomin.*

# Description
**Codexion** is a systems-level programming project focused on multi-threading and concurrency. The goal is to simulate a software development environment where multiple "coders" work on a shared codebase. Each coder must compile, debug, and refactor code while managing limited resources (dongles) and avoiding "burnout" (starvation). The simulation requires precise synchronization to ensure thread safety, prevent deadlocks, and maintain accurate real-time monitoring of all coder states.

# Instructions

### Compilation
The project uses a `Makefile` and must be compiled with `cc` and standard flags (`-Wall -Wextra -Werror`).
```bash
make
```

### Execution
Run the executable with the following parameters:
```bash
./codexion [num_coders] [time_to_burnout] [time_to_compile] [time_to_debug] [time_to_refactor] [num_compiles_required] [dongle_cooldown] [scheduler]
```
*Example:*
```bash
./codexion 4 800 200 200 200 5 30 edf
```

# Resources
- **Documentation:** `man pthread`, `man pthread_mutex_init`, `man pthread_cond_wait`.
- **Articles:** https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html
- **AI Usage:** AI was used to assist in debugging complex memory errors identified by Valgrind, explaining the nuances of `pthread_cond_broadcast` vs `signal`, and structuring the logic for thread-safe status checks (`is_failed`).

# Blocking cases handled
- **Deadlock Prevention:** Handled by enforcing a strict resource acquisition order and ensuring that mutexes are always released before a thread exits or waits.
- **Precise Burnout Detection:** A dedicated monitor thread checks the time elapsed since the last successful "compile" or "refactor" start to ensure millisecond-level accuracy.
- **Log Serialization:** Access to the standard output is protected by a `logger_mutex` to prevent "interleaving" (scrambled text) where messages from different threads overlap.

# Thread synchronization mechanisms
- **`pthread_mutex_t`**: Used to protect shared variables like the `failed` flag, the `logger`, and individual dongle states.
- **`pthread_cond_t`**: Employed to put threads into a sleep state when dongles are unavailable, reducing CPU usage compared to "busy-waiting."
- **Monitor-Coder Communication**: The monitor thread updates a shared `failed` state under a mutex. Coders check this state using a thread-safe `is_failed()` function at the start of every action. 

### Race Condition Prevention Example:
To prevent two coders from grabbing the same dongle simultaneously, the acquisition logic is wrapped in a mutex:
```c
pthread_mutex_lock(&table->dongle_mutex);
// Check and take dongle
pthread_mutex_unlock(&table->dongle_mutex);
```
Thread-safe termination is achieved by using `pthread_cond_broadcast` when a burnout occurs, ensuring all sleeping threads wake up, check the `failed` flag, and exit cleanly.