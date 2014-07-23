#include "Command.hpp"

// Constructors
Command::Command(){}
Command::~Command(){}

// Methods
void Command::split(std::string strToSplit, std::string* first, std::string** second, char del){
	size_t delPos = strToSplit.find(del);
	*first = strToSplit.substr(0,delPos);
	if(delPos == strToSplit.npos){
		*second = NULL;
	}
	else{
		*second = new std::string(strToSplit.substr(delPos+1));
	}
}
