The short example to investigate the following issue.
The libpqxx library in multithreaded use cases does not
enough free memory allocated for results of the query.

libpqxx was built from sources (master branch) using 
               gcc -std=c++17 -O2
(gcc version 9.2.0)


-- Prepare test database --

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
