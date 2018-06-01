
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#include "bulk.h"
#include "log.h"


Bulk::Bulk()
{
	SPDLOG_TRACE(my::my_logger, "Bulk::Bulk()");

	std::time_t t = std::time(nullptr);
	m_id = std::to_string((long long)t);
}

auto Bulk::begin()->decltype(data.begin())
{
	SPDLOG_TRACE(my::my_logger, "auto Bulk::begin()");

	return data.begin();
}

auto Bulk::end()->decltype(data.end())
{
	SPDLOG_TRACE(my::my_logger, "auto Bulk::end()");

	return data.end();
}

void Bulk::append(const std::string &s)
{
	SPDLOG_TRACE(my::my_logger, "void Bulk::append");

	data.push_back(s);
}

std::string Bulk::id()
{
	return m_id;
}



Bulk_Reader::Bulk_Reader(std::istream &_is, std::size_t c) : 
		is(_is), 
		bulk_size(c),
		level(0)
{
	SPDLOG_TRACE(my::my_logger, "Bulk_Reader::Bulk_Reader");

	if(bulk_size < 1) bulk_size = 1;

	SPDLOG_TRACE(my::my_logger, "   bulk_size={}", bulk_size);
}

void Bulk_Reader::add_printer(Bulk_Printer *p)
{
	SPDLOG_TRACE(my::my_logger, "void Bulk_Reader::add_printer");

	printers.push_back(p);
}

void Bulk_Reader::process()
{
	SPDLOG_TRACE(my::my_logger, "void Bulk_Reader::process()");

	if(is)
	{
		std::string str;
		int bulk_cnt = bulk_size;
		create_bulk();
		while(std::getline(is, str))
		{
			append_bulk(str);
		}
		close_bulk();
	}
}


void Bulk_Reader::create_bulk()
{
	SPDLOG_TRACE(my::my_logger, "void Bulk_Reader::create_bulk()");

	bulks.emplace_back();
	bulk_cnt = bulk_size;
}

void Bulk_Reader::append_bulk(const std::string &s)
{
	SPDLOG_TRACE(my::my_logger, "void Bulk_Reader::append_bulk");

	if(level == 0)
	{
		if(s == "{")
		{
			if(bulk_cnt != bulk_size)
			{
				notify(*(--bulks.end()));
				create_bulk();
			}
			level++;
		}
		else if(bulk_cnt)
		{
			(--bulks.end())->append(s);
			bulk_cnt--;
		}
		else
		{
			notify(*(--bulks.end()));
			create_bulk();
			(--bulks.end())->append(s);
			bulk_cnt--;
		}
	}
	else
	{
		if(s == "{") level++;
		else if(s == "}")
		{
			if(--level == 0)
			{
				notify(*(--bulks.end()));
				create_bulk();
			}
		}
		else (--bulks.end())->append(s);
	}
}

void Bulk_Reader::close_bulk()
{
	SPDLOG_TRACE(my::my_logger, "void Bulk_Reader::close_bulk()");

	if((level == 0) && (bulk_cnt != bulk_size))
		notify(*(--bulks.end()));
}

void Bulk_Reader::notify(Bulk &b)
{
	SPDLOG_TRACE(my::my_logger, "void Bulk_Reader::notify");

	for(const auto it: printers)
	{
		it->update(b);
	}
}



Bulk_Printer::Bulk_Printer(Bulk_Reader &r)
{
	SPDLOG_TRACE(my::my_logger, "Bulk_Printer::Bulk_Printer");

	r.add_printer(this);
}


Con_Printer::Con_Printer(Bulk_Reader &r) : Bulk_Printer(r)
{
	SPDLOG_TRACE(my::my_logger, "Con_Printer::Con_Printer");

}

void Con_Printer::update(Bulk &b) 
{
	SPDLOG_TRACE(my::my_logger, "void Con_Printer::update");

	std::cout << "bulk " << b.id() << ": ";
	auto it = b.begin();
	std::cout << *it;
	it++;
	for(; it != b.end(); it++)
	{
		std::cout << ", " << *it;
	}
	std::cout << std::endl;
}


File_Printer::File_Printer(Bulk_Reader &r) : Bulk_Printer(r)
{
	SPDLOG_TRACE(my::my_logger, "File_Printer::File_Printer");

}

void File_Printer::update(Bulk &b) 
{
	SPDLOG_TRACE(my::my_logger, "void File_Printer::update");

	std::string fname {"bulk" + b.id()};
	std::fstream fs;
	fs.open(fname + ".log", std::ios::in);
	if(fs)
	{
		int cnt = 1;
		std::string fname_new;
		while(fs)
		{
			fs.close();
			fname_new = fname + "_" + std::to_string(cnt);
			fs.open(fname_new + ".log");
			cnt++;
			if(cnt > 10000) throw std::logic_error("Can not create log file");
		}
		fname = fname_new;
	}
	fs.close();
	SPDLOG_TRACE(my::my_logger, "   fname={}", fname);
	fs.open(fname + ".log", std::ios::out);

	for(const auto &it: b)
	{
		fs << it << std::endl;
	}
	fs.close();
}




