-- Execute these under privileged user

DROP DATABASE IF EXISTS libpqxx_db;
CREATE DATABASE libpqxx_db;

DROP ROLE IF EXISTS libpqxx_user;
CREATE ROLE libpqxx_user LOGIN;

GRANT ALL ON DATABASE libpqxx_db TO libpqxx_user;

-- Reconnect to libpqxx_db as libpqxx_user
\c libpqxx_db libpqxx_user

\set test_table_rowcount 1000000

CREATE OR REPLACE FUNCTION random_string()
RETURNS text
LANGUAGE plpgsql
AS $function$
DECLARE
  base_string text := 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
  base_string_len integer := length(base_string);
  rand_string_lenfactor integer := 30;
BEGIN
  RETURN (SELECT array_to_string(array(
    SELECT substr(base_string, trunc(random() * base_string_len)::integer + 1)
    FROM generate_series(1, rand_string_lenfactor)
  ), ''));
END;
$function$;

-- Create workload
CREATE UNLOGGED TABLE test_table
(
workload_1 text NOT NULL,
workload_2 text NOT NULL,
workload_3 text NOT NULL,
workload_4 text NOT NULL,
workload_5 text NOT NULL
);

INSERT INTO test_table (workload_1, workload_2, workload_3, workload_4, workload_5)
SELECT 
random_string(),
random_string(),
random_string(),
random_string(),
random_string()
FROM
generate_series(1, :test_table_rowcount);

