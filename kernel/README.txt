Lab : Locks

The common symptom of poor parallelism on multi-core machines is high lock contention. Improving parallelism often involves changing both data structures and locking strategies in order to reduce contention. We'll do this for the xv6 memory allocator.

  $ git fetch
  $ git checkout lock

Memory allocator 

The program user/kalloctest stresses xv6's memory allocator: three processes grow and shrink their address spaces, resulting in many calls to kalloc and kfree. kalloc and kfree obtain kmem.lock.
kalloctest prints the number of test-and-sets that did not succeed in acquiring the kmem lock (and some other locks), which is a rough measure of contention.

The root cause of lock contention in kalloctest is that kalloc() has a single free list, protected by a single lock. To remove lock contention, we will have to redesign the memory allocator to avoid a single lock and list. The basic idea is to maintain a free list per CPU, each list with its own lock. Allocations and frees on different CPUs can run in parallel, because each CPU will operate on a different list. The main challenge will be to deal with the case in which one CPU's free list is empty, but another CPU's list has free memory; in that case, the one CPU must "steal" part of the other CPU's free list. Stealing may introduce lock contention, but that will hopefully be infrequent. 

OBJECTIVE : is to implement per-CPU freelists and stealing when one CPU's list is empty. Running kalloctest to see if our implementation has reduced lock contention, and to check that it can still allocate all of memory and also to make sure usertests still passes. 

Steps taken to implement:

1. We have given all of our locks initlock names that start with "kmem".
2. Use of the constant NCPU from kernel/param.h.
3. Setting the freerange so that it gives all free memory to the CPU running freerange.
4. The function cpuid returns the current core number, but it's only safe to call it and use its result when interrupts are turned off. Hence we should use push_off() and pop_off() to turn interrupts off and on. 

Output:

$ kalloctest
start test0
test0 results:
=== lock kmem/bcache stats
lock: kmem: #test-and-set 0 #acquire() 33167
lock: kmem: #test-and-set 0 #acquire() 200114
lock: kmem: #test-and-set 0 #acquire() 199752
lock: bcache: #test-and-set 0 #acquire() 28
=== top 5 contended locks:
lock: proc: #test-and-set 22303 #acquire() 180082
lock: proc: #test-and-set 4162 #acquire() 180130
lock: proc: #test-and-set 4115 #acquire() 180129
lock: proc: #test-and-set 342 #acquire() 180070
lock: proc: #test-and-set 39 #acquire() 180070
test0 OK
start test1
total allocated number of pages: 200000 (out of 32768)
test1 OK
$
$ usertests
...
ALL TESTS PASSED
$
