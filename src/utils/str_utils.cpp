#include "./str_utils.h"

std::vector<std::string> strsplit(std::string tosplit, std::string delimiter) {
	
	std::vector<std::string> result;
	
	int start = 0;
	int end = tosplit.find(delimiter);
	
	if (end == (int)std::string::npos) { result.push_back(tosplit); }
	else {
		
		while (end != -1) {
			
			result.push_back(tosplit.substr(start, end - start));
			start = end + delimiter.size();
			end = tosplit.find(delimiter, start);
		}
		result.push_back(tosplit.substr(start, end - start));
	}
	
	return result;
}