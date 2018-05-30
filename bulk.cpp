
#include <iostream>
#include <string>
#include <ctime>

#include "bulk.h"



Bulk::Bulk()
{
	std::cout << "Bulk::Bulk()" << std::endl;

	std::time_t t = std::time(nullptr);
	id = std::to_string((long long)t);
}

auto Bulk::begin()->decltype(data.begin())
{
	std::cout << "auto Bulk::begin()" << std::endl;

	return data.begin();
}

auto Bulk::end()->decltype(data.end())
{
	std::cout << "auto Bulk::end()" << std::endl;

	return data.end();
}

void Bulk::append(const std::string &s)
{
	std::cout << "void Bulk::append" << std::endl;

	data.push_back(s);
}



Bulk_Reader::Bulk_Reader(std::istream &_is, std::size_t c) : 
		is(_is), 
		bulk_size(c),
		level(0)
{
	std::cout << "Bulk_Reader::Bulk_Reader" << std::endl;

	if(bulk_size < 1) bulk_size = 1;

	std::cout << "   bulk_size=" << bulk_size << std::endl;
}

void Bulk_Reader::add_printer(Bulk_Printer *p)
{
	std::cout << "void Bulk_Reader::add_printer" << std::endl;

	printers.push_back(p);
}

void Bulk_Reader::process()
{
	std::cout << "void Bulk_Reader::process()" << std::endl;

	// if(*is != nullptr)
	// {
		std::string str;
		int bulk_cnt = bulk_size;
		create_bulk();
		while(std::getline(is, str).good())
		{
			append_bulk(str);
		}
		close_bulk();
	// }
}


void Bulk_Reader::create_bulk()
{
	std::cout << "void Bulk_Reader::create_bulk()" << std::endl;

	bulks.emplace_back();
	bulk_cnt = bulk_size;
}

void Bulk_Reader::append_bulk(const std::string &s)
{
	std::cout << "void Bulk_Reader::append_bulk" << std::endl;

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
	std::cout << "void Bulk_Reader::close_bulk()" << std::endl;

	if((level == 0) && (bulk_cnt != bulk_size))
		notify(*(--bulks.end()));
}

void Bulk_Reader::notify(Bulk &b)
{
	std::cout << "void Bulk_Reader::notify" << std::endl;

	for(const auto it: printers)
	{
		it->update(b);
	}
}



Bulk_Printer::Bulk_Printer(Bulk_Reader &r)
{
	std::cout << "Bulk_Printer::Bulk_Printer" << std::endl;

	r.add_printer(this);
}


Con_Printer::Con_Printer(Bulk_Reader &r) : Bulk_Printer(r)
{
	std::cout << "Con_Printer::Con_Printer" << std::endl;

}

void Con_Printer::update(Bulk &b) 
{
	std::cout << "void Con_Printer::update" << std::endl;

	std::cout << "bulk: ";
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
	std::cout << "File_Printer::File_Printer" << std::endl;

}

void File_Printer::update(Bulk &b) 
{
	std::cout << "void File_Printer::update" << std::endl;

}

