


#include <iostream>
#include "log.h"
#include "version.h"
#include "bulk.h"


#include <boost/program_options.hpp>

namespace po = boost::program_options;


void set_bulk(std::size_t val)
{
	std::cout << "bulk size is " << val << std::endl;
}

std::shared_ptr<spdlog::logger> my::my_logger;

int main (int argc, char* argv[])
{
	try
	{
		my::my_logger = spdlog::basic_logger_st("mainlogger", "bulk.log", true);
		my::my_logger->set_level(spdlog::level::info);
		my::my_logger->info(" -=- Start bulk");

		po::options_description descr("Allowed options");
		descr.add_options()
			("help,h", "Produce help message")
			("version,v", "Version")
			("debug,d", "Enable loggigng")
			("bulk,b", po::value<std::size_t>()->default_value(3)->notifier(set_bulk), "bulk size")
		;

		po::positional_options_description p;
		p.add("bulk", -1);
		po::variables_map vm;
		// po::store(po::parse_command_line(argc, argv, descr), vm);
		po::store(po::command_line_parser(argc, argv).options(descr).positional(p).run(), vm);
		po::notify(vm);

		if(vm.count("help"))
		{
			std::cout << descr << std::endl;
			return 0;
		}

		if(vm.count("version"))
		{
			std::cout << "OTUS cpp\n";
			std::cout << "Homework 7. Bulk.\n";
			std::cout << "Version " << VER << "\n" << std::endl;
			return 0;
		}

		if(vm.count("debug"))
		{
			my::my_logger->set_level(spdlog::level::trace);
		}




		auto reader = std::make_shared<Bulk_Reader>(std::cin, vm["bulk"].as<size_t>());
		auto console = Con_Printer::create(reader);
		auto file = File_Printer::create(reader);

		reader->process();




		my::my_logger->info(" -=- End bulk");

	}
	catch(const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
