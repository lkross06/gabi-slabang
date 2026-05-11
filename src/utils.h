#pragma once

#include <Arduino.h>

const uint8_t MAX_FORMATTED_MD_SIZE = 15;
const char* months_abbv[12] PROGMEM = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

size_t formatMD(const uint8_t month, const uint8_t day, char* s){    
    // check base cases
    if (month <= 0 || month > 12) return 0;
    if (day <= 0 || day > 31) return 0;

    switch (month) {
        case 4:
        case 6:
        case 9:
        case 11:
            if (day > 30) return 0;
            break;
        case 2:
            if (day > 29) return 0;
            break;
        default:
            break;
    };

    size_t nbytes_written = 0;

    // write the month name
    strcpy(s, months_abbv[month - 1]);
    nbytes_written = strlen(s);

    // write the day
    s[nbytes_written++] = ' ';
    if (day >= 10) s[nbytes_written++] = (day / 10) + '0';
    s[nbytes_written++] = (day % 10) + '0';
    s[nbytes_written] = '\0'; //null terminate to allow strncat to work

    // write the ordinal suffix
    const char* suffix = "th";
    if (day < 11 || day > 13) {
        switch (day % 10) {
            case 1: suffix = "st"; break;
            case 2: suffix = "nd"; break;
            case 3: suffix = "rd"; break;
        }
    }
    
    strncat(s, suffix, 2);
    nbytes_written += 2;

    return nbytes_written;
}