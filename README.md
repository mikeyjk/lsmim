lsmim
=====

I want to completely re-write this to be ridiculously modular.
Debug programs to print out specific structs?
Going to use ccan/failtest as a dependency.

to do: 
- break program into smaller chunks
- create unit test for use with failtest

 Small C program that imitates 'ls' from GNU Core Utilities.

Takes 1 directory over cli or the current working directory is assumed.
1 flag option currently '-s' which provides a simplified listing format.

It is fairly verbose without -s, so as to meet the requirements of an ICT310 project.

TODO: It'd be good to use the CSPAN C testing library to test for memory leaks etc. Saw an interesting presentation from Rusty at Linux Conference 2014 regarding this library.
