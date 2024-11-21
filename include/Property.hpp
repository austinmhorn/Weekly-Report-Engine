#ifndef __Property_hpp
#define __Property_hpp


#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>



struct AvailabilityData 
{
    bool p_is_excluded;
    std::string p_status;
    std::string p_name;
    std::string p_building_unit;
    std::string p_floorplan;
    std::string p_unit_type;
    std::string p_exclusion_name;
    float p_best_price;
    int p_sqft;
    int p_days_vacant;
    std::tm p_move_out;
    std::tm p_available_on;
    float p_budgeted_rent;
    float p_market_rent;
    float p_prior_lease_rent;
    std::tm p_scheduled_move_in;
    std::string p_unit_notes;
    std::string p_unit_activity_notes;
    std::string p_hazard_note;
    float p_est_vacancy_cost;
    std::tm p_lease_completed;
    float p_future_lease_rent = 0.0f;
    std::string p_future_lease_rent_str = "~";
};




struct DashboardData
{
    std::string p_name;
    float p_occupied;
    float p_14_day_trend;
    float p_28_day_trend;
    float p_56_day_trend;
    float p_14_day_trend_with_apps;
    float p_28_day_trend_with_apps;
    float p_56_day_trend_with_apps;

    int p_30_day_traffic_count;
    float p_30_day_closing_ratio;
    float p_occupied_rent;
    int p_previous_30_day_move_ins;
    int p_total_num_pending_move_ins;

    int p_open_work_orders;
    float p_percent_work_orders_completed_on_time;

    float p_total_billed;
    float p_current_month_collections;
    float p_percent_collected;
    float p_delinquency_1;
    float p_delinquency_2;
    float p_delinquency_3;
    int p_evictions_current;
    int p_evictions_total;

    int p_notice_unrented;
    int p_vacant_unrented_not_ready;
    int p_vacant_unrented_ready;

    float p_ap_total_outstanding;
    int p_notice_rented;
    int p_vacant_rented_ready;

    float p_current_month_renewal_percent;
    float p_prior_month_renewal_percent;
    int p_expirations_next_month;
    int p_renewals_next_month;
    int p_expirations_in_2_months;
    int p_renewals_in_2_months;
    int p_current_number_notice_to_vacate;

    int p_total_Ps;
    int p_total_Us;
    int p_total_Rs;
    int p_total_excluded_units;
};



struct MoveOutData
{
    std::string p_name;
    std::string p_building_and_unit_number;
    std::string p_unit_type;
    std::string p_resident_fname;
    std::string p_resident_lname;
    std::tm p_notice_given;
    std::tm p_lease_end_date;
    std::tm p_move_out_date;
    std::string p_reason;
    std::string p_early_termination;
    float p_market_rent;
    float p_scheduled_rent;
    float p_deposit_held;
    float p_balance;
};



struct BoxScoreLeadConversionsData
{
    std::string p_property_name;
    int p_apps_partially_completed;
    int p_apps_completed;
    int p_apps_completed_cancelled;
    int p_apps_denied;
    int p_apps_approved;
    int p_apps_approved_cancelled;
    int p_lease_completed;
    int p_lease_completed_cancelled;
    int p_lease_approved;
};


struct BoxScoreMakeReadyStatusData
{
    std::string p_property_name;
    std::string p_status;
    int p_vacant_rented;
    int p_vacant_unrented;
    int p_total_vacant;
    float p_percent;
};


struct BoxScorePropertyPulseData
{
    std::string p_property_name;
    int p_units;
    int p_move_ins;
    int p_mtm;
    int p_mtm_conversions;
    int p_renewal_offers_completed;
    int p_transfers;
    int p_notices;
    int p_move_outs;
    int p_renewal_transfers;
    int p_skips;
    int p_evictions;
    int p_leased;
};


struct ResidentRetentionData
{
    std::string p_name;
    std::string p_month;
    int p_units;
    int p_rentable_units;
    int p_expiring_leases;
    int p_early_move_out;
    int p_ntv;
    int p_renewals;
    int p_renewal_transfers;
    int p_transfers;
    int p_pending;
    int p_mtm;
};


struct ReceiptsData
{
    std::string p_name;
    std::string p_charge_code;
    std::string p_gl_account;
    float p_total_charges;
    float p_total_cash_collections;
    float p_credits_x_concessions;
    float p_total_collections;
    float p_percent_collected;
};


struct GrossPotentialRentData
{
    std::string p_name;
    int p_unit_space_count;
    float p_gpr_market_rent;
    float p_total_potential_rent;
    float p_gain_loss_to_lease;
    int p_days_vacant;
    float p_vacany;
    float p_vacany_adjustments;
    float p_out_of_period_rent;
    float p_scheduled_rent;
    float p_rent_adjustments;
    float p_rent_write_offs;
    float p_rental_income;
};


struct WorkOrderPerformanceData
{
    std::string p_name;
    std::string p_priority;
    int p_response_previous_work_orders;
    int p_response_new_work_orders;
    int p_response_on_time;
    float p_response_percent_on_time;
    int p_response_late;
    int p_response_total;
    int p_response_not_started;
    int p_completion_previous_work_orders;
    int p_completion_new_work_orders;
    int p_completion_on_time;
    float p_completion_percent_on_time;
    int p_completion_late;
    int p_completion_total;
    int p_completion_not_complete;
    int p_completion_exceptions;
};


struct ResidentAgedReceivablesDetailData
{
    std::string p_name;
    std::string p_building_unit;
    std::string p_resident_fname;
    std::string p_resident_lname;
    std::string p_lease_status;
    std::tm p_eviction_start_date;
    float p_delinquent_total;
    float p_0_to_30_days;
    float p_31_to_60_days;
    float p_61_to_90_days;
    float p_90_plus_days;
    float p_pre_payments;
    float p_balance;
    std::string p_last_deliquency_note;
    std::string p_last_notice_sent;
};


struct ResidentAgedReceivablesSummaryData
{
    std::string p_name;
    std::string p_lease_status;
    float p_unallocated_charges_x_credits;
    float p_0_to_30_days;
    float p_31_to_60_days;
    float p_61_to_90_days;
    float p_90_plus_days;
    float p_pre_payments;
    float p_balance;
};


typedef std::pair<std::string, float> balance_data;
struct IncomeStatementData
{
    std::string p_name;
    int p_unit_count;
    std::string p_account_name;
    balance_data p_balance_11_months_ago;
    balance_data p_balance_10_months_ago;
    balance_data p_balance_9_months_ago;
    balance_data p_balance_8_months_ago;
    balance_data p_balance_7_months_ago;
    balance_data p_balance_6_months_ago;
    balance_data p_balance_5_months_ago;
    balance_data p_balance_4_months_ago;
    balance_data p_balance_3_months_ago;
    balance_data p_balance_2_months_ago;
    balance_data p_balance_1_months_ago;
    balance_data p_balance_0_months_ago;
    float p_balance_total;
};


struct DailyAndWeeklyOperationsData
{
    std::string p_name;
    std::string p_lead_source;
    int p_new_leads;
    int p_first_visit_tour;
};


struct Property
{
    Property() = default;
    ~Property() = default;

    unsigned p_dict_key = 0;
    std::string p_official_marketing_name;

    DashboardData                                   dash;
    std::vector<AvailabilityData>                   availability;
    std::vector<MoveOutData>                        move;
    BoxScoreLeadConversionsData                     leadConversions;
    std::vector<BoxScoreMakeReadyStatusData>        makeReady;
    BoxScorePropertyPulseData                       pulse;
    std::vector<ResidentRetentionData>              retention;
    std::vector<ReceiptsData>                       receipts;
    std::vector<GrossPotentialRentData>             gpr;
    std::vector<WorkOrderPerformanceData>           workOrder;
    std::vector<ResidentAgedReceivablesDetailData>  receivablesDetail;
    std::vector<ResidentAgedReceivablesSummaryData> receivablesSummary;
    IncomeStatementData                             income;
    std::vector<DailyAndWeeklyOperationsData>       ops;
};

#endif /* __Property_hpp */