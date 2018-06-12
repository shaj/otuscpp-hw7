


#define BOOST_LOG_DYN_LINK 1

#define BOOST_TEST_MODULE test_func

#include <boost/test/included/unit_test.hpp>
#include "log.h"
#include "bulk.h"

std::shared_ptr<spdlog::logger> my::my_logger;

struct F {
    F()           
    {
    	std::cout << "Create logger" << std::endl; 
		my::my_logger = spdlog::basic_logger_st("mainlogger", "bulk.log", true);
		my::my_logger->set_level(spdlog::level::info);
    }
    ~F()         
    { 
    	std::cout << "Destroy logger" << std::endl; 
    }
};
 

BOOST_AUTO_TEST_SUITE(bulk_func_test_suite)

BOOST_GLOBAL_FIXTURE( F )

BOOST_AUTO_TEST_CASE(test_bulk_class_id) 
{
	Bulk b;
	std::cout << b.id() << "\nsize = " << b.id().size() << std::endl;
	BOOST_CHECK(b.id().size() == 10);
}

BOOST_AUTO_TEST_CASE(test_bulk_append) 
{
	Bulk b;
	for(int i=0; i<10; i++)
	{
		BOOST_CHECK_NO_THROW(b.append(std::string("cmd" + std::to_string(i))));
	}
	BOOST_CHECK(b.size() == 10);
}

BOOST_AUTO_TEST_CASE(test_bulk_iterators) 
{
	Bulk b;
	for(int i=0; i<10; i++)
	{
		b.append(std::string("cmd" + std::to_string(i)));
	}

	int cnt = 0;
	for(const auto &it: b)
	{
		BOOST_CHECK(it == std::string("cmd" + std::to_string(cnt)));
		cnt++;
	}

	cnt = 0;
	for(auto &it: b)
	{
		BOOST_CHECK(it == std::string("cmd" + std::to_string(cnt)));
		BOOST_CHECK_NO_THROW(it = std::string("checked"));
		cnt++;
	}

	for(const auto &it: b)
	{
		BOOST_CHECK(it == std::string("checked"));
		cnt++;
	}

	BOOST_CHECK(b.size() == 10);
}


// class printer_test : test_bulk_remove_printer
// {
// 	printer_test(const std::weak_ptr<Bulk_Reader> &r)
// 	{
// 		reader = r;
// 	}

// public:	
// 	std::vector<int> result;

// 	static std::shared_ptr<printer_test> create(const std::weak_ptr<Bulk_Reader> &r)
// 	{
// 		auto ret = std::shared_ptr<printer_test>(new printer_test(r));
// 		return ret;
// 	}


// 	void update(Bulk &b) override
// 	{
// 		int c = 0;
// 		for(const auto &it: b) c++;
// 		result.push_back(c);
// 	}
// };

// BOOST_AUTO_TEST_CASE(test_bulk_add_printer) 
// {
// 	std::istringstream in_data { "cmd0\n"
// 								  "cmd2\n"
// 	};
// 	auto reader = std::make_shared<Bulk_Reader>(in_data, 3);
// }

BOOST_AUTO_TEST_CASE(test_bulk_remove_printer) 
{
	std::istringstream in_data { "cmd0\n"
								  "cmd2\n"
	};

	auto reader = std::make_shared<Bulk_Reader>(in_data, 3);
	auto console = Con_Printer::create(reader);
	{
		auto fp = File_Printer::create(reader);
	}

	BOOST_CHECK_NO_THROW(reader->process());
}

BOOST_AUTO_TEST_SUITE_END()
