#pragma once

#include <vector>
#include <list>
#include <istream>


class Bulk
{
	std::vector<std::string> data;
	std::string m_id;
public:

	Bulk();

	auto begin()->decltype(data.begin());
	auto end()->decltype(data.end());

	void append(const std::string &s);

	std::string id();
};


class Bulk_Printer;


class Bulk_Reader
{
	std::list<Bulk> bulks;
	std::list<Bulk_Printer *> printers;
	std::istream &is;
	std::size_t bulk_size;
	std::size_t bulk_cnt;
	int level;

	Bulk_Reader() = delete;

public:
	Bulk_Reader(std::istream &_is, std::size_t c);

	void add_printer(Bulk_Printer *p);
	void remove_printer(Bulk_Printer *p);

	void create_bulk();
	void append_bulk(const std::string &s);
	void close_bulk();
	// void push_bulk();

	void process();
	void notify(Bulk &b);
	
};


class Bulk_Printer
{
	Bulk_Reader *reader;

	Bulk_Printer() = delete;
public:
	Bulk_Printer(Bulk_Reader &r);
	~Bulk_Printer();
	virtual void update(Bulk &b) = 0;	
};


class Con_Printer: Bulk_Printer
{
	Con_Printer() = delete;
public:	
	Con_Printer(Bulk_Reader &r);
	void update(Bulk &b) override;
};


class File_Printer: Bulk_Printer
{
	File_Printer() = delete;
public:	
	File_Printer(Bulk_Reader &r);
	void update(Bulk &b) override;
};
