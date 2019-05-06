#include <iostream>

int main(int argc, char *argv[])
{
	std::cout << "Magilla CS Coding Challenge" << std::endl;
	
	if (argc!=2)
	{
		std::cout << "Usage: " << std::string(argv[0]) << " <input file>" << std::endl;
	}
	return 0;
}