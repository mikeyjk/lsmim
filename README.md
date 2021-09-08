lsmim
=====

Small C program that imitates 'ls' from GNU Core Utilities.

Takes 1 directory over cli or the current working directory is assumed.
1 flag option currently '-s' which provides a simplified listing format.

It is fairly verbose without -s, so as to meet the requirements of an ICT310 project.

to do:
- break program into smaller chunks
- create unit test for use with failtest

Finally added some unit testing, using the 'criterion' library

https://github.com/Snaipe/Criterion

Notes for criterion on debian, currently not supported via apt, so instead:
- download the latest release
- bzip2 -d *
- tar -xvf *
- preserve symlinks/copy criterion/lib into /usr/lib
- copy criterion/include/criterion to /usr/include
