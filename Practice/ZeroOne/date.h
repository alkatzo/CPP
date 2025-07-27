#pragma once

#include <string>
#include <string_view>
#include <array>

class Date
{
public:
    enum class Month {
        Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
    };

public:
    Date(size_t day, Month month, size_t year);
    std::string to_string() const;
    auto operator<=>(const Date&) const = default;

private:
    size_t day;
    Month month;
    size_t year;

    static constexpr std::array<std::string_view, 12> month_names = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    static constexpr std::array<size_t, 12> days_in_month = {
        31, // Jan
        28, // Feb (handle leap years separately)
        31, // Mar
        30, // Apr
        31, // May
        30, // Jun
        31, // Jul
        31, // Aug
        30, // Sep
        31, // Oct
        30, // Nov
        31  // Dec
    };

    bool is_leap_year() const;
};

