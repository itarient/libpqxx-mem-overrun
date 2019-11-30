The short example to investigate the following issue.
The libpqxx library in multithreaded use cases does not
enough free memory allocated for results of the query.

libpqxx was built from sources (master branch) using 
               gcc -std=c++17 -O2
(gcc version 9.2.0)

