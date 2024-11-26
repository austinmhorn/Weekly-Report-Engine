#ifndef __main_hpp
#define __main_hpp

#include "injection_routines.hpp"
#include "output_routines.hpp"

void inflateAttachmentsDir(const std::string& date) noexcept(false) 
{
    std::string attachmentsDir = "attachments";
    std::string destDir = "data/" + date + "/raw";

    // Find the ZIP file in the attachments directory
    std::string zipFilePath = findZipFile(attachmentsDir);
    if (zipFilePath.empty()) {
        throw std::runtime_error("No ZIP file found in the attachments directory.");
    }

    // Unzip the file
    if (unzipFile(zipFilePath, destDir)) {
        std::cout << "Unzipped successfully!" << std::endl;
    } else {
        throw std::runtime_error("Failed to unzip the file.");
    }
}

bool copyCsvFiles(const std::string& sourceDir, const std::string& destDir) 
{
    // Ensure the source directory exists
    if (!fs::exists(sourceDir) || !fs::is_directory(sourceDir)) {
        std::cerr << "Source directory does not exist or is not a directory: " << sourceDir << std::endl;
        return false;
    }

    // Create the destination directory if it doesn't exist
    if (!fs::exists(destDir)) {
        fs::create_directories(destDir);
    }

    // Iterate through the source directory
    for (const auto& entry : fs::directory_iterator(sourceDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            try {
                // Construct the destination file path
                fs::path destFilePath = fs::path(destDir) / entry.path().filename();
                std::string filename = entry.path().filename();
                std::string newFilePath = "";
                
                if ( filename == "04. Weekly Report_Gross Potential Rent (GPR).csv" ) {
                    newFilePath = destDir + "/" + "Gross Potential Rent.csv";
                }
                else if ( filename.find("Receipt By Charge Code RENT only - Pre-Payments.csv") != SIZE_MAX ) {
                    newFilePath = destDir + "/" + "Receipts By Charge Code (Rent) - Pre-Payments.csv";
                }
                else if ( filename.find("Receipt By Charge Code RENT only - Receipts.csv") != SIZE_MAX ) {
                    newFilePath = destDir + "/" + "Receipts By Charge Code (Rent) - Receipts.csv";
                }
                else if ( filename == "06-07. Weekly Report_Resident Aged Receivables - Detail.csv" ) {
                    newFilePath = destDir + "/" + "Resident Aged Receivables (Detail).csv";
                }
                else if ( filename == "06-07. Weekly Report_Resident Aged Receivables - Summary.csv" ) {
                    newFilePath = destDir + "/" + "Resident Aged Receivables (Summary).csv";
                }
                else if ( filename == "08. Weekly Report_Availability.csv" ) {
                    newFilePath = destDir + "/" + "Availability.csv";
                }
                else if ( filename == "09. Weekly Report_Resident Retention.csv" ) {
                    newFilePath = destDir + "/" + "Resident Retention.csv";
                }
                else if ( filename == "10. Weekly Report_Move Out Reasons.csv" ) {
                    newFilePath = destDir + "/" + "Move Out Reasons.csv";
                }
                else if ( filename.find("Weekly Report_Box Score - Availability") != SIZE_MAX ) {
                    newFilePath = destDir + "/" + "Box Score - Availability.csv";
                }
                else if ( filename.find("Weekly Report_Box Score - Lead Activity") != SIZE_MAX ) {
                    newFilePath = destDir + "/" + "Box Score - Lead Activity.csv";
                }
                else if ( filename.find("Weekly Report_Box Score - Lead Conversions") != SIZE_MAX ) {
                    newFilePath = destDir + "/" + "Box Score - Lead Conversions.csv";
                }
                else if ( filename.find("Weekly Report_Box Score - Make Ready Status") != SIZE_MAX ) {
                    newFilePath = destDir + "/" + "Box Score - Make Ready Status.csv";
                }
                else if ( filename.find("Weekly Report_Box Score - Property Pulse") != SIZE_MAX ) {
                    newFilePath = destDir + "/" + "Box Score - Property Pulse.csv";
                }
                else if ( filename == "12. Weekly Report_Work Order Performance.csv" ) {
                    newFilePath = destDir + "/" + "Work Order Performance.csv";
                }
                else if ( filename == "Birchstone Dashboard v2.3.csv" ) {
                    newFilePath = destDir + "/" + "Birchstone Dashboard.csv";
                }
                else if ( filename == "17. Daily and Weekly Operations.csv" ) {
                    newFilePath = destDir + "/" + "Daily and Weekly Operations.csv";
                }
                else if ( filename == "16. Income Statement as Cash.csv" ) {
                    newFilePath = destDir + "/" + "Income Statement.csv";
                }

                if ( newFilePath.size() ) {
                    // Copy the file to the destination directory
                    fs::copy_file(entry.path(), newFilePath, fs::copy_options::overwrite_existing);
                    std::cout << "Copied " << entry.path() << " to " << newFilePath << std::endl;
                }
                
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Error copying file " << entry.path() << ": " << e.what() << std::endl;
            }
        }
    }

    return true;
}

void cleanFiles(const std::string& date) 
{
    std::string srcDir = "data/" + date + "/raw";
    std::string destDir = "data/" + date + "/clean";

    copyCsvFiles(srcDir, destDir);
}

void init(const std::string& date) 
{
    std::string path = "data/" + date + "/clean";
    std::cout << "~ Generating for:\t\t (" << date << ")" << std::endl;

    if ( fs::exists(path) ) {
        std::cout << "~ Bypassing inflation and clean: (Files exist)" << std::endl;
        return;
    }

    try {
        inflateAttachmentsDir(date);
        cleanFiles(date);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void simpleCopyAvailabilityReport(const std::string& date)
{
    std::string src = "data/" + date + "/clean/Availability.csv";
    std::string dest = "data/" + date + "/bin/Availability Report.csv";

    std::string command = "python3 fix_csv_format.py \"" + src + "\" \"" + dest + "\"";
    std::cout << "~ Executing script: " << command << std::endl;
    system(command.c_str());
}

#endif /* __main_hpp */