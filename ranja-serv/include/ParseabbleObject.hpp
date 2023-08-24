#pragma once

#include <string>
#include <vector>

class ParseabbleObject {
	public:
		ParseabbleObject();
		~ParseabbleObject();
		// more constructors for cannonical form
		std::vector<std::string> lexer();
}
