#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

namespace FileModification {

    void modify_file()
    {
        std::string file_path = "openssl.cnf"; // Replace with your file path

        // Open the file for reading
        std::ifstream input_file(file_path);
        if (!input_file.is_open()) {
            std::cerr << "Error opening file: " << file_path << std::endl;
            return ;
        }

        const char* path_prefix = "C:\\temp";

        std::string path_prefix_forward_slash(path_prefix);

        std::replace(path_prefix_forward_slash.begin(), path_prefix_forward_slash.end(), '\\', '/');

        // Read the content
        std::string content;
        std::string line;
        while (std::getline(input_file, line)) {
            if (line == "CHANGE THIS LINE")
            {
                content += ".include ";
                content += path_prefix_forward_slash;
                content += "/config/fipsmodule.cnf\n";
            }
            else
                content += line + "\n"; // Add newline character
        }
        input_file.close();

        // Make changes (for example, replace "old_text" with "new_text")
        /*
        std::string new_content = content;
        size_t pos = new_content.find("old_text");
        if (pos != std::string::npos) {
            new_content.replace(pos, 8, "new_text");
        }
        */

        // Save the changes
        std::ofstream output_file(file_path);
        if (!output_file.is_open()) {
            std::cerr << "Error opening file for writing: " << file_path << std::endl;
            return;
        }
        output_file << content;
        output_file.close();

        std::cout << "Changes made to " << file_path << " and saved successfully." << std::endl;

        return;
    }
}