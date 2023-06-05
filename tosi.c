#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <getopt.h>

int isnum(char *string);
void get_arguments(int argc, char **argv, char *function, double argument[2], int *precision);
void calculate(const char *function, const double const argument[2], double *result, char *unit);
void print_help();

int main(int argc, char **argv) {
    char function[3] = "", unit[3] = "";
    double argument[2] = {0}, result;
    int precision = 0;
    
    if (argc == 1)
        print_help();
    else
        get_arguments(argc, argv, function, argument, &precision);

    if (strcmp(function, "") == 0)
        exit(1);

    calculate(function, argument, &result, unit);
    
    if (precision)
        printf("%lf %s\n", result, unit);
    else
        printf("%.2lf %s\n", result, unit);

    system("pause");
    return 0;
}

void get_arguments(int argc, char **argv, char *function, double argument[2], int *precision) {
    int opt, arg_cnt = 0;
    while (((opt = getopt(argc, argv, ":f:a:hp")) != -1)) {
        switch(opt) {
            case 'f':
                strncpy(function, argv[optind-1], 3); break;

            case 'a':
                if (isnum(argv[optind-1]))
                    argument[arg_cnt++] = atof(argv[optind-1]);
                break;
            
            case 'p':
                *precision = 1; break;

            case 'h':
            case '?':
                print_help();

            case ':':
                printf("Missing argument after \'%c\'\n", (char) optopt); break;
        }
    }
}

int isnum(char *string) {
    char *chr = string;
    int is_num = 1;
    while (*chr != '\0') {
        is_num = is_num && isdigit(*chr++);
    }
    return is_num;
}

void calculate(const char *function, const double argument[2], double *result, char *unit) {
    if      (strcmp("fi", function) == 0) { // foot inch
        strcpy(unit, "m");
        *result = argument[0] * 0.3048 + argument[1] * 0.0254;
    } else if (strcmp("f", function) == 0) {// foot
        strcpy(unit, "m");
        *result = argument[0] * 0.3048;
    } else if (strcmp("i", function) == 0) {// inch
        strcpy(unit, "m");
        *result = argument[0] * 0.0254;
    } else if (strcmp("lb", function) == 0) {// pound mass
        strcpy(unit, "k");
        *result = argument[0] * 0.4535924;
    } else if (strcmp("oz", function) == 0) {// us customary fluid ounce
        strcpy(unit, "L");
        *result = argument[0] * 0.02957344;
    } else if (strcmp("ozuk", function) == 0) {// imperial fluid ounce
        strcpy(unit, "L");
        *result = argument[0] * 0.0284131;
    } else if (strcmp("K", function) == 0) {// kelvin
        strcpy(unit, "C");
        *result = argument[0] - 273.15;
    } else if (strcmp("F", function) == 0) {// fahrenheit
        strcpy(unit, "C");
        *result = (argument[0] - 32) * 0.5555555555555555802;
    } else {
        printf("Unrecognised function \'%s\'", function);
        exit(2);
    }
}

void print_help() {
    puts("Script for converting from various units to SI");
    puts("");
    puts("Options:");
    printf("%-12s %s\n", "-f function", "set source unit");
    printf("%-12s %s\n", "-a argument", "source unit quantity, repeatable");
    printf("%-12s %s\n", "-p", "displays with maximum precision");
    printf("%-12s %s\n", "-h", "show help");
    printf("%-12s %s\n", "-?", "show help");
    puts("");
    puts("Source units:");
    printf("%-6s %s\n", "fi",   "Foot+Inch, requires -a option twice");
    printf("%-6s %s\n", "f",    "Foot");
    printf("%-6s %s\n", "i",    "Inch");
    printf("%-6s %s\n", "lb",   "Pound mass");
    printf("%-6s %s\n", "oz",   "US customary fluid ounce");
    printf("%-6s %s\n", "ozuk", "Imperial fluid ounce");
    printf("%-6s %s\n", "K",    "Kelvin");
    printf("%-6s %s\n", "F",    "Fahrenheit");
    puts("");
    puts("Exit codes:");
    printf("%-6s %s\n", "0", "Terminated without errors");
    printf("%-6s %s\n", "1", "Did not receive function option");
    printf("%-6s %s\n", "2", "Function argument invalid");
    exit(0);
}