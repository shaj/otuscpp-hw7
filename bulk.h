#pragma once



class Bulk
{
	std::vector<std::string> data;
public:

	Bulk();

	const auto begin()
	{
		return data.cbegin();
	}

	const auto end()
	{
		return data.cend();
	}
};

class Bulk_Printer
{
public:
	virtual void update(const Bulk &b) = 0;	
}

class Bulk_Reader
{
	std::vector<Bulk> bulks;
	std::vector<Bulk_Printer> printers;
	std::istream is;

	Bulk_Reader() = delete;

public:
	Bulk_Reader(const std::istream &_is);

	void add_printer();
	void push_bulk();

	void process();
	
}


