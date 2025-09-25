#include <iostream>
#include <fstream>

class Parser {
  public:
    Parser(std::ifstream& file, std::string_view file_name) { 
      if (file_name.length() < 1)
        std::cerr << "[Error] file_name was not passed" << '\n';

      int n = file_name.length();
      
      // Check if file is assembly or not    
      if (file_name[n - 3] == 'a' && file_name[n - 2] == 's' && file_name[n - 3] == 'm')
        std::cerr << "[Error] File is not an assembly" << '\n';
      
      // Check if file is open or not if not open it
      if (!file.is_open())
        file.open(file_name);
    
      if (!file)
        std::cerr << "[ERROR] unable to open file" << '\n';
    }
};
