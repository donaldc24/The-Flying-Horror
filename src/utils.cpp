#include <flying_horror/utils.hpp>

#include <fstream>

namespace flying_horror {
    void writeToCSV(const std::string& data, const std::string filename) {
        std::ofstream myFile(filename, std::ios::app);

        if (myFile.is_open()) {
            myFile << data << '\n';
        }
    }

}