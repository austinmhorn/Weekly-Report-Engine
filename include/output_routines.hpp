#ifndef __output_routines__hpp
#define __output_routines__hpp

#include <iostream>
#include <map>

#include "Property.hpp"
#include "helper_functions.hpp"
#include "dictionaries.hpp"

static std::string sExportDate = getFormattedDate();

static const std::string OUTPUT_DIRECTORY_PATH = "data/" + getFormattedDate(".") + "/bin/";

static const std::string COL_1_NAME = "Property,";
static const std::string COL_2_NAME = "Date,";
static const std::string COL_3_NAME = "Metric,";
static const std::string COL_4_NAME = "Value,";
static const std::string COL_5_NAME = "Origin,";
static const std::string COL_6_NAME = "Decoder,";
static const std::string COL_7_NAME = "Section,";

static const std::string ORIGIN_DASHBOARD                  = "Birchstone Dashboard Report";
static const std::string ORIGIN_MOVE_OUT_REASONS           = "Move Out Reasons Report";
static const std::string ORIGIN_BOX_SCORE_LEAD_CONVERSIONS = "Box Score - Lead Conversions";
static const std::string ORIGIN_BOX_SCORE_PROPERTY_PULSE   = "Box Score - Property Pulse";
static const std::string ORIGIN_RESIDENT_RETENTION         = "Resident Retention";
static const std::string ORIGIN_INCOME_STATEMENT           = "Income Statement";

static const std::string SECTION_COLLECTIONS              = "Collections";
static const std::string SECTION_RENEWALS_AND_EXPIRATIONS = "Renewals and Expirations";
static const std::string SECTION_WEEKLY_ACTIVITY          = "Weekly Activity";
static const std::string SECTION_OPERATIONAL_KPI          = "Operational KPI";

static const std::string OUTPUT_OVERVIEW_REPORT_FILENAME           = "Overview Report.csv";
static const std::string OUTPUT_AVAILABILITY_REPORT_FILENAME       = "Availability Report.csv";
static const std::string OUTPUT_RESIDENT_RETENTION_REPORT_FILENAME = "Resident Retention Report.csv";
static const std::string OUTPUT_INCOME_STATEMENT_REPORT_FILENAME   = "Income Report.csv";
static const std::string OUTPUT_OPERATIONS_REPORT_FILENAME         = "Operations Report.csv";

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
            nameAndDate(outFS, name, sExportDate) << "Occupied %," << prop.dash.p_occupied << "," << ORIGIN_DASHBOARD << "," << "Occupancy," << SECTION_OPERATIONAL_KPI << std::endl;
            nameAndDate(outFS, name, sExportDate) << "28 Day Trend," << prop.dash.p_28_day_trend << "," << ORIGIN_DASHBOARD << "," << "Trended Occ - 30 Days," << SECTION_OPERATIONAL_KPI << std::endl;
            nameAndDate(outFS, name, sExportDate) << "56 Day Trend," << prop.dash.p_56_day_trend << "," << ORIGIN_DASHBOARD << "," << "Trended Occ - 60 Days," << SECTION_OPERATIONAL_KPI << std::endl;

            outFS.precision(30);

            ///< SECTION: Collections
            nameAndDate(outFS, name, sExportDate) << "Current Month Collections," << prop.dash.p_current_month_collections << "," << ORIGIN_DASHBOARD << "," << "Total Delinquent - Current," << SECTION_COLLECTIONS << std::endl;
            nameAndDate(outFS, name, sExportDate) << "Delinquency (As of Today)," << prop.dash.p_delinquency_1 << "," << ORIGIN_DASHBOARD << "," << "Delinquent Total," << SECTION_COLLECTIONS << std::endl;
            nameAndDate(outFS, name, sExportDate) << "Delinquency (End of Last Month)," << prop.dash.p_delinquency_2 << "," << ORIGIN_DASHBOARD << "," << "Net 31-60 Day Balance," << SECTION_COLLECTIONS << std::endl;
            nameAndDate(outFS, name, sExportDate) << "Delinquency (End of 2 Months Ago)," << prop.dash.p_delinquency_3 << "," << ORIGIN_DASHBOARD << "," << "Net 90+ Day Balance," << SECTION_COLLECTIONS << std::endl;
            nameAndDate(outFS, name, sExportDate) << "Evictions (Total)," << prop.dash.p_evictions_total << "," << ORIGIN_DASHBOARD << "," << "Total Delinquent - Eviction," << SECTION_COLLECTIONS << std::endl;
            nameAndDate(outFS, name, sExportDate) << "Total Billed," << prop.dash.p_total_billed << "," << ORIGIN_DASHBOARD << "," << "Total Scheduled Charges," << SECTION_COLLECTIONS << std::endl;

            outFS.unsetf(std::ios_base::floatfield);

            ///< SECTION: Section: Weekly Activity
            nameAndDate(outFS, name, sExportDate) << "Open Work Orders," << prop.dash.p_open_work_orders << "," << ORIGIN_DASHBOARD << "," << "Outstanding Work Orders," << SECTION_WEEKLY_ACTIVITY << std::endl;
            nameAndDate(outFS, name, sExportDate) << "Total # Pending Move-Ins," << prop.dash.p_total_num_pending_move_ins << "," << ORIGIN_DASHBOARD << "," << "Move Ins," << SECTION_WEEKLY_ACTIVITY << std::endl;
            nameAndDate(outFS, name, sExportDate) << "# Move-Outs," << prop.move.size() << "," << ORIGIN_DASHBOARD << "," << "Move Outs," << SECTION_WEEKLY_ACTIVITY << std::endl;
            nameAndDate(outFS, name, sExportDate) << "Application - Completed," << prop.leadConversions.p_apps_completed << "," << ORIGIN_BOX_SCORE_LEAD_CONVERSIONS << "," << "Completed Applications," << SECTION_WEEKLY_ACTIVITY << std::endl; 
            nameAndDate(outFS, name, sExportDate) << "Application - Denied," << prop.leadConversions.p_apps_denied << "," << ORIGIN_BOX_SCORE_LEAD_CONVERSIONS << "," << "Denied Applications," << SECTION_WEEKLY_ACTIVITY << std::endl; 

            int totalCancelledApps = prop.leadConversions.p_apps_approved_cancelled + prop.leadConversions.p_apps_completed_cancelled;

            nameAndDate(outFS, name, sExportDate) << "Application - Approved (Cancelled)," << totalCancelledApps << "," << ORIGIN_BOX_SCORE_LEAD_CONVERSIONS << "," << "Cancelled Applications," << SECTION_WEEKLY_ACTIVITY << std::endl;
            nameAndDate(outFS, name, sExportDate) << "Application - Approved," << prop.leadConversions.p_apps_approved << "," << ORIGIN_BOX_SCORE_LEAD_CONVERSIONS << "," << "Approved Applications," << SECTION_WEEKLY_ACTIVITY << std::endl;
            nameAndDate(outFS, name, sExportDate) << "Lease - Completed," << prop.leadConversions.p_lease_completed << "," << ORIGIN_BOX_SCORE_LEAD_CONVERSIONS << "," << "Signed Leases," << SECTION_WEEKLY_ACTIVITY << std::endl;
            nameAndDate(outFS, name, sExportDate) << "MTM," << prop.pulse.p_mtm << "," << ORIGIN_BOX_SCORE_PROPERTY_PULSE << "," << "MTM," << SECTION_RENEWALS_AND_EXPIRATIONS << std::endl;
            nameAndDate(outFS, name, sExportDate) << "Skips," << prop.pulse.p_skips << "," << ORIGIN_BOX_SCORE_PROPERTY_PULSE << "," << "Skips - Actual," << SECTION_WEEKLY_ACTIVITY << std::endl;

            ///< SECTION:: Reason For Move Out
            __writeMoveOutReasons(outFS, prop, name, sExportDate);
        }
    }
    
    outFS.close();
}

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

            nameAndDate(outFS, name, sExportDate) << ((entry.p_status == "Excluded") ? "Yes" : "No") << ","
                                                  << entry.p_status << ","
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

            outFS << entry.p_best_price << ","
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


    outFS.close();
}

#endif /* __output_routines__hpp */