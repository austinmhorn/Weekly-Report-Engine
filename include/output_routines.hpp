#ifndef __output_routines__hpp
#define __output_routines__hpp

#include <iostream>
#include <map>

#include "Property.hpp"
#include "helper_functions.hpp"
#include "dictionaries.hpp"

static int TOTAL_NUMBER_ROWS_WRITTEN = 0;

#define PRINT_TOTAL_NUMBER_ROWS_WRITTEN() std::cout << "~ Total # rows written: " << TOTAL_NUMBER_ROWS_WRITTEN << std::endl;

static std::string OUTPUT_DIRECTORY_PATH;

static const std::string COL_1_NAME = "Property,";
static const std::string COL_2_NAME = "Date,";
static const std::string COL_3_NAME = "Metric,";
static const std::string COL_4_NAME = "Value,";
static const std::string COL_5_NAME = "Origin,";
static const std::string COL_6_NAME = "Decoder,";
static const std::string COL_7_NAME = "Section,";

static const std::string ORIGIN_DASHBOARD                        = "Birchstone Dashboard Report";
static const std::string ORIGIN_MOVE_OUT_REASONS                 = "Move Out Reasons Report";
static const std::string ORIGIN_BOX_SCORE_LEAD_CONVERSIONS       = "Box Score - Lead Conversions";
static const std::string ORIGIN_BOX_SCORE_PROPERTY_PULSE         = "Box Score - Property Pulse";
static const std::string ORIGIN_RESIDENT_RETENTION               = "Resident Retention";
static const std::string ORIGIN_INCOME_STATEMENT                 = "Income Statement";
static const std::string ORIGIN_GPR                              = "GPR Report";
static const std::string ORIGIN_RENT_RECEIPTS                    = "Receipts by Charge Code (Rent)";
static const std::string ORIGIN_RESIDENT_AGED_RECEIVABLES_DETAIL = "Resident Aged Receivables (Detail)";

static const std::string SECTION_COLLECTIONS              = "Collections";
static const std::string SECTION_RENEWALS_AND_EXPIRATIONS = "Renewals and Expirations";
static const std::string SECTION_WEEKLY_ACTIVITY          = "Weekly Activity";
static const std::string SECTION_OPERATIONAL_KPI          = "Operational KPI";

static const std::string OUTPUT_OVERVIEW_REPORT_FILENAME                          = "Overview Report.csv";
static const std::string OUTPUT_AVAILABILITY_REPORT_FILENAME                      = "Availability Report.csv";
static const std::string OUTPUT_RESIDENT_RETENTION_REPORT_FILENAME                = "Resident Retention Report.csv";
static const std::string OUTPUT_INCOME_STATEMENT_REPORT_FILENAME                  = "Income Report.csv";
static const std::string OUTPUT_OPERATIONS_REPORT_FILENAME                        = "Operations Report.csv";
static const std::string OUTPUT_GROSS_POTENTIAL_RENT_REPORT_FILENAME              = "GPR Report.csv";
static const std::string OUTPUT_RENT_RECEIPTS_REPORT_FILENAME                     = "Rent Receipts Report.csv";
static const std::string OUTPUT_RESIDENT_AGED_RECEIVABLES_SUMMARY_REPORT_FILENAME = "Resident Aged Receivables Report - (Summary).csv";
static const std::string OUTPUT_WORK_ORDER_PERFORMANCE_REPORT_FILENAME            = "Work Order Performance Report.csv";

static const int PRECISION_PERCENT  = 4;
static const int PRECISION_CURRENCY = 15;

void calcTotalRowsWritten(const std::string& path)
{
    rapidcsv::Document doc(path);
    int numRows = doc.GetRowCount() - 1;
    TOTAL_NUMBER_ROWS_WRITTEN += numRows;
}

std::ofstream& nameAndDate(std::ofstream& outFS, const std::string& name, const std::string& date)
{
    outFS << name << "," << date << ",";
    return outFS;
}

void __writeMoveOutReasons(std::ofstream& outFS, Property& prop, const std::string& name, const std::string& date)
{
    std::vector<std::string> reasons;
    for (auto &moveOut: prop.move)
        reasons.push_back(moveOut.p_reason);

    std::sort(reasons.begin(), reasons.end());

    // Map to store each string and its count
    std::map<std::string, int> reasonCount;

    // Count occurrences of each string
    for (const auto& reason : reasons) 
        reasonCount[reason]++;

    // Create a vector of pairs from the map
    std::vector<std::pair<std::string, int>> result;
    for (const auto& entry : reasonCount) 
        result.push_back(entry);

    // Output the results
    for (const auto& entry : result) 
    {
        //std::cout << entry.first << ": " << entry.second << std::endl;
        nameAndDate(outFS, name, date) << entry.first << "," << entry.second << "," << ORIGIN_MOVE_OUT_REASONS << ",Reason For Move Out," << "Reason For Move Out," << std::endl;
    }
}

void writeOverviewReport(std::vector<Property>& properties)
{
    const std::string path = OUTPUT_DIRECTORY_PATH + OUTPUT_OVERVIEW_REPORT_FILENAME;
    PRINT_WRITE_FILE_MSG(path);

    // Create the destination directory if it doesn't exist
    if (!fs::exists(OUTPUT_DIRECTORY_PATH)) {
        fs::create_directories(OUTPUT_DIRECTORY_PATH);
    }

    std::ofstream outFS;
    outFS.open(path, std::ios::out);

    outFS << COL_1_NAME << COL_2_NAME << COL_3_NAME << COL_4_NAME << COL_5_NAME << COL_6_NAME << COL_7_NAME << std::endl;

    for (auto &prop: properties)
    {
        // Ignore "Company" Property struct created from Birchstone Dashboard Report
        if (prop.p_dict_key != static_cast<unsigned>(DashboardReportDictionary::Company))
        {
            const char* name = overviewReportNameMap.at(static_cast<OverviewReportDictionary>(prop.p_dict_key));

            ///< SECTION: Property Attributes
            
            ///< SECTION: Operational KPIs
            outFS.precision(PRECISION_PERCENT);
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Occupied %," << prop.dash.p_occupied << "," << ORIGIN_DASHBOARD << "," << "Occupancy," << SECTION_OPERATIONAL_KPI << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "28 Day Trend," << prop.dash.p_28_day_trend << "," << ORIGIN_DASHBOARD << "," << "Trended Occ - 30 Days," << SECTION_OPERATIONAL_KPI << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "56 Day Trend," << prop.dash.p_56_day_trend << "," << ORIGIN_DASHBOARD << "," << "Trended Occ - 60 Days," << SECTION_OPERATIONAL_KPI << std::endl;

            ///< SECTION: Collections
            float totalPotentialRent = 0;
            float totalVacanyLoss = 0;

            for (auto &entry: prop.gpr)
            {
                totalPotentialRent += entry.p_total_potential_rent;
                totalVacanyLoss += entry.p_vacany;
            }

            outFS.precision(PRECISION_CURRENCY);

            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Sum of Total Potential Rent," << totalPotentialRent << "," << ORIGIN_GPR << "," << "01. Total Potential Rent," << SECTION_COLLECTIONS << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Sum of Vacancy," << totalVacanyLoss << "," << ORIGIN_GPR << "," << "02. Vacancy Loss," << SECTION_COLLECTIONS << std::endl;

            float totalRentCharges = 0;
            float totalRentCollected = 0;

            for (auto &entry: prop.receipts)
            {
                totalRentCharges += entry.p_total_charges;
                totalRentCollected += entry.p_total_collections;
            }

            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Sum of Total Charges," << totalRentCharges << "," << ORIGIN_RENT_RECEIPTS << "," << "03. Total Rent Charges," << SECTION_COLLECTIONS << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Sum of Total Collections," << totalRentCollected << "," << ORIGIN_RENT_RECEIPTS << "," << "04. Total Rent Collected," << SECTION_COLLECTIONS << std::endl;

            float totalPercentCollected = totalRentCollected / totalRentCharges;

            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "% Collected," << totalPercentCollected << "," << ORIGIN_RENT_RECEIPTS << "," << "05. % Collected," << SECTION_COLLECTIONS << std::endl;

            float totalDelinquent = 0;
            float totalDelinquentCurrent = 0;
            float totalDelinquentEviction = 0;
            float netCurrentBalance = 0;
            float net31to60Balance = 0;
            float net61to90Balance = 0;
            float net90PlusBalance = 0;

            for (auto &entry: prop.receivablesDetail)
            {
                totalDelinquent += entry.p_delinquent_total;
                netCurrentBalance += entry.p_0_to_30_days;
                net31to60Balance += entry.p_31_to_60_days;
                net61to90Balance += entry.p_61_to_90_days;
                net90PlusBalance += entry.p_90_plus_days;

                if ( entry.p_lease_status.find("Eviction") != SIZE_MAX )
                    totalDelinquentEviction += entry.p_delinquent_total;
                else
                    totalDelinquentCurrent += entry.p_delinquent_total;
            }

            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Sum of Delinquent Total," << totalDelinquent << "," << ORIGIN_RESIDENT_AGED_RECEIVABLES_DETAIL << "," << "06. Delinquent Total," << SECTION_COLLECTIONS << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Total Delinquent - Eviction," << totalDelinquentEviction << "," << ORIGIN_RESIDENT_AGED_RECEIVABLES_DETAIL << "," << "07. Total Delinquent - Eviction," << SECTION_COLLECTIONS << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Total Delinquent - Current," << totalDelinquentCurrent << "," << ORIGIN_RESIDENT_AGED_RECEIVABLES_DETAIL << "," << "08. Total Delinquent - Current," << SECTION_COLLECTIONS << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Sum of Net Current Balance," << netCurrentBalance << "," << ORIGIN_RESIDENT_AGED_RECEIVABLES_DETAIL << "," << "09. Net Current Balance," << SECTION_COLLECTIONS << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Sum of Net 31-60 Day Balance," << net31to60Balance << "," << ORIGIN_RESIDENT_AGED_RECEIVABLES_DETAIL << "," << "10. Net 31-60 Day Balance," << SECTION_COLLECTIONS << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Sum of Net 61-90 Day Balance," << net61to90Balance << "," << ORIGIN_RESIDENT_AGED_RECEIVABLES_DETAIL << "," << "11. Net 61-90 Day Balance," << SECTION_COLLECTIONS << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Sum of Net 90+ Balance," << net90PlusBalance << "," << ORIGIN_RESIDENT_AGED_RECEIVABLES_DETAIL << "," << "12. Net 90+ Day Balance," << SECTION_COLLECTIONS << std::endl;

            outFS.unsetf(std::ios_base::floatfield);

            ///< SECTION: Section: Weekly Activity
            int totalCancelledApps = prop.leadConversions.p_apps_approved_cancelled + prop.leadConversions.p_apps_completed_cancelled;

            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Open Work Orders," << prop.dash.p_open_work_orders << "," << ORIGIN_DASHBOARD << "," << "Outstanding Work Orders," << SECTION_WEEKLY_ACTIVITY << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Application - Completed," << prop.leadConversions.p_apps_completed << "," << ORIGIN_BOX_SCORE_LEAD_CONVERSIONS << "," << "Completed Applications," << SECTION_WEEKLY_ACTIVITY << std::endl; 
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Application - Denied," << prop.leadConversions.p_apps_denied << "," << ORIGIN_BOX_SCORE_LEAD_CONVERSIONS << "," << "Denied Applications," << SECTION_WEEKLY_ACTIVITY << std::endl; 
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Application - Approved (Cancelled)," << totalCancelledApps << "," << ORIGIN_BOX_SCORE_LEAD_CONVERSIONS << "," << "Cancelled Applications," << SECTION_WEEKLY_ACTIVITY << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Application - Approved," << prop.leadConversions.p_apps_approved << "," << ORIGIN_BOX_SCORE_LEAD_CONVERSIONS << "," << "Approved Applications," << SECTION_WEEKLY_ACTIVITY << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Lease - Completed," << prop.leadConversions.p_lease_completed << "," << ORIGIN_BOX_SCORE_LEAD_CONVERSIONS << "," << "Signed Leases," << SECTION_WEEKLY_ACTIVITY << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Move Ins," << prop.pulse.p_move_ins << "," << ORIGIN_BOX_SCORE_PROPERTY_PULSE << "," << "Move Ins," << SECTION_WEEKLY_ACTIVITY << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Move Outs," << prop.pulse.p_move_outs << "," << ORIGIN_BOX_SCORE_PROPERTY_PULSE << "," << "Move Outs," << SECTION_WEEKLY_ACTIVITY << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << "Skips," << prop.pulse.p_skips << "," << ORIGIN_BOX_SCORE_PROPERTY_PULSE << "," << "Skips," << SECTION_WEEKLY_ACTIVITY << std::endl;

            ///< SECTION:: Reason For Move Out
            __writeMoveOutReasons(outFS, prop, name, sExportDate_Forward_Slash_Format);
        }
    }

    outFS.unsetf(std::ios_base::floatfield);

    calcTotalRowsWritten(path);
    
    outFS.close();
}

/*
void writeAvailabilityReport(std::vector<Property>& properties)
{
    const std::string path = OUTPUT_DIRECTORY_PATH + OUTPUT_AVAILABILITY_REPORT_FILENAME;
    PRINT_WRITE_FILE_MSG(path);

    std::ofstream outFS;
    outFS.open(path, std::ios::out);

    outFS << "Property," 
          << "Date," 
          << "Excluded," 
          << "Status," 
          << "Exclusion Name,"
          << "Bldg-Unit," 
          << "Unit Type," 
          << "Floorplan," 
          << "SQFT," 
          << "Market Rent," 
          << "Prior Lease Rent," 
          << "Move-Out," 
          << "Days Vacant," 
          << "Est. Vacancy Cost," 
          << "Available On," 
          << "Scheduled Move-In,"
          << "Days Until Move-In,"
          << "Turn Time [MO-Avail],"
          << "Available Date Exclusion,"
          << "Best Price," 
          << "Budgeted Rent," 
          << "Future Lease Rent," 
          << "Lease Completed" 
          << std::endl;

    std::string dateAsStr;

    for (auto &prop: properties)
    {
        for (auto &entry: prop.availability)
        {
            const char* name = overviewReportNameMap.at(static_cast<OverviewReportDictionary>(prop.p_dict_key));

            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << ((entry.p_status == "Excluded") ? "Yes" : "No") << ","
                                                                       << entry.p_status << ","
                                                                       << entry.p_exclusion_name << ","
                                                                       << entry.p_building_unit << ","
                                                                       << entry.p_unit_type << ","
                                                                       << entry.p_floorplan << ","
                                                                       << entry.p_sqft << ","
                                                                       << entry.p_market_rent << ","
                                                                       << entry.p_prior_lease_rent << ",";
            dateAsStr = tmtostr(entry.p_move_out);
            if ( dateAsStr == "1900/01/00" )
                outFS << ",";
            else 
                outFS << dateAsStr << ",";

            outFS << (int)entry.p_days_vacant << ","
                  << entry.p_est_vacancy_cost << ",";

            dateAsStr = tmtostr(entry.p_available_on);                                            
            if ( dateAsStr == "1900/01/00" )
                outFS << ",";
            else 
                outFS << dateAsStr << ",";

            dateAsStr = tmtostr(entry.p_scheduled_move_in);
            if ( dateAsStr == "1900/01/00" )
                outFS << ",";
            else
                outFS << dateAsStr << ",";

            if ( dateAsStr == "1900/01/00" )
                outFS << ",";
            else
            {
                int daysUntilMoveIn = daysBetweenDates(entry.p_scheduled_move_in);
                if ( daysUntilMoveIn < 0 )
                    outFS << daysUntilMoveIn << ",";
                else
                    outFS << daysUntilMoveIn + 1 << ",";
            }

            if ( entry.p_turn_time_mo_avail_str.size() )
                outFS << entry.p_turn_time_mo_avail << ",";
            else
                outFS << entry.p_turn_time_mo_avail_str << ",";

            outFS << -daysBetweenDates(entry.p_available_on) << ","
                  << entry.p_best_price << ","
                  << entry.p_budgeted_rent << ",";

            if ( entry.p_future_lease_rent_str.size() )
                outFS << entry.p_future_lease_rent << ",";
            else
                outFS << entry.p_future_lease_rent_str << ",";

            dateAsStr = tmtostr(entry.p_lease_completed);
            if ( dateAsStr == "1900/01/00" )
                outFS << "," << std::endl;
            else 
                outFS << dateAsStr << "," << std::endl;
        }
    }

    calcTotalRowsWritten(path);

    outFS.close();
}
*/

void writeResidentRetentionReport(std::vector<Property>& properties)
{
    const std::string path = OUTPUT_DIRECTORY_PATH + OUTPUT_RESIDENT_RETENTION_REPORT_FILENAME;

    PRINT_WRITE_FILE_MSG(path);

    std::ofstream outFS;
    outFS.open(path, std::ios::out);
    outFS << "Property," 
          << "Date," 
          << "Period," 
          << "Gross Expiring Leases," 
          << "Early Move Out," 
          << "Early Move Out %," 
          << "NTV," 
          << "NTV %," 
          << "Renewal," 
          << "Renewal %," 
          << "Transfers," 
          << "Transfers %," 
          << "Remaining Expiration," 
          << "Remaining Expiration %," 
          << "MTM," 
          << "MTM %," 
          << std::endl;

    for (auto &prop: properties) {
        for (auto &entry: prop.retention) {
            outFS << overviewReportNameMap.at(static_cast<OverviewReportDictionary>(prop.p_dict_key)) << ","
                  << sExportDate_Forward_Slash_Format << ","
                  << transformToMonthAndYear(entry.p_month) << ","
                  << entry.p_expiring_leases << ","
                  << entry.p_early_move_out << ","
                  << static_cast<float>(entry.p_early_move_out) / static_cast<float>(entry.p_expiring_leases) << ","
                  << entry.p_ntv << ","
                  << static_cast<float>(entry.p_ntv) / static_cast<float>(entry.p_expiring_leases) << ","
                  << entry.p_renewals + entry.p_renewal_transfers << ","
                  << static_cast<float>(entry.p_renewals + entry.p_renewal_transfers) / static_cast<float>(entry.p_expiring_leases) << ","
                  << entry.p_transfers << ","
                  << static_cast<float>(entry.p_transfers) / static_cast<float>(entry.p_expiring_leases) << ","
                  << entry.p_pending << ","
                  << static_cast<float>(entry.p_pending) / static_cast<float>(entry.p_expiring_leases) << ","
                  << entry.p_mtm << ","
                  << static_cast<float>(entry.p_mtm) / static_cast<float>(entry.p_expiring_leases) << "," 
                  << std::endl;
        }
    }

    calcTotalRowsWritten(path);

    outFS.close();
}

void writeIncomeStatementReport(std::vector<Property>& properties)
{
    const std::string path = OUTPUT_DIRECTORY_PATH + OUTPUT_INCOME_STATEMENT_REPORT_FILENAME;

    PRINT_WRITE_FILE_MSG(path)

    std::ofstream outFS;
    outFS.open(path, std::ios::out);
    outFS.precision(30);
    outFS << "Property," << "Date," << "Period," << "Value," << std::endl;

    for (auto& prop: properties) {
        const char* name = overviewReportNameMap.at(static_cast<OverviewReportDictionary>(prop.p_dict_key));
        // Ignore "Company" Property struct created from Birchstone Dashboard Report
        if (prop.p_dict_key != static_cast<unsigned>(DashboardReportDictionary::Company)) {
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << transformToMonthAndYear(prop.income.p_balance_11_months_ago.first) << "," << prop.income.p_balance_11_months_ago.second << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << transformToMonthAndYear(prop.income.p_balance_10_months_ago.first) << "," << prop.income.p_balance_10_months_ago.second << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << transformToMonthAndYear(prop.income.p_balance_9_months_ago.first) << "," << prop.income.p_balance_9_months_ago.second << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << transformToMonthAndYear(prop.income.p_balance_8_months_ago.first) << "," << prop.income.p_balance_8_months_ago.second << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << transformToMonthAndYear(prop.income.p_balance_7_months_ago.first) << "," << prop.income.p_balance_7_months_ago.second << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << transformToMonthAndYear(prop.income.p_balance_6_months_ago.first) << "," << prop.income.p_balance_6_months_ago.second << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << transformToMonthAndYear(prop.income.p_balance_5_months_ago.first) << "," << prop.income.p_balance_5_months_ago.second << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << transformToMonthAndYear(prop.income.p_balance_4_months_ago.first) << "," << prop.income.p_balance_4_months_ago.second << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << transformToMonthAndYear(prop.income.p_balance_3_months_ago.first) << "," << prop.income.p_balance_3_months_ago.second << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << transformToMonthAndYear(prop.income.p_balance_2_months_ago.first) << "," << prop.income.p_balance_2_months_ago.second << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << transformToMonthAndYear(prop.income.p_balance_1_months_ago.first) << "," << prop.income.p_balance_1_months_ago.second << std::endl;
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << transformToMonthAndYear(prop.income.p_balance_0_months_ago.first) << "," << prop.income.p_balance_0_months_ago.second << std::endl;
        }
    }

    outFS.unsetf(std::ios_base::floatfield);

    calcTotalRowsWritten(path);

    outFS.close();
}

void writeOperationsReport(std::vector<Property>& properties)
{
    const std::string path = OUTPUT_DIRECTORY_PATH + OUTPUT_OPERATIONS_REPORT_FILENAME;

    PRINT_WRITE_FILE_MSG(path);
    std::ofstream outFS(path, std::ios::out);

    outFS << "Property," << "Date," << "Lead Source," << "New Leads," << "First Visit/Tour" << std::endl;

    for (auto& prop: properties) {
        const char* name = overviewReportNameMap.at(static_cast<OverviewReportDictionary>(prop.p_dict_key));
        // Ignore "Company" Property struct created from Birchstone Dashboard Report
        if (prop.p_dict_key != static_cast<unsigned>(DashboardReportDictionary::Company)) {
            for (auto &entry: prop.ops) {
                nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << entry.p_lead_source << "," << entry.p_new_leads << "," << entry.p_first_visit_tour << std::endl;
            }
        }
    }

    calcTotalRowsWritten(path);

    outFS.close();
}

void writeWorkOrderPerformanceReport(std::vector<Property>& properties)
{
    const std::string path = OUTPUT_DIRECTORY_PATH + OUTPUT_WORK_ORDER_PERFORMANCE_REPORT_FILENAME;

    PRINT_WRITE_FILE_MSG(path);
    std::ofstream outFS(path, std::ios::out);

    outFS.precision(30);

    outFS << "Property,"
          << "Date,"
          << "Priority,"
          << "Response - Previous Work Orders,"
          << "Response - New Work Orders,"
          << "Response - On Time,"
          << "Response - % On Time,"
          << "Response - Late,"
          << "Response - Total,"
          << "Response - Not Started,"
          << "Completion - Previous Work Orders,"
          << "Completion - New Work Orders,"
          << "Completion - On Time,"
          << "Completion - % On Time,"
          << "Completion - Late,"
          << "Completion - Total,"
          << "Completion - Not Complete,"
          << "Completion - Exceptions"
          << std::endl;

    for (auto &prop: properties)
    {
        const char* name = overviewReportNameMap.at(static_cast<OverviewReportDictionary>(prop.p_dict_key));

        for (auto &entry: prop.workOrder)
        {
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << entry.p_priority << ","
                                                  << entry.p_response_previous_work_orders << ","
                                                  << entry.p_response_new_work_orders << ","
                                                  << entry.p_response_on_time << ","
                                                  << entry.p_response_percent_on_time << ","
                                                  << entry.p_response_late << ","
                                                  << entry.p_response_total << ","
                                                  << entry.p_response_not_started << ","
                                                  << entry.p_completion_previous_work_orders << ","
                                                  << entry.p_completion_new_work_orders << ","
                                                  << entry.p_completion_on_time << ","
                                                  << entry.p_completion_percent_on_time << ","
                                                  << entry.p_completion_late << ","
                                                  << entry.p_completion_total << ","
                                                  << entry.p_completion_not_complete << ","
                                                  << entry.p_completion_exceptions << ","
                                                  << std::endl;
        }
    }

    outFS.unsetf(std::ios_base::floatfield);

    calcTotalRowsWritten(path);

    outFS.close();
}

/*
void writeGrossPotentialRentReport(std::vector<Property>& properties)
{
    const std::string path = OUTPUT_DIRECTORY_PATH + OUTPUT_GROSS_POTENTIAL_RENT_REPORT_FILENAME;

    PRINT_WRITE_FILE_MSG(path);
    std::ofstream outFS(path, std::ios::out);

    outFS << "Property,"
          << "Date,"
          << "Unit Space Count,"
          << "GPR Market Rent,"
          << "Total Potential Rent,"
          << "Gain/Loss to Lease,"
          << "Days Vacant,"
          << "Vacancy,"
          << "Vacancy Adjustments,"
          << "Out of Period Rent,"
          << "Scheduled Rent,"
          << "Rent Adjustments,"
          << "Rent Write-offs,"
          << "Rental Income,"
          << std::endl;

    outFS.precision(30);

    for (auto &prop: properties)
    {
        const char* name = overviewReportNameMap.at(static_cast<OverviewReportDictionary>(prop.p_dict_key));

        for (auto &entry: prop.gpr)
        {
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << entry.p_unit_space_count << ","
                                                  << entry.p_gpr_market_rent << ","
                                                  << entry.p_total_potential_rent << ","
                                                  << entry.p_gain_loss_to_lease << ","
                                                  << entry.p_days_vacant << ","
                                                  << entry.p_vacany << ","
                                                  << entry.p_vacany_adjustments << ","
                                                  << entry.p_out_of_period_rent << ","
                                                  << entry.p_scheduled_rent << ","
                                                  << entry.p_rent_adjustments << ","
                                                  << entry.p_rent_write_offs << ","
                                                  << entry.p_rental_income << std::endl;
        }
    }

    outFS.unsetf(std::ios_base::floatfield);

    outFS.close();
}

void writeRentReceipts(std::vector<Property>& properties)
{
    const std::string path = OUTPUT_DIRECTORY_PATH + OUTPUT_RENT_RECEIPTS_REPORT_FILENAME;

    PRINT_WRITE_FILE_MSG(path);
    std::ofstream outFS(path, std::ios::out);

    outFS.precision(30);

    outFS << "Property,"
          << "Date,"
          << "Charge Code,"
          << "GL Account,"
          << "Total Charges,"
          << "Total Cash Collections,"
          << "Credits/Concessions,"
          << "Total Collections,"
          << "% Collected,"
          << std::endl;

    for (auto &prop: properties)
    {
        const char* name = overviewReportNameMap.at(static_cast<OverviewReportDictionary>(prop.p_dict_key));

        for (auto &entry: prop.receipts)
        {
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << entry.p_charge_code << ","
                                                  << entry.p_gl_account << ","
                                                  << entry.p_total_charges << ","
                                                  << entry.p_total_cash_collections << ","
                                                  << entry.p_credits_x_concessions << ","
                                                  << entry.p_total_collections << ","
                                                  << entry.p_percent_collected << ","
                                                  << std::endl;
        }
    }

    outFS.unsetf(std::ios_base::floatfield);

    outFS.close();
}

void writeResidentAgedReceivablesSummary(std::vector<Property>& properties)
{
    const std::string path = OUTPUT_DIRECTORY_PATH + OUTPUT_RESIDENT_AGED_RECEIVABLES_SUMMARY_REPORT_FILENAME;

    PRINT_WRITE_FILE_MSG(path);
    std::ofstream outFS(path, std::ios::out);

    outFS.precision(30);

    outFS << "Property,"
          << "Date,"
          << "Lease Status,"
          << "Unallocated Charges / Credits,"
          << "0-30 Days,"
          << "31-60 Days,"
          << "61-90 Days,"
          << "90+ Days,"
          << "Pre-Payments,"
          << "Balance,"
          << std::endl;

    for (auto &prop: properties)
    {
        const char* name = overviewReportNameMap.at(static_cast<OverviewReportDictionary>(prop.p_dict_key));

        for (auto &entry: prop.receivablesSummary)
        {
            nameAndDate(outFS, name, sExportDate_Forward_Slash_Format) << entry.p_lease_status << ","
                                                  << entry.p_unallocated_charges_x_credits << ","
                                                  << entry.p_0_to_30_days << ","
                                                  << entry.p_31_to_60_days << ","
                                                  << entry.p_61_to_90_days << ","
                                                  << entry.p_90_plus_days << ","
                                                  << entry.p_pre_payments << ","
                                                  << entry.p_balance << ","
                                                  << std::endl;
        }
    }

    outFS.unsetf(std::ios_base::floatfield);

    outFS.close();
}
*/


#endif /* __output_routines__hpp */