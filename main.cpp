


#include <iostream>
#include "version.h"
#include "bulk.h"


#include <boost/program_options.hpp>

namespace po = boost::program_options;


void set_bulk(std::size_t val)
{
	std::cout << "bulk size is " << val << std::endl;
}

int main (int argc, char* argv[])
{
	try
	{

		po::options_description descr("Allowed options");
		descr.add_options()
			("help,h", "Produce help message")
			("version,v", "Version")
			("debug,d", "Enable loggigng")
			("bulk,b", po::value<std::size_t>()->default_value(3)->notifier(set_bulk), "bulk size")
		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, descr), vm);
		po::notify(vm);

		if(vm.count("help"))
		{
			std::cout << descr << std::endl;
			return 0;
		}

		if(vm.count("version"))
		{
			std::cout << "OTUS cpp" << std::endl;
			std::cout << "Homework 7. Bulk." << std::endl;
			std::cout << "Version " << VER << std::endl << std::endl;
			return 0;
		}

		if(vm.count("debug"))
		{
			// logging::core::get()->set_logging_enabled(true);	
		}

		Bulk_Reader reader(std::cin, vm["bulk"].as<size_t>());
		Con_Printer console(reader);
		File_Printer file(reader);

		reader.process();


	}
	catch(const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
