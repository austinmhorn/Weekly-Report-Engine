#ifndef __helper_functions_hpp
#define __helper_functions_hpp

#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include <sstream>

// Declaration
std::string getFormattedDate(const std::string& delimiter);

static std::string sExportDate;
static std::string sExportDate_Forward_Slash_Format;

#define PRINT_FUNCTION_START() std::cout << "\033[32m" << "~ Starting" << "\e[0m" << " - " << __func__ << "()" << std::endl;
#define PRINT_FUNCTION_STOP() std::cout << "\033[31m" << "~ Stopping" << "\e[0m" << " - " << __func__ << "()" << std::endl;

#define PRINT_NUM_ITEMS_PROCESSED(x) std::cout << "~\t (" << x << ") items processed" << std::endl;
#define PRINT_WRITE_FILE_MSG(x) std::cout << "~ Writing file: \"" << x << "\"" << std::endl;

namespace fs = std::filesystem;

// Helper function to trim leading and trailing whitespace and commas
std::string trim(const std::string& str) 
{
    auto start = str.begin();
    // Remove leading spaces and commas
    while (start != str.end() && (std::isspace(*start) || *start == ',')) 
        ++start;

    auto end = str.end();
    // Remove trailing spaces and commas
    do 
        --end;
    while (std::distance(start, end) > 0 && (std::isspace(*end) || *end == ','));

    return std::string(start, end + 1);
}

// Utility to split the full resident name into first and last name, and trim
void splitName(const std::string& fullName, std::string& firstName, std::string& lastName)
{
    std::stringstream ss(fullName);
    std::getline(ss, lastName, ',');  // Extract last name
    std::getline(ss, firstName);      // Extract first name (after the comma)

    // Trim any leading/trailing spaces and commas
    firstName = trim(firstName);
    lastName = trim(lastName);
}

// Helper function to remove commas from a string
std::string removeCommas(const std::string &str)
{
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), ','), result.end());
    return result;
}

// Helper function to remove commas from a string and convert it to float
float parseFloatWithComma(const std::string& str)
{
    std::string cleanStr = removeCommas(str);
    return std::stof(cleanStr);
}

// Helper function to remove commas and parentheses, then convert to float
float parseFloatWithCommaAndParentheses(const std::string& str)
{
    std::string cleanStr = str;

    // Remove commas
    cleanStr.erase(remove(cleanStr.begin(), cleanStr.end(), ','), cleanStr.end());

    // If the string has parentheses (indicating negative value), handle it
    if (cleanStr.front() == '(' && cleanStr.back() == ')')
    {
        cleanStr = cleanStr.substr(1, cleanStr.size() - 2); // Remove the parentheses
        return -std::stof(cleanStr);
    }

    return std::stof(cleanStr);
}

std::string trimDoubleQuotes(const std::string& str) 
{
    std::string::size_type start = str.find_first_not_of('"');
    std::string::size_type end = str.find_last_not_of('"');

    if (start == std::string::npos || end == std::string::npos) 
    {
        return ""; // Return empty string if there are only quotes
    }

    return str.substr(start, end - start + 1);
}

// Utility to convert string to std::tm for dates
std::tm stotm(const std::string& dateStr, const char* format = "%Y/%m/%d")
{
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, format);  // Parses the date string into std::tm
    tm.tm_year += 1900;
    tm.tm_mon += 1;
    return tm;
}

std::string tmtostr(std::tm& timeStruct) 
{
    timeStruct.tm_year -= 1900;
    timeStruct.tm_mon -= 1;

    char buffer[100]; // Buffer to hold the formatted date string
    // Format the date as "YYYY/M/D"
    std::strftime(buffer, sizeof(buffer), "%Y/%m/%d", &timeStruct);
    return std::string(buffer); // Convert buffer to std::string
}

std::string getFormattedDate(const std::string& delimiter = "/")
{
    std::time_t t = std::time(nullptr);
    std::tm currentDate;
    #ifdef _WIN32
        localtime_s(&currentDate, &t); // For Windows
    #else
        currentDate = *std::localtime(&t); // For Unix-based systems
    #endif

    std::stringstream ss;
    ss << (1900 + currentDate.tm_year) << delimiter
       << std::setfill('0') << std::setw(2) << (1 + currentDate.tm_mon) << delimiter
       << std::setfill('0') << std::setw(2) << currentDate.tm_mday;

    return ss.str();
}

std::string findZipFile(const std::string& dir) {
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".zip") {
            return entry.path().string();  // Return the first .zip file found
        }
    }
    return "";  // Return empty string if no .zip file is found
}

bool unzipFile(const std::string& zipFilePath, const std::string& destDir) {
    // Ensure the zip file exists
    if (!fs::exists(zipFilePath)) {
        std::cerr << "ZIP file does not exist: " << zipFilePath << std::endl;
        return false;
    }

    // Create the destination directory if it doesn't exist
    if (!fs::exists(destDir)) {
        fs::create_directories(destDir);
    }

    // Use the system command to call an external unzip tool, handling spaces in filenames
    std::string command = "unzip -o \"" + zipFilePath + "\" -d \"" + destDir + "\"";
    int result = system(command.c_str());

    if (result != 0) {
        std::cerr << "Failed to unzip the file. Command exited with code " << result << std::endl;
        return false;
    }

    return true;
}

std::string transformDatePeriodStr(const std::string& input) {
    // Map of month abbreviations to numbers
    std::unordered_map<std::string, int> monthMap = {
        {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4},
        {"May", 5}, {"Jun", 6}, {"Jul", 7}, {"Aug", 8},
        {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}
    };

    std::istringstream iss(input);
    std::string monthStr;
    int year;

    // Parse the month and year from the input string
    iss >> monthStr >> year;

    // Check if the month string is valid
    if (monthMap.find(monthStr) == monthMap.end()) {
        return "Invalid month format";
    }

    // Format the output as "M/YYYY"
    std::ostringstream oss;
    oss << monthMap[monthStr] << "/" << (year);  // Assumes "24" means "2024"
    
    return oss.str();
}

void replaceDotsWithSlashes(std::string& str) {
    for (char& ch : str) {
        if (ch == '.') {
            ch = '/';
        }
    }
}

int daysBetweenDates(const std::tm& inputDate) {
    // Get the current time
    std::time_t currentTime = std::time(nullptr);
    std::tm* currentDate = std::localtime(&currentTime);
    
    // Convert both dates to time_t for easy comparison
    std::time_t inputTime = std::mktime(const_cast<std::tm*>(&inputDate));
    
    // Calculate the difference in seconds
    double differenceInSeconds = std::difftime(currentTime, inputTime);
    
    // Convert seconds to days
    int daysDifference = static_cast<int>(differenceInSeconds / (60 * 60 * 24));

    return daysDifference;
}

#endif /* __helper_functions_hpp */