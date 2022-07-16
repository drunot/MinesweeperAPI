#pragma once
#include <string>


std::string toHexString(unsigned char byte) {
    std::string s = "";

    switch(byte & 0xF0) {
        case 0x00:
            s = "0";
            break;

        case 0x10:
            s = "1";
            break;

        case 0x20:
            s = "2";
            break;

        case 0x30:
            s = "3";
            break;

        case 0x40:
            s = "4";
            break;

        case 0x50:
            s = "5";
            break;

        case 0x60:
            s = "6";
            break;

        case 0x70:
            s = "7";
            break;

        case 0x80:
            s = "8";
            break;

        case 0x90:
            s = "9";
            break;

        case 0xA0:
            s = "A";
            break;

        case 0xB0:
            s = "B";
            break;

        case 0xC0:
            s = "C";
            break;

        case 0xD0:
            s = "D";
            break;

        case 0xE0:
            s = "E";
            break;

        case 0xF0:
            s = "F";
            break;

        default:
            break;
    }

    switch(byte & 0x0F) {
        case 0x00:
            s += "0";
            break;

        case 0x01:
            s += "1";
            break;

        case 0x02:
            s += "2";
            break;

        case 0x03:
            s += "3";
            break;

        case 0x04:
            s += "4";
            break;

        case 0x05:
            s += "5";
            break;

        case 0x06:
            s += "6";
            break;

        case 0x07:
            s += "7";
            break;

        case 0x08:
            s += "8";
            break;

        case 0x09:
            s += "9";
            break;

        case 0x0A:
            s += "A";
            break;

        case 0x0B:
            s += "B";
            break;

        case 0x0C:
            s += "C";
            break;

        case 0x0D:
            s += "D";
            break;

        case 0x0E:
            s += "E";
            break;

        case 0x0F:
            s += "F";
            break;

        default:
            break;
    }

    return s;
}

