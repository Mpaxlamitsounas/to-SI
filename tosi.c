#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <getopt.h>

#define UNIT_COUNT 10

typedef struct unit {
    char function_code[16];
    double (*transform) (const double[]);
    char SI_unit[16];
    char full_name[32];
    char comment[64];
} unit_t;

typedef struct args {
    char function[17];
    double arguments[4];
    int precision;
} args_t;

int isnum(char *string);
args_t get_arguments(int argc, char **argv);
unit_t get_unit(char *function);
void print_help();

double ft(const double arguments[]) { return arguments[0] * 0.3048; }
double in(const double arguments[]) { return arguments[0] * 0.0254; }
double ftin(const double arguments[]) { return ft(arguments) + in(arguments+1); }
double mi(const double arguments[]) { return arguments[0] * 1609.344; }
double nmi(const double arguments[])  {return arguments[0] * 1852; }
double lb(const double arguments[]) { return arguments[0] * 0.4535924; }
double oz(const double arguments[]) { return arguments[0] * 0.02957344; }
double ozuk(const double arguments[]) { return arguments[0] * 0.0284131; }
double F(const double arguments[]) {return (arguments[ 0] - 32) * 0.5555555555555555802; }
double K(const double arguments[]) { return arguments[0] - 273.15; }

unit_t units[UNIT_COUNT] = {
        (unit_t) {"ftin", ftin, "m",  "foot+inch",                ", requires -a option twice"},
        (unit_t) {"ft",   ft,   "m",  "foot",                     ""},
        (unit_t) {"in",   in,   "m",  "inch",                     ""},
        (unit_t) {"mi",   mi,   "m",  "mile",                     ""},
        (unit_t) {"nmi",  nmi,  "m",  "nautical mile",            ""},
        (unit_t) {"lb",   lb,   "kg", "pound mass",               ""},
        (unit_t) {"oz",   oz,   "L",  "US customary fluid ounce", ""},
        (unit_t) {"ozku", ozuk, "L",  "imperial fluid ounce",     ""},
        (unit_t) {"F",    F,    "C",  "fahrenheit",               ""},
        (unit_t) {"K",    K,    "C",  "kelvin",                   ""}
};

int main(int argc, char **argv) {

    args_t args;
    if (argc == 1)
        print_help();
    else
        args = get_arguments(argc, argv);

    if (strcmp(args.function, "") == 0) {
        puts("Missing function option");
        system("pause");
        exit(4);
    }

    unit_t unit = get_unit(args.function);

    if (args.precision)
        printf("%lf %s\n", unit.transform(args.arguments), unit.SI_unit);
    else
        printf("%.2lf %s\n", unit.transform(args.arguments), unit.SI_unit);


    system("pause");
    return 0;
}

args_t get_arguments(int argc, char **argv) {
    static args_t args = {"", 0, 0};
    int opt, arg_cnt = 0;
    while (((opt = getopt(argc, argv, ":f:a:hp")) != -1)) {
        switch(opt) {
            case 'f':
                strncpy_s(args.function, 17, argv[optind-1], 16); break;

            case 'a':
                if (arg_cnt == 2) {
                    puts("Too many argument options");
                    system("pause");
                    exit(3);
                }

                if (isnum(argv[optind-1]))
                    args.arguments[arg_cnt++] = atof(argv[optind-1]);
                break;
            
            case 'p':
                args.precision = 1; break;

            case 'h':
            case '?':
                print_help();

            case ':':
                printf("Missing argument after \'%c\'\n", (char) optopt); system("pause"); exit(1);
        }
    }
    
    return args;
}

int isnum(char *string) {
    char *chr = string;
    int is_num = 1;
    while (*chr != '\0') {
        is_num = is_num && isdigit(*chr++);
    }
    return is_num;
}

unit_t get_unit(char *function) {
    for (size_t i = 0; i < UNIT_COUNT; i++) {
        if (strcmp(function, units[i].function_code) == 0)
            return units[i];
    }
    printf("Unrecognised function \'%s\'\n", function);
    system("pause");
    exit(2);
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
    for (size_t i = 0; i < UNIT_COUNT; i++)
        printf("%-6s %s%s\n", units[i].function_code, units[i].full_name, units[i].comment);
    puts("");
    puts("Exit codes:");
    printf("%-6s %s\n", "0", "Terminated without errors");
    printf("%-6s %s\n", "1", "Missing option argument");
    printf("%-6s %s\n", "2", "Function argument invalid");
    printf("%-6s %s\n", "3", "Too many argument options");
    printf("%-6s %s\n", "4", "Missing function option");
    exit(0);
}