#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <iostream>
#include <pqxx/pqxx>

pqxx::connection connection_pool[5] = {
	pqxx::connection("postgresql://libpqxx_user@localhost/libpqxx_db"),
	pqxx::connection("postgresql://libpqxx_user@localhost/libpqxx_db"),
	pqxx::connection("postgresql://libpqxx_user@localhost/libpqxx_db"),
	pqxx::connection("postgresql://libpqxx_user@localhost/libpqxx_db"),
	pqxx::connection("postgresql://libpqxx_user@localhost/libpqxx_db")
};
	
pqxx::result query(pqxx::connection& conn)
{
  pqxx::work txn{conn};
  pqxx::result r = txn.exec("SELECT * FROM test_table");
  txn.commit();

  return r;
}

bool pool_start = false;
std::condition_variable pool_cv;
std::mutex pool_access;
std::mutex cout_access;
 
struct test_thread { 
	int thread_id;
	
	test_thread(int _id) : 
		thread_id(_id)
	{}
	
	void operator()() {
		try {
			{
				std::unique_lock<std::mutex> lock(cout_access);
				std::cout << "Thread " << thread_id << " is running" << std::endl;
			}
			{
				std::unique_lock<std::mutex> lk(pool_access);
				pool_cv.wait(lk, []{ return pool_start; });
			}
			pqxx::result r = query(connection_pool[thread_id - 1]); 
			for (auto row: r) {
				std::unique_lock<std::mutex> lock(cout_access);

				std::cout << thread_id << ": | "; 
				for (auto field: row) 
					std::cout << field.view().substr(0,10) << " | "; 
				std::cout << std::endl; 
			}
		} catch (const pqxx::sql_error &e) { 
			std::cerr << "Thread: " << thread_id << " SQL error: " << e.what() << std::endl; 
			std::cerr << "Thread: " << thread_id << "Query was: " << e.query() << std::endl; 
		} catch (const std::exception &e) { 
			std::cerr << "Thread: " << thread_id << "Error: " << e.what() << std::endl; 
		}
	} 
};

void test(const int thread_count)
{
	std::vector<std::thread> pool;
	pool_start = false;
	
	{
		std::unique_lock<std::mutex> lock(cout_access);
		std::cout << "Preparing pool of " << thread_count << " threads" << std::endl;
		std::cout << "Waiting 3 seconds to start" << std::endl;
	}
	
	for (int t = 0; t < thread_count; ++t) {
		pool.push_back(std::thread(test_thread(t + 1)));
	}
	
	std::this_thread::sleep_for(std::chrono::seconds(3));
	{
		std::lock_guard<std::mutex> lk(pool_access);
		pool_start = true;
		pool_cv.notify_all();
	}

	for (auto& t: pool)
		t.join();
		
	{
		std::unique_lock<std::mutex> lock(cout_access);
		std::cout << "Threads finished. Waiting 3 seconds" << std::endl;
	}
	std::this_thread::sleep_for(std::chrono::seconds(3));
}

int main(int, char *argv[])
{
	for (int t = 1; t <= 5; ++t)
		test(t);
	
	return 0;
}
