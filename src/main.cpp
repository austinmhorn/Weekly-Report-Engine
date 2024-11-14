#include "main.hpp"


void injectData(std::vector<Property>& properties) 
{
    injectDashboardReport(properties);
    injectAvailabilityReport(properties);
    injectMoveOutReasonsReport(properties);
    injectBoxScoreLeadConversionsReport(properties);
    injectBoxScoreMakeReadyStatusReport(properties);
    injectBoxScorePropertyPulseReport(properties);
    injectResidentRetentionReport(properties);
    injectReceiptsByChargeCodeReport(properties);
    injectGrossPotentialRentReport(properties);
    injectWorkOrderPerformance(properties);
    injectResidentAgedReceivablesDetailReport(properties);
    injectResidentAgedReceivablesSummaryReport(properties);
}

void writeData(std::vector<Property>& properties)
{
    writeOverviewReport(properties);
    writeAvailabilityReport(properties);
    writeResidentRetentionReport(properties);
}

int main(int argc, const char **argv) 
{
    init();

    std::vector<Property> properties;

    injectData(properties);

    writeData(properties);

    return 0;
}
