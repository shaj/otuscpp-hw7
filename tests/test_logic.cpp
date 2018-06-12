


#define BOOST_LOG_DYN_LINK 1

#define BOOST_TEST_MODULE test_logic

#include <boost/test/included/unit_test.hpp>
#include "log.h"
#include "bulk.h"

std::shared_ptr<spdlog::logger> my::my_logger;

class bulk_checker : public Bulk_Printer
{
	bulk_checker() = delete;
	bulk_checker(const std::weak_ptr<Bulk_Reader> &r)
	{
		reader = r;
	}

public:	
	std::vector<int> result;

	static std::shared_ptr<bulk_checker> create(const std::weak_ptr<Bulk_Reader> &r)
	{
		auto ret = std::shared_ptr<bulk_checker>(new bulk_checker(r));
		r.lock()->add_printer(ret);
		return ret;
	}


	void update(Bulk &b) override
	{
		int c = 0;
		for(const auto &it: b) c++;
		result.push_back(c);
	}
};


BOOST_AUTO_TEST_SUITE(bulk_logic_test_suite)

BOOST_AUTO_TEST_CASE(test_bulk1) 
{

	std::istringstream in_data { "cmd0\n"
						  "cmd2\n"
						  "cmd3\n"
						  "cmd4\n"
						  "cmd5\n"
						  "cmd6\n"
						  "cmd7\n"
						  "cmd8\n"
						  "cmd9"
	};

	auto reader = std::make_shared<Bulk_Reader>(in_data, 3);
	auto checker = bulk_checker::create(reader);

	BOOST_CHECK_NO_THROW(reader->process());

	std::vector<int> a{3,3,3};
	BOOST_CHECK(checker->result == a);
}

BOOST_AUTO_TEST_CASE(test_bulk2) 
{

	std::istringstream in_data { "cmd0\n"
						  "cmd2\n"
						  "{\n"
						  "cmd4\n"
						  "cmd5\n"
						  "cmd6\n"
						  "}\n"
						  "cmd8\n"
						  "cmd9"
	};

	auto reader = std::make_shared<Bulk_Reader>(in_data, 3);
	auto checker = bulk_checker::create(reader);

	BOOST_CHECK_NO_THROW(reader->process());

	std::vector<int> a{2,3,2};
	BOOST_CHECK(checker->result == a);
}

BOOST_AUTO_TEST_CASE(test_bulk3) 
{

	std::istringstream in_data { "cmd100\n"
								"cmd101\n"
								"cmd102\n"
								"cmd103\n"
								"cmd104\n"
								"{\n"
								"cmd1\n"
								"cmd2\n"
								"cmd3\n"
								"{\n"
								"cmd4\n"
								"cmd5\n"
								"cmd6\n"
								"cmd7\n"
								"}\n"
								"cmd8\n"
								"cmd9\n"
								"}\n"
								"cmd200\n"
								"cmd201\n"
								"cmd202\n"
								"cmd203\n"
								"cmd204\n"
	};

	auto reader = std::make_shared<Bulk_Reader>(in_data, 3);
	auto checker = bulk_checker::create(reader);

	BOOST_CHECK_NO_THROW(reader->process());

	std::vector<int> a{3, 2, 9, 3, 2};
	BOOST_CHECK(checker->result == a);
}

BOOST_AUTO_TEST_CASE(test_bulk4) 
{

	std::istringstream in_data { "cmd0\n"
								  "cmd2\n"
								  "{\n"
								  "cmd4\n"
								  "cmd5\n"
								  "cmd6\n"
	};

	auto reader = std::make_shared<Bulk_Reader>(in_data, 3);
	auto checker = bulk_checker::create(reader);

	BOOST_CHECK_NO_THROW(reader->process());

	std::vector<int> a{2};
	BOOST_CHECK(checker->result == a);
}



BOOST_AUTO_TEST_SUITE_END()
