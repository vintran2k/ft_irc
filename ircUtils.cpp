#include "includes.hpp"

void	split(std::vector<std::string> & res, std::string str, std::string const & delimiter) {

	size_t					pos = 0;
	std::string::iterator	it;
	
	while (str.size())
	{
		pos = str.find_first_of(delimiter);
		if (pos == std::string::npos)
			pos = str.size();
		res.push_back(str.substr(0, pos));
		for (it = str.begin() + pos; it != str.end() && delimiter.find(*it) != std::string::npos; it++)
			pos++;
		str.erase(0, pos);
	}
}

std::string const	getTime() {

	time_t	t(time(NULL));
	std::string	res(ctime(&t));
	res.erase(res.end() - 1);
	
	return (res);
}
