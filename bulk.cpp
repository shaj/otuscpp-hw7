

#include "bulk.h"






void Bulk_Reader::process()
{
	if(is != nullptr)
	{
		std::string str;
		
		while(std::getline(is, str).good())
		{
			process_str(str);
		}
	}
}
