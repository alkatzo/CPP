#include "date.h"
#include <stdexcept>
#include <format>

Date::Date(size_t day, Month month, size_t year) :
    day{day}, month{month}, year{year}
{
    if(year < 1) {
        throw std::runtime_error("Invalid year");
    }

    int max_day = days_in_month[static_cast<size_t>(month)];
    if(month == Month::Feb && is_leap_year()) {
        max_day = 29;
    }

    if(day < 1 || day > max_day) {
        throw std::runtime_error("Invalid day");
    }
}

std::string Date::to_string() const
{
    return std::format("{:02d}-{}-{:04d}", day, month_names[static_cast<size_t>(month)], year);
}

bool Date::is_leap_year() const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
