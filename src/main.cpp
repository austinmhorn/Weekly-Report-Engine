#include "main.hpp"

#include <functional>
#include <regex>


void injectData(std::vector<Property>& properties) 
{
    injectDashboardReport(properties);
    injectAvailabilityReport(properties);
    injectMoveOutReasonsReport(properties);
    injectBoxScoreLeadConversionsReport(properties);
    injectBoxScoreMakeReadyStatusReport(properties);
    injectBoxScorePropertyPulseReport(properties);
    injectResidentRetentionReport(properties);
    injectReceiptsByChargeCode_Receipts_Report(properties);
    injectGrossPotentialRentReport(properties);
    injectWorkOrderPerformance(properties);
    injectResidentAgedReceivablesDetailReport(properties);
    injectResidentAgedReceivablesSummaryReport(properties);
    injectIncomeStatementReport(properties);
    injectDailyAndWeeklyOperationsReport(properties);
}



void writeData(std::vector<Property>& properties)
{
    writeOverviewReport(properties);
    writeAvailabilityReport(properties);
    writeResidentRetentionReport(properties);
    writeIncomeStatementReport(properties);
    writeOperationsReport(properties);
    //writeGrossPotentialRentReport(properties);
    //writeRentReceipts(properties);
    //writeResidentAgedReceivablesSummary(properties);
    //writeWorkOrderPerformanceReport(properties);

    PRINT_TOTAL_NUMBER_ROWS_WRITTEN();
}

bool ifDateProvidedInCommandLineArguements(int argc, const char **argv)
{
    if ( argc > 1 )
    {
        std::string input = argv[1];

        // Define a regular expression for the YYYY.MM.DD format
        std::regex datePattern(R"(^\d{4}\.\d{2}\.\d{2}$)");

        // Check if the input matches the pattern
        return (std::regex_match(input, datePattern));
    }

    return false;
}

int main(int argc, const char **argv) 
{
    if ( ifDateProvidedInCommandLineArguements(argc, argv) )
    {
        std::string customDate = argv[1];
        std::cout << "~ Custom date provided: \t (" << customDate << ")" << std::endl;

        sExportDate = customDate;
        replaceDotsWithSlashes(customDate);
        sExportDate_Forward_Slash_Format = customDate;
    }
    else
    {
        sExportDate = getFormattedDate(".");
        std::string date = sExportDate;
        replaceDotsWithSlashes(date);
        sExportDate_Forward_Slash_Format = date;
    }

    OUTPUT_DIRECTORY_PATH = "data/" + sExportDate + "/bin/";

    init(sExportDate);
    std::vector<Property> properties;

    injectData(properties);
    writeData(properties);

    return 0;
}
