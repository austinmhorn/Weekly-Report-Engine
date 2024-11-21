#ifndef injection_routines_hpp
#define injection_routines_hpp

#include <sstream>
#include <vector>

#include "rapidcsv.h"

#include "Property.hpp"
#include "dictionaries.hpp"
#include "helper_functions.hpp"


/// @brief  Inject Dashboard Report data into vector
/// @param properties 
void injectDashboardReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    static const std::string DASHBOARD_REPORT_FILENAME = "data/" + sExportDate + "/clean/Birchstone Dashboard.csv";

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

    static const std::string AVAILABILITY_REPORT_FILENAME = "data/" + sExportDate + "/clean/Availability.csv";

    rapidcsv::Document doc(AVAILABILITY_REPORT_FILENAME);
    int numRows = doc.GetRowCount();
    int currRow = 0;
    bool finished = false;
    std::vector<AvailabilityData> dataList;

    do {
        try {
            for (; currRow < numRows ; currRow++) {
                AvailabilityData data;
                
                data.p_name           = doc.GetCell<std::string>("Property", currRow);
                data.p_building_unit  = doc.GetCell<std::string>("Bldg-Unit", currRow);
                data.p_status         = doc.GetCell<std::string>("Unit Status", currRow);
                data.p_floorplan      = doc.GetCell<std::string>("Floor Plan", currRow);
                data.p_unit_type      = doc.GetCell<std::string>("Unit Type", currRow);
                data.p_exclusion_name = trimDoubleQuotes(doc.GetCell<std::string>("Exclusion Name", currRow));
                data.p_days_vacant    = doc.GetCell<int>("Days Vacant", currRow);
                
                data.p_move_out = stotm(
                    doc.GetCell<std::string>("Move-Out", currRow), 
                    "%m/%d/%Y"
                    );
                data.p_available_on = stotm(
                    doc.GetCell<std::string>("Available On", currRow), 
                    "%m/%d/%Y"
                    );
                data.p_scheduled_move_in = stotm(
                    doc.GetCell<std::string>("Scheduled Move-In", currRow), 
                    "%m/%d/%Y"
                    );

                //data.p_unit_notes = doc.GetCell<std::string>("Unit Notes", currRow);
                //data.p_unit_activity_notes = doc.GetCell<std::string>("Unit Activity Notes", currRow);
                //data.p_hazard_note = doc.GetCell<std::string>("Hazard Note", currRow);
                data.p_budgeted_rent = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Budgeted Rent", currRow));
                data.p_prior_lease_rent = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Prior Lease Rent", currRow));
                data.p_best_price = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Best Price", currRow));
                data.p_market_rent = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Market Rent", currRow));

                std::string futureLeaseRentStr = doc.GetCell<std::string>("Future Lease Rent", currRow);
                if (futureLeaseRentStr.size()) {
                    data.p_future_lease_rent = parseFloatWithCommaAndParentheses(futureLeaseRentStr);
                }
                else {
                    data.p_future_lease_rent_str = "";
                } 

                data.p_est_vacancy_cost = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Est. Vacancy Cost", currRow));

                data.p_lease_completed = stotm(
                    doc.GetCell<std::string>("Lease Completed", currRow), 
                    "%m/%d/%Y"
                    );

                //std::cout << currRow-2 << " " << data.p_name << " " << data.p_building_unit << std::endl;

                dataList.push_back(data);
            }
            finished = true;
        }
        catch (const std::exception& except) {
            std::cerr << "Error at row " << currRow << ": " << except.what() << std::endl;
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

    static const std::string MOVE_OUT_REASONS_REPORT_FILENAME = "data/" + sExportDate + "/clean/Move Out Reasons.csv";

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

    static const std::string BOX_SCORE_LEAD_CONVERSIONS_FILENAME = "data/" + sExportDate + "/clean/Box Score - Lead Conversions.csv";

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

    static const std::string BOX_SCORE_MAKE_READY_STATUS_FILENAME = "data/" + sExportDate + "/clean/Box Score - Make Ready Status.csv";

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

    static const std::string BOX_SCORE_PROPERTY_PULSE_FILENAME = "data/" + sExportDate + "/clean/Box Score - Property Pulse.csv";

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

    static const std::string RESIDENT_RETENTION_FILENAME = "data/" + sExportDate + "/clean/Resident Retention.csv";

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

void injectReceiptsByChargeCode_Receipts_Report(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    static const std::string RECEIPTS_BY_CHARGE_CODE_RECEIPTS_FILENAME = "data/" + sExportDate + "/clean/Receipts By Charge Code (Rent) - Receipts.csv";

    // Create a document from the CSV file
    rapidcsv::Document doc(RECEIPTS_BY_CHARGE_CODE_RECEIPTS_FILENAME);
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

//void injectReceiptsByChargeCode_Pre_Payments_Report(std::vector<Property>& properties)
//{
//    PRINT_FUNCTION_START();
//
//    static const std::string RECEIPTS_BY_CHARGE_CODE_PREPAYMENTS_FILENAME = "data/" + sExportDate + "/clean/Receipts By Charge Code (Rent) - Pre-Payments.csv";
//
//    // Create a document from the CSV file
//    rapidcsv::Document doc(RECEIPTS_BY_CHARGE_CODE_PREPAYMENTS_FILENAME);
//    std::vector<ReceiptsData> dataList;
//    std::size_t numRows = doc.GetRowCount();
//
//    for (std::size_t i = 0; i < numRows; i++)
//    {
//
//    }
//
//    PRINT_NUM_ITEMS_PROCESSED(numRows);
//
//    PRINT_FUNCTION_STOP();
//}

void injectGrossPotentialRentReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    static const std::string GROSS_POTENTIAL_RENT_FILENAME = "data/" + sExportDate + "/clean/Gross Potential Rent.csv";

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

        std::string unitSpaceCountStr = doc.GetCell<std::string>("Unit Space Count", i);
        
        data.p_unit_space_count = (unitSpaceCountStr.size()) 
            ? std::stoi(unitSpaceCountStr) 
            : 0;

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

    static const std::string WORK_ORDER_PERFORMANCE_FILENAME = "data/" + sExportDate + "/clean/Work Order Performance.csv";

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

    static const std::string RESIDENT_AGED_RECEIVABLES_DETAIL_FILENAME = "data/" + sExportDate + "/clean/Resident Aged Receivables (Detail).csv";

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
                data.p_delinquent_total = parseFloatWithCommaAndParentheses(doc.GetCell<std::string>("Delinquent Total", currRow));
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

    static const std::string RESIDENT_AGED_RECEIVABLES_SUMMARY_FILENAME = "data/" + sExportDate + "/clean/Resident Aged Receivables (Summary).csv";

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

void injectIncomeStatementReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    static const std::string INCOME_STATEMENT_FILENAME = "data/" + sExportDate + "/clean/Income Statement.csv";

    rapidcsv::Document doc(INCOME_STATEMENT_FILENAME);
    std::ifstream file(INCOME_STATEMENT_FILENAME, std::ios::in);
    std::string line;
    std::vector<IncomeStatementData> dataList;
    int row = 0;
    std::vector<std::string> columnNames;

    while ( !file.eof() )
    {
        std::getline(file, line);
        std::stringstream ss(line);

        if ( !line.size() ) // Found empty row
            break;

        if ( row == 0 )
        {
            std::string headerTitle;
            std::getline(ss, headerTitle, ',');
            std::getline(ss, headerTitle, ',');
            std::getline(ss, headerTitle, ',');

            std::getline(ss, headerTitle, ',');
            columnNames.push_back(trimDoubleQuotes(headerTitle));
            std::getline(ss, headerTitle, ',');
            columnNames.push_back(trimDoubleQuotes(headerTitle));
            std::getline(ss, headerTitle, ',');
            columnNames.push_back(trimDoubleQuotes(headerTitle));
            std::getline(ss, headerTitle, ',');
            columnNames.push_back(trimDoubleQuotes(headerTitle));
            std::getline(ss, headerTitle, ',');
            columnNames.push_back(trimDoubleQuotes(headerTitle));
            std::getline(ss, headerTitle, ',');
            columnNames.push_back(trimDoubleQuotes(headerTitle));
            std::getline(ss, headerTitle, ',');
            columnNames.push_back(trimDoubleQuotes(headerTitle));
            std::getline(ss, headerTitle, ',');
            columnNames.push_back(trimDoubleQuotes(headerTitle));
            std::getline(ss, headerTitle, ',');
            columnNames.push_back(trimDoubleQuotes(headerTitle));
            std::getline(ss, headerTitle, ',');
            columnNames.push_back(trimDoubleQuotes(headerTitle));
            std::getline(ss, headerTitle, ',');
            columnNames.push_back(trimDoubleQuotes(headerTitle));
            std::getline(ss, headerTitle, ',');
            columnNames.push_back(trimDoubleQuotes(headerTitle));
        }
        else
        {
            IncomeStatementData data;
            std::string token;

            std::getline(ss, token, ',');
            data.p_name = trimDoubleQuotes(token);

            std::getline(ss, token, ',');
            data.p_unit_count = std::stoi(trimDoubleQuotes(token));

            std::getline(ss, token, ',');
            data.p_account_name = trimDoubleQuotes(token);

            if ( data.p_account_name == "Rent - Resident" ) {
                token = doc.GetCell<std::string>(columnNames.at(0), row-1);
                token = removeCommas(token);
                data.p_balance_11_months_ago.first = columnNames.at(0);
                data.p_balance_11_months_ago.second = std::stof(token);

                token = doc.GetCell<std::string>(columnNames.at(1), row-1);
                token = removeCommas(token);
                data.p_balance_10_months_ago.first = columnNames.at(1);
                data.p_balance_10_months_ago.second = std::stof(token);

                token = doc.GetCell<std::string>(columnNames.at(2), row-1);
                token = removeCommas(token);
                data.p_balance_9_months_ago.first = columnNames.at(2);
                data.p_balance_9_months_ago.second = std::stof(token);

                token = doc.GetCell<std::string>(columnNames.at(3), row-1);
                token = removeCommas(token);
                data.p_balance_8_months_ago.first = columnNames.at(3);
                data.p_balance_8_months_ago.second = std::stof(token);

                token = doc.GetCell<std::string>(columnNames.at(4), row-1);
                token = removeCommas(token);
                data.p_balance_7_months_ago.first = columnNames.at(4);
                data.p_balance_7_months_ago.second = std::stof(token);

                token = doc.GetCell<std::string>(columnNames.at(5), row-1);
                token = removeCommas(token);
                data.p_balance_6_months_ago.first = columnNames.at(5);
                data.p_balance_6_months_ago.second = std::stof(token);

                token = doc.GetCell<std::string>(columnNames.at(6), row-1);
                token = removeCommas(token);
                data.p_balance_5_months_ago.first = columnNames.at(6);
                data.p_balance_5_months_ago.second = std::stof(token);

                token = doc.GetCell<std::string>(columnNames.at(7), row-1);
                token = removeCommas(token);
                data.p_balance_4_months_ago.first = columnNames.at(7);
                data.p_balance_4_months_ago.second = std::stof(token);

                token = doc.GetCell<std::string>(columnNames.at(8), row-1);
                token = removeCommas(token);
                data.p_balance_3_months_ago.first = columnNames.at(8);
                data.p_balance_3_months_ago.second = std::stof(token);

                token = doc.GetCell<std::string>(columnNames.at(9), row-1);
                token = removeCommas(token);
                data.p_balance_2_months_ago.first = columnNames.at(9);
                data.p_balance_2_months_ago.second = std::stof(token);

                token = doc.GetCell<std::string>(columnNames.at(10), row-1);
                token = removeCommas(token);
                data.p_balance_1_months_ago.first = columnNames.at(10);
                data.p_balance_1_months_ago.second = std::stof(token);

                token = doc.GetCell<std::string>(columnNames.at(11), row-1);
                token = removeCommas(token);
                data.p_balance_0_months_ago.first = columnNames.at(11);
                data.p_balance_0_months_ago.second = std::stof(token);

                token = doc.GetCell<std::string>("Total", row-1);
                token = removeCommas(token);
                data.p_balance_total = std::stof(token);

                dataList.push_back(data);
            }
        }

        row++;
    }
    
    for (auto& entry: dataList)
        for (auto &prop: properties)
            if (entry.p_name == prop.dash.p_name)
                prop.income = entry;

    file.close();
    
    PRINT_NUM_ITEMS_PROCESSED(row-1);

    PRINT_FUNCTION_STOP();
}

void injectDailyAndWeeklyOperationsReport(std::vector<Property>& properties)
{
    PRINT_FUNCTION_START();

    static const std::string DAILY_AND_WEEKLY_OPERATIONS_FILENAME = "data/" + sExportDate + "/clean/Daily and Weekly Operations.csv";

    std::vector<DailyAndWeeklyOperationsData> dataList;
    std::size_t numRows;

    try 
    {
        // Load the CSV document; row 0 is the header and no label column
        rapidcsv::Document doc(DAILY_AND_WEEKLY_OPERATIONS_FILENAME, rapidcsv::LabelParams(0, -1));

        // Get the total row count (excluding the header row)
        numRows = doc.GetRowCount();

        // Iterate over each row and populate the struct fields
        for (size_t row = 0; row < numRows; ++row) 
        {
            DailyAndWeeklyOperationsData data;

            // Parse and assign each field from the corresponding CSV column
            data.p_name = doc.GetCell<std::string>("Property", row);
            data.p_lead_source = doc.GetCell<std::string>("Lead Source", row);
            data.p_new_leads = doc.GetCell<int>("New Leads", row);
            data.p_first_visit_tour = doc.GetCell<int>("First Visit/Tour", row);

            // Add the populated struct to our data list
            dataList.push_back(data);
        }
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error reading CSV: " << e.what() << std::endl;
    }

    for (auto& entry: dataList)
        for (auto &prop: properties)
            if (entry.p_name == prop.dash.p_name)
                prop.ops.push_back(entry);

    PRINT_NUM_ITEMS_PROCESSED(numRows);

    PRINT_FUNCTION_STOP();
}

#endif /* injection_routines_hpp */
