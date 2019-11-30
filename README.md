# libpqxx-mem-overrun

The short example to investigate the following issue.
The libpqxx library in multithreaded use cases does not
enough free memory allocated for results of the query.

libpqxx was built from sources (master branch) using 
               gcc -std=c++17 -O2
(gcc version 9.2.0)


# Prepare test database

To run test you should prepare test database before.
Please review SQL scripts in "sql" subdirectory.
Check and modify init_database.sql and fini_database.sql
files. After modification execute appropriate scripts
under DB account that has privileges to create roles and
database (under "postgres" superuser, for example).

psql -U postgres < sql/init_database.sql

After test yum may drop all the test data using the
following command

psql -U postgres < sql/fini_database.sql


# Measure results

1. First, youn need prepare executable. Change a number of threads
and scale factor in source file. Be carreful not to overfull memory of
you host. The main goal is to achieve some visible memory allocation
size by all running threads. Change appropriate config variables
in Makefile, and make executable.

vim Makefile
vim libpqxx-mem-overrun.cpp
make

2. [optional] You may check test by valgrind to ensure the there are
no memory leaks in test.

valgrind ./libpqxx-mem-overrun

Output example

```
==20086==
==20086== HEAP SUMMARY:
==20086==     in use at exit: 0 bytes in 0 blocks
==20086==   total heap usage: 4,371,820 allocs, 4,371,820 frees, 7,884,699,802 bytes allocated
==20086==
==20086== All heap blocks were freed -- no leaks are possible
==20086==
==20086== For lists of detected and suppressed errors, rerun with: -s
==20086== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

3. Install empty python environment and init it

python -m venv venv
source ./venv/bin/activate

4. Install memory profiler and matplot library

pip install memory-profiler
pip install matplotlib

5. Run test under profiler

mprof run ./libpqxx-mem-overrun

6. Plot memory usage diagramm

mprof plot --output libpqxx-mem-overrun.png
