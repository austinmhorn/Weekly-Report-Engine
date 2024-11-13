#ifndef injection_routines_hpp
#define injection_routines_hpp

#include <sstream>
#include <vector>

#include "rapidcsv.h"

#include "Property.hpp"
#include "dictionaries.hpp"
#include "helper_functions.hpp"

static const std::string DASHBOARD_REPORT_FILENAME                  = "data/" + getFormattedDate(".") + "/clean/Birchstone Dashboard.csv";
static const std::string AVAILABILITY_REPORT_FILENAME               = "data/" + getFormattedDate(".") + "/clean/Availability.csv";
static const std::string MOVE_OUT_REASONS_REPORT_FILENAME           = "data/" + getFormattedDate(".") + "/clean/Move Out Reasons.csv";
static const std::string BOX_SCORE_LEAD_CONVERSIONS_FILENAME        = "data/" + getFormattedDate(".") + "/clean/Box Score - Lead Conversions.csv";
static const std::string BOX_SCORE_MAKE_READY_STATUS_FILENAME       = "data/" + getFormattedDate(".") + "/clean/Box Score - Make Ready Status.csv";
static const std::string BOX_SCORE_PROPERTY_PULSE_FILENAME          = "data/" + getFormattedDate(".") + "/clean/Box Score - Property Pulse.csv";
static const std::string RESIDENT_RETENTION_FILENAME                = "data/" + getFormattedDate(".") + "/clean/Resident Retention.csv";
static const std::string RECEIPTS_BY_CHARGE_CODE_FILENAME           = "data/" + getFormattedDate(".") + "/clean/Receipts By Charge Code (Rent).csv";
static const std::string GROSS_POTENTIAL_RENT_FILENAME              = "data/" + getFormattedDate(".") + "/clean/Gross Potential Rent.csv";
static const std::string WORK_ORDER_PERFORMANCE_FILENAME            = "data/" + getFormattedDate(".") + "/clean/Work Order Performance.csv";
static const std::string RESIDENT_AGED_RECEIVABLES_DETAIL_FILENAME  = "data/" + getFormattedDate(".") + "/clean/Resident Aged Receivables (Detail).csv";
static const std::string RESIDENT_AGED_RECEIVABLES_SUMMARY_FILENAME = "data/" + getFormattedDate(".") + "/clean/Resident Aged Receivables (Summary).csv";

/// @brief  Inject Dashboard Report data into vector
/// @param properties 
void injectDashboardReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    rapidcsv::Document doc(DASHBOARD_REPORT_FILENAME, rapidcsv::LabelParams(0, 0));
    std::vector<std::string> columnNames = doc.GetColumnNames();

    std::sort(columnNames.begin(), columnNames.end());
    
    unsigned dict_key = 1;
    
    for (auto &name : columnNames)
    {
        std::vector<std::string> col = doc.GetColumn<std::string>(name);
        std::stringstream ss;
        Property newProperty;
        std::vector<int> emptyRows = {9,15,18,27,31,35,43};
        int row_num = 2;
        std::string stringified_currency;

        //for (int i = 0; i < static_cast<unsigned>(DashboardReportDictionary::Count); i++) {
        //    if ( name == static_cast<const char *>(dashboardReportPropertyNameMap.at(static_cast<DashboardReportDictionary>(i))) ) {
        //        std::cout << "On iteration " << i << " - " << name << std::endl;
        //    }
        //}
        
        newProperty.p_dict_key = dict_key;
         // Set Property name
        newProperty.dash.p_name = name;
        newProperty.p_official_marketing_name = conversionReportPropertyNameMap.at(static_cast<ConversionReportDictionary>(newProperty.p_dict_key));

        //std::cout << dict_key << ". " << name << std::endl;

        dict_key++;

        // Iterate through rows and feed data to string stream
        for (auto &row : col)
        {
            bool insert = true;

            for(auto &num: emptyRows)
                if (num == row_num)
                    insert = false;

            if (insert)
                ss << row << ' ';

            row_num++;
        }

        ss >> newProperty.dash.p_occupied;
        ss >> newProperty.dash.p_14_day_trend;
        ss >> newProperty.dash.p_28_day_trend;
        ss >> newProperty.dash.p_56_day_trend;
        ss >> newProperty.dash.p_14_day_trend_with_apps;
        ss >> newProperty.dash.p_28_day_trend_with_apps;
        ss >> newProperty.dash.p_56_day_trend_with_apps;

        ss >> newProperty.dash.p_30_day_traffic_count;
        ss >> newProperty.dash.p_30_day_closing_ratio;
        ss >> newProperty.dash.p_occupied_rent;
        ss >> newProperty.dash.p_previous_30_day_move_ins;
        ss >> newProperty.dash.p_total_num_pending_move_ins;

        ss >> newProperty.dash.p_open_work_orders;
        ss >> newProperty.dash.p_percent_work_orders_completed_on_time;

        ss >> stringified_currency;
            stringified_currency = removeCommas(stringified_currency);
            newProperty.dash.p_total_billed = std::stof(stringified_currency);
        ss >> stringified_currency; 
            stringified_currency = removeCommas(stringified_currency);
            newProperty.dash.p_current_month_collections = std::stof(stringified_currency);

        ss >> newProperty.dash.p_percent_collected;
        ss >> stringified_currency; 
            stringified_currency = removeCommas(stringified_currency);
            newProperty.dash.p_delinquency_1 = std::stof(stringified_currency);
        ss >> stringified_currency;
            stringified_currency = removeCommas(stringified_currency);
            newProperty.dash.p_delinquency_2 = std::stof(stringified_currency);
        ss >> stringified_currency;
            stringified_currency = removeCommas(stringified_currency);
            newProperty.dash.p_delinquency_3 = std::stof(stringified_currency);
        ss >> newProperty.dash.p_evictions_current;
        ss >> newProperty.dash.p_evictions_total;

        ss >> newProperty.dash.p_notice_unrented;
        ss >> newProperty.dash.p_vacant_unrented_not_ready;
        ss >> newProperty.dash.p_vacant_unrented_ready;

        ss >> stringified_currency;
            stringified_currency = removeCommas(stringified_currency);
            newProperty.dash.p_ap_total_outstanding = std::stof(stringified_currency);
        ss >> newProperty.dash.p_notice_rented;
        ss >> newProperty.dash.p_vacant_rented_ready;

        ss >> newProperty.dash.p_current_month_renewal_percent;
        ss >> newProperty.dash.p_prior_month_renewal_percent;
        ss >> newProperty.dash.p_expirations_next_month;
        ss >> newProperty.dash.p_renewals_next_month;
        ss >> newProperty.dash.p_expirations_in_2_months;
        ss >> newProperty.dash.p_renewals_in_2_months;
        ss >> newProperty.dash.p_current_number_notice_to_vacate;

        ss >> newProperty.dash.p_total_Ps;
        ss >> newProperty.dash.p_total_Us;
        ss >> newProperty.dash.p_total_Rs;
        ss >> newProperty.dash.p_total_excluded_units;

        properties.push_back(newProperty);
    }

    PRINT_FUNCTION_STOP();
}

void injectAvailabilityReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    rapidcsv::Document doc(AVAILABILITY_REPORT_FILENAME);
    int numRows = doc.GetRowCount();
    int currRow = 0;
    bool finished = false;
    std::vector<AvailabilityData> dataList;

    do {
        try {
            for (; currRow < numRows; currRow++) {
                AvailabilityData data;
                
                data.p_name           = doc.GetCell<std::string>("Property", currRow);
                data.p_building_unit  = doc.GetCell<std::string>("Bldg-Unit", currRow);
                data.p_floorplan      = doc.GetCell<std::string>("Floor Plan", currRow);
                data.p_unit_type      = doc.GetCell<std::string>("Unit Type", currRow);
                data.p_exclusion_name = doc.GetCell<std::string>("Exclusion Name", currRow);
                data.p_days_vacant    = doc.GetCell<int>("Days Vacant", currRow);
                
                data.p_move_out = stotm(
                    doc.GetCell<std::string>("Move-Out", currRow), 
                    "%m/%d/%Y"
                    );
                data.p_available_on = stotm(
                    doc.GetCell<std::string>("Available On", currRow), 
                    "%m/%d/%Y"
                    );
                data.p_budgeted_rent = std::stof(removeCommas(doc.GetCell<std::string>("Budgeted Rent", currRow)));
                data.p_scheduled_move_in = stotm(
                    doc.GetCell<std::string>("Scheduled Move-In", currRow), 
                    "%m/%d/%Y"
                    );

                data.p_unit_notes = doc.GetCell<std::string>("Unit Notes", currRow);
                data.p_unit_activity_notes = doc.GetCell<std::string>("Unit Activity Notes", currRow);
                data.p_hazard_note = doc.GetCell<std::string>("Hazard Note", currRow);
                data.p_est_vacancy_cost = std::stof(removeCommas(doc.GetCell<std::string>("Est. Vacany Cost", currRow)));

                //std::cout << currRow-2 << " " << data.p_name << " " << notes << std::endl;

                dataList.push_back(data);
            }
            finished = true;
        }
        catch (const std::exception& except) {
            //std::cerr << except.what() << std::endl;
            currRow++;
        }
    }
    while( !finished );

    for (auto &data: dataList)
        for (auto &prop: properties)
            if (data.p_name == prop.dash.p_name)
                prop.availability.push_back(data);

    PRINT_NUM_ITEMS_PROCESSED(numRows);

    PRINT_FUNCTION_STOP();
}

void injectMoveOutReasonsReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    // Create a vector of MoveOutData structs to store the results
    std::vector<MoveOutData> moveOutDataList;
    rapidcsv::Document doc(MOVE_OUT_REASONS_REPORT_FILENAME); 

    // Get the number of rows (excluding the header)
    int numRows = doc.GetRowCount();

    for (int i = 0; i < numRows; ++i)
    {
        MoveOutData data;
        
        // Parse and assign values to the struct fields
        data.p_name                     = doc.GetCell<std::string>("Property", i);
        data.p_building_and_unit_number = doc.GetCell<std::string>("Bldg-Unit", i);
        data.p_unit_type                = doc.GetCell<std::string>("Unit Type", i);
        
        // Split resident name into first and last
        std::string fullName = doc.GetCell<std::string>("Resident", i);
        splitName(fullName, data.p_resident_fname, data.p_resident_lname);
        
        // Convert string dates to std::tm
        data.p_notice_given   = stotm(doc.GetCell<std::string>("Notice Given", i));
        data.p_lease_end_date = stotm(doc.GetCell<std::string>("Lease End Date", i));
        data.p_move_out_date  = stotm(doc.GetCell<std::string>("Move Out Date", i));
        
        // Assign remaining values
        data.p_reason            = doc.GetCell<std::string>("Reason", i);
        data.p_early_termination = doc.GetCell<std::string>("Early Termination", i);
        data.p_market_rent       = std::stof(doc.GetCell<std::string>("Market Rent", i));
        data.p_scheduled_rent    = std::stof(doc.GetCell<std::string>("Scheduled Rent", i));

        std::string depositHeldStr = doc.GetCell<std::string>("Deposit Held", i);
        if ( depositHeldStr.find("(") != SIZE_MAX ) { // Value enclosed in parenthesis; interpret as negative
            depositHeldStr.erase(remove( depositHeldStr.begin(),  depositHeldStr.end(), '('),  depositHeldStr.end());
            depositHeldStr.erase(remove( depositHeldStr.begin(),  depositHeldStr.end(), ')'),  depositHeldStr.end());
            data.p_deposit_held = -static_cast<int>(std::stof(depositHeldStr));
        }
        else {
            data.p_deposit_held = std::stof(depositHeldStr);
        }
        
        std::string balanceStr = doc.GetCell<std::string>("Balance", i);
        if ( balanceStr.find("(") != SIZE_MAX ) { // Value enclosed in parenthesis; interpret as negative
            balanceStr.erase(remove( balanceStr.begin(),  balanceStr.end(), '('),  balanceStr.end());
            balanceStr.erase(remove( balanceStr.begin(),  balanceStr.end(), ')'),  balanceStr.end());
            data.p_balance = -static_cast<int>(std::stof(balanceStr));
        }
        else {
            data.p_balance = std::stof(balanceStr);
        }
        
        // Add the data to the list
        moveOutDataList.push_back(data);
    }

    for (auto &moveOut: moveOutDataList)
        for (auto &prop: properties)
            if (moveOut.p_name == prop.dash.p_name)
                prop.move.push_back(moveOut);

    PRINT_NUM_ITEMS_PROCESSED(numRows);

    PRINT_FUNCTION_STOP();
}

void injectBoxScoreLeadConversionsReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    // Load the CSV document; row 0 is the header and no label column
    rapidcsv::Document doc(BOX_SCORE_LEAD_CONVERSIONS_FILENAME, rapidcsv::LabelParams(0, -1));

    std::vector<BoxScoreLeadConversionsData> dataList;

    // Get the total row count (excluding the header row)
    size_t numRows = doc.GetRowCount();

    // Iterate over each row and populate the struct fields
    for (size_t row = 0; row < numRows; ++row) 
    {
        BoxScoreLeadConversionsData data;

        // Parse and assign each field from the corresponding CSV column
        data.p_property_name = doc.GetCell<std::string>("Property", row);
        data.p_apps_partially_completed = doc.GetCell<int>("Application - Partially Completed", row);
        data.p_apps_completed = doc.GetCell<int>("Application - Completed", row);
        data.p_apps_completed_cancelled = doc.GetCell<int>("Application - Completed (Cancelled)", row);
        data.p_apps_denied = doc.GetCell<int>("Application - Denied", row);
        data.p_apps_approved = doc.GetCell<int>("Application - Approved", row);
        data.p_apps_approved_cancelled = doc.GetCell<int>("Application - Approved (Cancelled)", row);
        data.p_lease_completed = doc.GetCell<int>("Lease - Completed", row);
        data.p_lease_completed_cancelled = doc.GetCell<int>("Lease - Completed (Cancelled)", row);
        data.p_lease_approved = doc.GetCell<int>("Lease - Approved", row);

        // Add the populated struct to our data list
        dataList.push_back(data);
    }

    for (auto &prop: properties)
        for (auto &data: dataList)
            if (prop.dash.p_name == data.p_property_name)
                prop.leadConversions = data;

    PRINT_NUM_ITEMS_PROCESSED(numRows);

    PRINT_FUNCTION_STOP();
}

void injectBoxScoreMakeReadyStatusReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    std::vector<BoxScoreMakeReadyStatusData> dataList;
    size_t numRows;
    try 
    {
        // Load the CSV document; row 0 is the header and no label column
        rapidcsv::Document doc(BOX_SCORE_MAKE_READY_STATUS_FILENAME, rapidcsv::LabelParams(0, -1));

        // Get the total row count (excluding the header row)
        numRows = doc.GetRowCount();

        // Iterate over each row and populate the struct fields
        for (size_t row = 0; row < numRows; ++row) 
        {
            BoxScoreMakeReadyStatusData data;

            // Parse and assign each field from the corresponding CSV column
            data.p_property_name = doc.GetCell<std::string>("Property", row);
            data.p_status = doc.GetCell<std::string>("Status", row);
            data.p_vacant_rented = doc.GetCell<int>("Vacant Rented", row);
            data.p_vacant_unrented = doc.GetCell<int>("Vacant Unrented", row);
            data.p_total_vacant = doc.GetCell<int>("Total Vacant", row);
            data.p_percent = doc.GetCell<float>("%", row);

            // Add the populated struct to our data list
            dataList.push_back(data);
        }
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error reading CSV: " << e.what() << std::endl;
    }

    for (auto &prop: properties)
        for (auto &data: dataList)
            if (prop.dash.p_name == data.p_property_name)
                prop.makeReady.push_back(data);

    PRINT_NUM_ITEMS_PROCESSED(numRows);

    PRINT_FUNCTION_STOP();
}

void injectBoxScorePropertyPulseReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    std::vector<BoxScorePropertyPulseData> dataList;
    std::size_t numRows;

    try 
    {
        // Load the CSV document; row 0 is the header and no label column
        rapidcsv::Document doc(BOX_SCORE_PROPERTY_PULSE_FILENAME, rapidcsv::LabelParams(0, -1));

        // Get the total row count (excluding the header row)
        numRows = doc.GetRowCount();

        // Iterate over each row and populate the struct fields
        for (size_t row = 0; row < numRows; ++row) 
        {
            BoxScorePropertyPulseData data;

            // Parse and assign each field from the corresponding CSV column
            data.p_property_name = doc.GetCell<std::string>("Property", row);
            data.p_units = doc.GetCell<int>("Units", row);
            data.p_move_ins = doc.GetCell<int>("Move-Ins", row);
            data.p_mtm = doc.GetCell<int>("MTM", row);
            data.p_mtm_conversions = doc.GetCell<int>("MTM Conversions", row);
            data.p_renewal_offers_completed = doc.GetCell<int>("Renewal Offers Completed", row);
            data.p_transfers = doc.GetCell<int>("Transfers", row);
            data.p_notices = doc.GetCell<int>("Notices", row);
            data.p_move_outs = doc.GetCell<int>("Move-Outs", row);
            data.p_renewal_transfers = doc.GetCell<int>("Renewal Transfers", row);
            data.p_skips = doc.GetCell<int>("Skips", row);
            data.p_evictions = doc.GetCell<int>("Evictions", row);
            data.p_leased = doc.GetCell<int>("Leased", row);

            // Add the populated struct to our data list
            dataList.push_back(data);
        }
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error reading CSV: " << e.what() << std::endl;
    }

    for (auto &prop: properties)
        for (auto &data: dataList)
            if (prop.dash.p_name == data.p_property_name)
                prop.pulse = data;

    PRINT_NUM_ITEMS_PROCESSED(numRows);

    PRINT_FUNCTION_STOP();
}

void injectResidentRetentionReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    std::ifstream file(RESIDENT_RETENTION_FILENAME, std::ios::in);
    std::string line;
    std::vector<ResidentRetentionData> dataList;
    int row = 0;

    while ( !file.eof() )
    {
        std::getline(file, line);
        std::stringstream ss(line);

        if ( !line.size() ) // Empty row
        {
            break;
        }
        else if ( row != 0 )
        {
            std::string token;
            ResidentRetentionData data;

            std::getline(ss, token, ',');
            data.p_name = trimDoubleQuotes(token);

            std::getline(ss, token, ',');
            data.p_month = trimDoubleQuotes(token);

            std::getline(ss, token, ',');
            data.p_units = std::stoi(token);

            std::getline(ss, token, ',');
            data.p_rentable_units = std::stoi(token);

            std::getline(ss, token, ',');
            data.p_expiring_leases = std::stoi(token);

            std::getline(ss, token, ',');
            data.p_early_move_out = std::stoi(token);
            std::getline(ss, token, ','); // Ignore % column

            std::getline(ss, token, ',');
            data.p_ntv = std::stoi(token);
            std::getline(ss, token, ','); // Ignore % column

            std::getline(ss, token, ',');
            data.p_renewals = std::stoi(token);
            std::getline(ss, token, ','); // Ignore % column

            std::getline(ss, token, ',');
            data.p_renewal_transfers = std::stoi(token);
            std::getline(ss, token, ','); // Ignore % column

            std::getline(ss, token, ',');
            data.p_transfers = std::stoi(token);
            std::getline(ss, token, ','); // Ignore % column

            std::getline(ss, token, ',');
            data.p_pending = std::stoi(token);
            std::getline(ss, token, ','); // Ignore % column

            std::getline(ss, token, ',');
            data.p_mtm = std::stoi(token);
            std::getline(ss, token, ','); // Ignore % column

            dataList.push_back(data);
        }

        row++;
    }

    for (auto &prop: properties)
        for (auto &entry:dataList)
            if (prop.dash.p_name == entry.p_name)
                prop.retention.push_back(entry);

    file.close();

    PRINT_NUM_ITEMS_PROCESSED(row);

    PRINT_FUNCTION_STOP();
}

void injectReceiptsByChargeCodeReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    // Create a document from the CSV file
    rapidcsv::Document doc(RECEIPTS_BY_CHARGE_CODE_FILENAME);
    std::vector<ReceiptsData> dataList;
    std::size_t numRows = doc.GetRowCount();

    // Iterate over each row in the CSV
    for (size_t i = 0; i < numRows; ++i)
    {
        ReceiptsData data;
        
        // Read each column and assign to the struct
        data.p_name                   = doc.GetCell<std::string>("Property", i);
        data.p_charge_code            = doc.GetCell<std::string>("Charge Code", i);
        data.p_gl_account             = doc.GetCell<std::string>("GL Account", i);
        data.p_total_charges          = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Total Charges", i));
        data.p_total_cash_collections = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Total Cash Collections", i));
        data.p_credits_x_concessions  = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Credits / Concessions", i));
        data.p_total_collections      = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Total Collections", i));

        std::string percentCollectedStr = doc.GetCell<std::string>("% Collected", i);
        if ( percentCollectedStr.size() == 0 )  
            data.p_percent_collected = 0.0f; // Cell was empty, set to 0
        else
            data.p_percent_collected = std::stof(percentCollectedStr); // Assuming this is already a float percentage

        // Add the struct to the vector
        dataList.push_back(data);
    }

    for (auto &prop: properties)
        for (auto &entry: dataList)
            if (prop.dash.p_name == entry.p_name)
                prop.receipts.push_back(entry);

    PRINT_NUM_ITEMS_PROCESSED(numRows);
    
    PRINT_FUNCTION_STOP();
}

void injectGrossPotentialRentReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    // Create a document from the CSV file
    rapidcsv::Document doc(GROSS_POTENTIAL_RENT_FILENAME);
    std::vector<GrossPotentialRentData> dataList;
    std::size_t numRows = doc.GetRowCount();

    // Iterate over each row in the CSV
    for (size_t i = 0; i < numRows; ++i)
    {
        GrossPotentialRentData data;

        // Read each column and assign to the struct
        data.p_name = doc.GetCell<std::string>("Property", i);
        data.p_unit_space_count = doc.GetCell<int>("Unit Space Count", i);
        data.p_gpr_market_rent = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("GPR - Market Rent", i));
        data.p_total_potential_rent = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Total Potential Rent", i));
        data.p_gain_loss_to_lease = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Gain/Loss to Lease", i));
        data.p_days_vacant = doc.GetCell<int>("Days Vacant", i);
        data.p_vacany = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Vacancy", i));
        data.p_vacany_adjustments = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Vacancy Adjustments", i));
        data.p_out_of_period_rent = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Out of Period Rent", i));
        data.p_scheduled_rent = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Scheduled Rent", i));
        data.p_rent_adjustments = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Rent Adjustments", i));
        data.p_rent_write_offs = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Rent Write-Offs", i));
        data.p_rental_income = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Rental Income", i));

        // Add the struct to the vector
        dataList.push_back(data);
    }

    for (auto &prop: properties)
        for (auto &entry: dataList)
            if (prop.dash.p_name == entry.p_name)
                prop.gpr.push_back(entry);

    PRINT_NUM_ITEMS_PROCESSED(numRows);

    PRINT_FUNCTION_STOP();
}

void injectWorkOrderPerformance(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    // Create a document from the CSV file
    rapidcsv::Document doc(WORK_ORDER_PERFORMANCE_FILENAME);
    std::vector<WorkOrderPerformanceData> dataList;
    std::size_t numRows = doc.GetRowCount();

    // Iterate over each row in the CSV
    for (size_t i = 0; i < numRows; ++i)
    {
        WorkOrderPerformanceData data;

        // Read each column and assign to the struct
        data.p_name = doc.GetCell<std::string>("Property", i);
        data.p_priority = doc.GetCell<std::string>("Priority", i);
        data.p_response_previous_work_orders = doc.GetCell<int>("Response - Previous Work Orders", i);
        data.p_response_new_work_orders = doc.GetCell<int>("Response - New Work Orders", i);
        data.p_response_on_time = doc.GetCell<int>("Response - On Time", i);
        data.p_response_percent_on_time = doc.GetCell<float>("Response - % On Time", i);
        data.p_response_late = doc.GetCell<int>("Response - Late", i);
        data.p_response_total = doc.GetCell<int>("Response - Total", i);
        data.p_response_not_started = doc.GetCell<int>("Response - Not Started", i);
        data.p_completion_previous_work_orders = doc.GetCell<int>("Completion - Previous Work Orders", i);
        data.p_completion_new_work_orders = doc.GetCell<int>("Completion - New Work Orders", i);
        data.p_completion_on_time = doc.GetCell<int>("Completion - On Time", i);
        data.p_completion_percent_on_time = doc.GetCell<float>("Completion - % On Time", i);
        data.p_completion_late = doc.GetCell<int>("Completion - Late", i);
        data.p_completion_total = doc.GetCell<int>("Completion - Total", i);
        data.p_completion_not_complete = doc.GetCell<int>("Completion - Not Complete", i);
        data.p_completion_exceptions = doc.GetCell<int>("Completion - Exceptions", i);

        // Add the struct to the vector
        dataList.push_back(data);
    }

    for (auto &prop: properties)
        for (auto &entry: dataList)
            if (prop.dash.p_name == entry.p_name)
                prop.workOrder.push_back(entry);

    PRINT_NUM_ITEMS_PROCESSED(numRows);

    PRINT_FUNCTION_STOP();
}

void injectResidentAgedReceivablesDetailReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    // Create a document from the CSV file
    rapidcsv::Document doc(RESIDENT_AGED_RECEIVABLES_DETAIL_FILENAME);
    std::vector<ResidentAgedReceivablesDetailData> dataList;
    std::size_t numRows = doc.GetRowCount();
    int currRow = 0;
    bool finished = false;

    do {
        try {
            for (; currRow < numRows; currRow++) {
                ResidentAgedReceivablesDetailData data;

                // Read each column and assign to the struct
                data.p_name = doc.GetCell<std::string>("Property", currRow);
                data.p_building_unit = doc.GetCell<std::string>("Bldg-Unit", currRow);

                // Split the Resident field into first and last names using splitName
                std::string resident = doc.GetCell<std::string>("Resident", currRow);
                splitName(resident, data.p_resident_fname, data.p_resident_lname);

                data.p_lease_status = doc.GetCell<std::string>("Lease Status", currRow);
                
                // Parse eviction start date, if available
                std::string evictionStartDate = doc.GetCell<std::string>("Eviction Start Date", currRow);
                if (!evictionStartDate.empty())
                {
                    data.p_eviction_start_date = stotm(evictionStartDate, "%m/%d/%Y");
                }

                // Use the helper function to handle commas and parentheses in financial data
                data.p_unallocated_charges_x_credits = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Unallocated Charges / Credits", currRow));
                data.p_0_to_30_days = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("0-30 Days", currRow));
                data.p_31_to_60_days = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("31-60 Days", currRow));
                data.p_61_to_90_days = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("61-90 Days", currRow));
                data.p_90_plus_days = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("90+ Days", currRow));
                data.p_pre_payments = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Pre-Payments", currRow));
                data.p_balance = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Balance", currRow));

                data.p_last_deliquency_note = doc.GetCell<std::string>("Last Delinquency Note", currRow);
                data.p_last_notice_sent = doc.GetCell<std::string>("Last Notice Sent", currRow);

                // Add the struct to the vector
                dataList.push_back(data);
            }
            finished = true;
        }
        catch (const std::exception& except) {
            //std::cerr << except.what() << std::endl;
            currRow++;
        }
    } 
    while ( !finished );

    for (auto &prop: properties)
        for (auto &entry: dataList)
            if (prop.dash.p_name == entry.p_name)
                prop.receivablesDetail.push_back(entry);

    PRINT_NUM_ITEMS_PROCESSED(numRows);

    PRINT_FUNCTION_STOP();
}

void injectResidentAgedReceivablesSummaryReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    // Create a document from the CSV file
    rapidcsv::Document doc(RESIDENT_AGED_RECEIVABLES_SUMMARY_FILENAME);
    std::vector<ResidentAgedReceivablesSummaryData> dataList;
    std::size_t numRows = doc.GetRowCount();

    // Iterate over each row in the CSV
    for (size_t i = 0; i < numRows; ++i)
    {
        ResidentAgedReceivablesSummaryData data;

        // Read each column and assign to the struct
        data.p_name = doc.GetCell<std::string>("Property", i);
        data.p_lease_status = doc.GetCell<std::string>("Lease Status", i);
        data.p_unallocated_charges_x_credits = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Unallocated Charges / Credits", i));
        data.p_0_to_30_days = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("0-30 Days", i));
        data.p_31_to_60_days = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("31-60 Days", i));
        data.p_61_to_90_days = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("61-90 Days", i));
        data.p_90_plus_days = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("90+ Days", i));
        data.p_pre_payments = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Pre-Payments", i));
        data.p_balance = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Balance", i));

        // Add the struct to the vector
        dataList.push_back(data);
    }

    for (auto &prop: properties)
        for (auto &entry: dataList)
            if (prop.dash.p_name == entry.p_name)
                prop.receivablesSummary.push_back(entry);

    PRINT_NUM_ITEMS_PROCESSED(numRows);

    PRINT_FUNCTION_STOP();
}

#endif /* injection_routines_hpp */
