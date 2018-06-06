


#define BOOST_LOG_DYN_LINK 1

#define BOOST_TEST_MODULE test_logic

#include <boost/test/included/unit_test.hpp>
#include "bulk.h"

class bulk_checker : Bulk_Printer
{

	bulk_checker() = delete;
public:	
	std::vector<int> result;

	bulk_checker(Bulk_Reader &r) : Bulk_Printer(r) {}

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

	std::string in_data { "cmd0\n"
						  "cmd2\n"
						  "cmd3\n"
						  "cmd4\n"
						  "cmd5\n"
						  "cmd6\n"
						  "cmd7\n"
						  "cmd8\n"
						  "cmd9"
	};
	std::stringbuf in_buffer(in_data);
	std::istream in_stream(&in_buffer);

	Bulk_Reader reader(in_stream, 3);

	bulk_checker checker(reader);

	BOOST_CHECK_NO_THROW(reader.process());

	std::vector<int> a{3,3,3};
	BOOST_CHECK(checker.result == a);
}

BOOST_AUTO_TEST_CASE(test_bulk2) 
{

	std::string in_data { "cmd0\n"
						  "cmd2\n"
						  "{\n"
						  "cmd4\n"
						  "cmd5\n"
						  "cmd6\n"
						  "}\n"
						  "cmd8\n"
						  "cmd9"
	};
	std::stringbuf in_buffer(in_data);
	std::istream in_stream(&in_buffer);

	Bulk_Reader reader(in_stream, 3);

	bulk_checker checker(reader);

	BOOST_CHECK_NO_THROW(reader.process());

	std::vector<int> a{2,3,2};
	BOOST_CHECK(checker.result == a);
}

BOOST_AUTO_TEST_CASE(test_bulk3) 
{

	std::string in_data { "cmd0\n"
						  "cmd2\n"
						  "{\n"
						  "cmd4\n"
						  "cmd5\n"
						  "cmd6\n"
	};
	std::stringbuf in_buffer(in_data);
	std::istream in_stream(&in_buffer);

	Bulk_Reader reader(in_stream, 3);

	bulk_checker checker(reader);

	BOOST_CHECK_NO_THROW(reader.process());

	std::vector<int> a{2};
	BOOST_CHECK(checker.result == a);
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
	Bulk_Reader reader(in_data, 3);

	bulk_checker checker(reader);

	BOOST_CHECK_NO_THROW(reader.process());

	std::vector<int> a{2};
	BOOST_CHECK(checker.result == a);
}



BOOST_AUTO_TEST_SUITE_END()
