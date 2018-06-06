


#define BOOST_LOG_DYN_LINK 1

#define BOOST_TEST_MODULE test_func

#include <boost/test/included/unit_test.hpp>
#include "bulk.h"

BOOST_AUTO_TEST_SUITE(bulk_func_test_suite)


BOOST_AUTO_TEST_CASE(test_bulk_remove_printer) 
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

	Bulk_Reader reader(in_data, 3);

	Con_Printer console(reader);

	File_Printer *pfile = new File_Printer(reader);

	delete pfile;

	BOOST_CHECK_NO_THROW(reader.process());
}

BOOST_AUTO_TEST_SUITE_END()
