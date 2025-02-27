#ifndef dictionaries__hpp
#define dictionaries__hpp

#include <map>


enum class OverviewReportDictionary : unsigned
{
    None                                  = 0,
    Anthem_Clearwater                     = 1,
    Anthem_Mesquite                       = 2,
    Anthem_Town_East                      = 3,
    Anthem_Valley_Ranch                   = 4,
    Avery                                 = 5,
    Company                               = 6,
    Devon_on_Northgate                    = 7,
    Elliot_Apartments_on_Abernathy        = 8,
    Elliot_Baymeadows_Apartments          = 9,
    Elliot_Cocoplum                       = 10,
    Elliot_Gwinnett_Place                 = 11,
    Elliot_Norcross                       = 12,
    Elliot_Pioneer                        = 13,
    Elliot_Roswell_Apartments             = 14,
    Elliot_Windsprint                     = 15,
    Estates_at_Las_Colinas                = 16,
    Exchange_7272                         = 17,
    Halston_Citrus_Ridge                  = 18,
    Halston_Four_Corners                  = 19,
    Halston_Harris_Bridge                 = 20,
    Halston_Lakeside                      = 21,
    Halston_McDonough                     = 22,
    Halston_Paces_Crossing                = 23,
    Halston_Park_Central                  = 24,
    Halston_Riverside                     = 25,
    Halston_Shiloh_Valley_Apartments      = 26,
    Halston_South_Point_Apartment_Homes   = 27,
    Halston_Waterleigh                    = 28,
    Halston_World_Gateway_Apartment_Homes = 29,
    Halston_On_Frankford                  = 30,
    Lexington_Apartment_Homes             = 31,
    Liv_at_Winter_Park_Apartment_Homes    = 32,
    Liv_on_the_Green_Apartment_Homes      = 33,
    MacArthur_Place_at_183                = 34,
    Northern_Cross_Apartments             = 35,
    Preserve_at_Preston                   = 36,
    Southside_Villas                      = 37,
    The_Apartments_at_Midtown_501         = 38,
    The_Oaks_at_Valley_Ranch              = 39,
    Vista_121_Apartment_Homes             = 40,
    Westmoor_Apartment_Homes              = 41,
    
    ///< Keep Last -- determines number of enumerations
    Count = 42
};

static std::map<OverviewReportDictionary, const char *> overviewReportNameMap = 
{
    {OverviewReportDictionary::None, "N/A"},
    {OverviewReportDictionary::Anthem_Clearwater, "Anthem Clearwater"},
    {OverviewReportDictionary::Anthem_Mesquite, "Anthem Mesquite"},
    {OverviewReportDictionary::Anthem_Town_East, "Anthem Town East"},
    {OverviewReportDictionary::Anthem_Valley_Ranch, "Anthem Valley Ranch"},
    {OverviewReportDictionary::Avery, "Avery"},
    {OverviewReportDictionary::Company, "Company"},
    {OverviewReportDictionary::Devon_on_Northgate, "Devon on Northgate"},
    {OverviewReportDictionary::Elliot_Apartments_on_Abernathy, "Elliot Apartments on Abernathy"},
    {OverviewReportDictionary::Elliot_Baymeadows_Apartments, "Elliot Baymeadows"},
    {OverviewReportDictionary::Elliot_Cocoplum, "Elliot Cocoplum"},
    {OverviewReportDictionary::Elliot_Gwinnett_Place, "Elliot Gwinnett Place"},
    {OverviewReportDictionary::Elliot_Norcross, "Elliot Norcross"},
    {OverviewReportDictionary::Elliot_Pioneer, "Elliot Pioneer"},
    {OverviewReportDictionary::Elliot_Roswell_Apartments, "Elliot Roswell"},
    {OverviewReportDictionary::Elliot_Windsprint, "Elliot Windsprint"},
    {OverviewReportDictionary::Estates_at_Las_Colinas, "Estates at Las Colinas"},
    {OverviewReportDictionary::Exchange_7272, "Exchange 7272"},
    {OverviewReportDictionary::Halston_Citrus_Ridge, "Halston Citrus Ridge"},
    {OverviewReportDictionary::Halston_Four_Corners, "Halston Four Corners"},
    {OverviewReportDictionary::Halston_Harris_Bridge, "Halston Harris Bridge"},
    {OverviewReportDictionary::Halston_Lakeside, "Halston Lakeside"},
    {OverviewReportDictionary::Halston_McDonough, "Halston McDonough"},
    {OverviewReportDictionary::Halston_On_Frankford, "Halston on Frankford"},
    {OverviewReportDictionary::Halston_Paces_Crossing, "Halston Paces Crossing"},
    {OverviewReportDictionary::Halston_Park_Central, "Halston Park Central"},
    {OverviewReportDictionary::Halston_Riverside, "Halston Riverside"},
    {OverviewReportDictionary::Halston_Shiloh_Valley_Apartments, "Halston Shiloh Valley"},
    {OverviewReportDictionary::Halston_South_Point_Apartment_Homes, "Halston South Point"},
    {OverviewReportDictionary::Halston_Waterleigh, "Halston Waterleigh"},
    {OverviewReportDictionary::Halston_World_Gateway_Apartment_Homes, "Halston World Gateway"},
    {OverviewReportDictionary::Lexington_Apartment_Homes, "Lexington"},
    {OverviewReportDictionary::Liv_at_Winter_Park_Apartment_Homes, "Liv at Winter Park"},
    {OverviewReportDictionary::Liv_on_the_Green_Apartment_Homes, "Liv on the Green"},
    {OverviewReportDictionary::MacArthur_Place_at_183, "MacArthur Place at 183"},
    {OverviewReportDictionary::Northern_Cross_Apartments, "Northern Cross"},
    {OverviewReportDictionary::Preserve_at_Preston, "Preserve at Preston"},
    {OverviewReportDictionary::Southside_Villas, "Southside Villas"},
    {OverviewReportDictionary::The_Apartments_at_Midtown_501, "The Apartments at Midtown 501"},
    {OverviewReportDictionary::The_Oaks_at_Valley_Ranch, "The Oaks at Valley Ranch"},
    {OverviewReportDictionary::Vista_121_Apartment_Homes, "Vista 121"},
    {OverviewReportDictionary::Westmoor_Apartment_Homes, "Westmoor"}
};



enum class ConversionReportDictionary : unsigned
{
    None                                  = 0,
    Anthem_Clearwater                     = 1,
    Anthem_Mesquite                       = 2,
    Anthem_Town_East                      = 3,
    Anthem_Valley_Ranch                   = 4,
    Avery                                 = 5,
    Company                               = 6,
    Devon_on_Northgate                    = 7,
    Elliot_Apartments_on_Abernathy        = 8,
    Elliot_Baymeadows_Apartments          = 9,
    Elliot_Cocoplum                       = 10,
    Elliot_Gwinnett_Place                 = 11,
    Elliot_Norcross                       = 12,
    Elliot_Pioneer                        = 13,
    Elliot_Roswell_Apartments             = 14,
    Elliot_Windsprint                     = 15,
    Estates_at_Las_Colinas                = 16,
    Exchange_7272                         = 17,
    Halston_Citrus_Ridge                  = 18,
    Halston_Four_Corners                  = 19,
    Halston_Harris_Bridge                 = 20,
    Halston_Lakeside                      = 21,
    Halston_McDonough                     = 22,
    Halston_Paces_Crossing                = 23,
    Halston_Park_Central                  = 24,
    Halston_Riverside                     = 25,
    Halston_Shiloh_Valley_Apartments      = 26,
    Halston_South_Point_Apartment_Homes   = 27,
    Halston_Waterleigh                    = 28,
    Halston_World_Gateway_Apartment_Homes = 29,
    Halston_On_Frankford                  = 30,
    Lexington_Apartment_Homes             = 31,
    Liv_at_Winter_Park_Apartment_Homes    = 32,
    Liv_on_the_Green_Apartment_Homes      = 33,
    MacArthur_Place_at_183                = 34,
    Northern_Cross_Apartments             = 35,
    Preserve_at_Preston                   = 36,
    Southside_Villas                      = 37,
    The_Apartments_at_Midtown_501         = 38,
    The_Oaks_at_Valley_Ranch              = 39,
    Vista_121_Apartment_Homes             = 40,
    Westmoor_Apartment_Homes              = 41,
    
    ///< Keep Last -- determines number of enumerations
    Count = 42
};

static std::map<ConversionReportDictionary, const char *> conversionReportPropertyNameMap = 
{
    {ConversionReportDictionary::None, "N/A"},
    {ConversionReportDictionary::Anthem_Clearwater, "Anthem Clearwater"},
    {ConversionReportDictionary::Anthem_Mesquite, "Anthem Mesquite"},
    {ConversionReportDictionary::Anthem_Town_East, "Anthem Town East"},
    {ConversionReportDictionary::Anthem_Valley_Ranch, "Anthem Valley Ranch"},
    {ConversionReportDictionary::Avery, "Avery"},
    {ConversionReportDictionary::Company, "Company"},
    {ConversionReportDictionary::Devon_on_Northgate, "Devon on Northgate"},
    {ConversionReportDictionary::Elliot_Apartments_on_Abernathy, "Elliot Apartments on Abernathy"},
    {ConversionReportDictionary::Elliot_Baymeadows_Apartments, "Elliot Baymeadows Apartments"},
    {ConversionReportDictionary::Elliot_Cocoplum, "Elliot Cocoplum"},
    {ConversionReportDictionary::Elliot_Gwinnett_Place, "Elliot Gwinnett Place"},
    {ConversionReportDictionary::Elliot_Norcross, "Elliot Norcross"},
    {ConversionReportDictionary::Elliot_Pioneer, "Elliot Pioneer"},
    {ConversionReportDictionary::Elliot_Roswell_Apartments, "Elliot Roswell Apartments"},
    {ConversionReportDictionary::Elliot_Windsprint, "Elliot Windsprint"},
    {ConversionReportDictionary::Estates_at_Las_Colinas, "Estates at Las Colinas"},
    {ConversionReportDictionary::Exchange_7272, "Exchange 7272"},
    {ConversionReportDictionary::Halston_Citrus_Ridge, "Halston Citrus Ridge"},
    {ConversionReportDictionary::Halston_Four_Corners, "Halston Four Corners"},
    {ConversionReportDictionary::Halston_Harris_Bridge, "Halston Harris Bridge"},
    {ConversionReportDictionary::Halston_Lakeside, "Halston Lakeside"},
    {ConversionReportDictionary::Halston_McDonough, "Halston McDonough"},
    {ConversionReportDictionary::Halston_On_Frankford, "Halston On Frankford"},
    {ConversionReportDictionary::Halston_Paces_Crossing, "Halston Paces Crossing"},
    {ConversionReportDictionary::Halston_Park_Central, "Halston Park Central"},
    {ConversionReportDictionary::Halston_Riverside, "Halston Riverside"},
    {ConversionReportDictionary::Halston_Shiloh_Valley_Apartments, "Halston Shiloh Valley Apartments"},
    {ConversionReportDictionary::Halston_South_Point_Apartment_Homes, "Halston South Point Apartment Homes"},
    {ConversionReportDictionary::Halston_Waterleigh, "Halston Waterleigh "},
    {ConversionReportDictionary::Halston_World_Gateway_Apartment_Homes, "Halston World Gateway Apartment Homes"},
    {ConversionReportDictionary::Lexington_Apartment_Homes, "Lexington Apartment Homes"},
    {ConversionReportDictionary::Liv_at_Winter_Park_Apartment_Homes, "Liv at Winter Park Apartment Homes"},
    {ConversionReportDictionary::Liv_on_the_Green_Apartment_Homes, "Liv on the Green Apartment Homes"},
    {ConversionReportDictionary::MacArthur_Place_at_183, "MacArthur Place at 183"},
    {ConversionReportDictionary::Northern_Cross_Apartments, "Northern Cross Apartments"},
    {ConversionReportDictionary::Preserve_at_Preston, "Preserve at Preston"},
    {ConversionReportDictionary::Southside_Villas, "Southside Villas"},
    {ConversionReportDictionary::The_Apartments_at_Midtown_501, "The Apartments at Midtown 501"},
    {ConversionReportDictionary::The_Oaks_at_Valley_Ranch, "The Oaks at Valley Ranch"},
    {ConversionReportDictionary::Vista_121_Apartment_Homes, "Vista 121 Apartment Homes"},
    {ConversionReportDictionary::Westmoor_Apartment_Homes, "Westmoor Apartment Homes"}
};

enum class DashboardReportDictionary : unsigned 
{
    None                                  = 0,
    Anthem_Clearwater                     = 1,
    Anthem_Mesquite                       = 2,
    Anthem_Town_East                      = 3,
    Anthem_Valley_Ranch                   = 4,
    Avery                                 = 5,
    Company                               = 6,
    Devon_on_Northgate                    = 7,
    Elliot_Apartments_on_Abernathy        = 8,
    Elliot_Baymeadows_Apartments          = 9,
    Elliot_Cocoplum                       = 10,
    Elliot_Gwinnett_Place                 = 11,
    Elliot_Norcross                       = 12,
    Elliot_Pioneer                        = 13,
    Elliot_Roswell_Apartments             = 14,
    Elliot_Windsprint                     = 15,
    Estates_at_Las_Colinas                = 16,
    Exchange_7272                         = 17,
    Halston_Citrus_Ridge                  = 18,
    Halston_Four_Corners                  = 19,
    Halston_Harris_Bridge                 = 20,
    Halston_Lakeside                      = 21,
    Halston_McDonough                     = 22,
    Halston_On_Frankford                  = 23,
    Halston_Paces_Crossing                = 24,
    Halston_Park_Central                  = 25,
    Halston_Riverside                     = 26,
    Halston_Shiloh_Valley_Apartments      = 27,
    Halston_South_Point_Apartment_Homes   = 28,
    Halston_Waterleigh                    = 29,
    Halston_World_Gateway_Apartment_Homes = 30,
    Lexington_Apartment_Homes             = 31,
    Liv_at_Winter_Park_Apartment_Homes    = 32,
    Liv_on_the_Green_Apartment_Homes      = 33,
    MacArthur_Place_at_183                = 34,
    Northern_Cross_Apartments             = 35,
    Preserve_at_Preston                   = 36,
    Southside_Villas                      = 37,
    The_Apartments_at_Midtown_501         = 38,
    The_Oaks_at_Valley_Ranch              = 39,
    Vista_121_Apartment_Homes             = 40,
    Westmoor_Apartment_Homes              = 41,

    ///< Keep Last -- determines number of enumerations
    Count = 42
};

static std::map<DashboardReportDictionary, const char *> dashboardReportPropertyNameMap = 
{
    {DashboardReportDictionary::None, "N/A"},
    {DashboardReportDictionary::Anthem_Clearwater, "Anthem Clearwater"},
    {DashboardReportDictionary::Anthem_Mesquite, "Anthem Mesquite"},
    {DashboardReportDictionary::Anthem_Town_East, "Anthem Town East"},
    {DashboardReportDictionary::Anthem_Valley_Ranch, "Anthem Valley Ranch"},
    {DashboardReportDictionary::Avery, "Avery"},
    {DashboardReportDictionary::Company, "Company"},
    {DashboardReportDictionary::Devon_on_Northgate, "Devon on Northgate"},
    {DashboardReportDictionary::Elliot_Apartments_on_Abernathy, "Elliot Apartments on Abernathy"},
    {DashboardReportDictionary::Elliot_Baymeadows_Apartments, "Elliot Baymeadows"},
    {DashboardReportDictionary::Elliot_Cocoplum, "Elliot Cocoplum"},
    {DashboardReportDictionary::Elliot_Gwinnett_Place, "Elliot Gwinnett Place"},
    {DashboardReportDictionary::Elliot_Norcross, "Elliot Norcross"},
    {DashboardReportDictionary::Elliot_Pioneer, "Elliot Pioneer"},
    {DashboardReportDictionary::Elliot_Roswell_Apartments, "Elliot Roswell"},
    {DashboardReportDictionary::Elliot_Windsprint, "Elliot Windsprint"},
    {DashboardReportDictionary::Estates_at_Las_Colinas, "Estates at Las Colinas"},
    {DashboardReportDictionary::Exchange_7272, "Exchange 7272"},
    {DashboardReportDictionary::Halston_Citrus_Ridge, "Halston Citrus Ridge"},
    {DashboardReportDictionary::Halston_Four_Corners, "Halston Four Corners"},
    {DashboardReportDictionary::Halston_Harris_Bridge, "Halston Harris Bridge"},
    {DashboardReportDictionary::Halston_Lakeside, "Halston Lakeside"},
    {DashboardReportDictionary::Halston_McDonough, "Halston McDonough"},
    {DashboardReportDictionary::Halston_On_Frankford, "Halston on Frankford"},
    {DashboardReportDictionary::Halston_Paces_Crossing, "Halston Paces Crossing"},
    {DashboardReportDictionary::Halston_Park_Central, "Halston Park Central"},
    {DashboardReportDictionary::Halston_Riverside, "Halston Riverside"},
    {DashboardReportDictionary::Halston_Shiloh_Valley_Apartments, "Halston Shiloh Valley"},
    {DashboardReportDictionary::Halston_South_Point_Apartment_Homes, "Halston South Point"},
    {DashboardReportDictionary::Halston_Waterleigh, "Halston Waterleigh"},
    {DashboardReportDictionary::Halston_World_Gateway_Apartment_Homes, "Halston World Gateway"},
    {DashboardReportDictionary::Lexington_Apartment_Homes, "Lexington"},
    {DashboardReportDictionary::Liv_at_Winter_Park_Apartment_Homes, "Liv at Winter Park"},
    {DashboardReportDictionary::Liv_on_the_Green_Apartment_Homes, "Liv on the Green"},
    {DashboardReportDictionary::MacArthur_Place_at_183, "MacArthur Place at 183"},
    {DashboardReportDictionary::Northern_Cross_Apartments, "Northern Cross"},
    {DashboardReportDictionary::Preserve_at_Preston, "Preserve at Preston"},
    {DashboardReportDictionary::Southside_Villas, "Southside Villas"},
    {DashboardReportDictionary::The_Apartments_at_Midtown_501, "The Apartments at Midtown 501"},
    {DashboardReportDictionary::The_Oaks_at_Valley_Ranch, "The Oaks at Valley Ranch"},
    {DashboardReportDictionary::Vista_121_Apartment_Homes, "Vista121"},
    {DashboardReportDictionary::Westmoor_Apartment_Homes, "Westmoor"}
};

#endif /* dictionaries__hpp */