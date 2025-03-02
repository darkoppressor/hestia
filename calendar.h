/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef calendar_h
#define calendar_h

#include <cstdint>

class Calendar {
    private:
        std::uint32_t frame;
        std::uint32_t day;
        std::uint32_t week;
        std::uint32_t month;
        std::uint64_t year;

    public:
        enum class Change : std::uint8_t {
            NONE,
            DAY,
            WEEK,
            MONTH,
            YEAR
        };

        Calendar ();

        void reset();

        std::uint32_t get_day() const;
        std::uint32_t get_week() const;
        std::uint32_t get_month() const;
        std::uint64_t get_year() const;

        Change increment();
};

#endif
