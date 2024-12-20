/*
 * MIT License
 * 
 * Copyright (c) 2024 Konstantinos Despoinidis
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/


/*
 * Usage: 
 * #define CLIB_IMPLEMENTATION
 * #inlcude "clib.h"
 *
 * -[TOC]-
 * 1. SYSTEM
 * 2. SAFE MEMORY
 * 3. MENUS // needs its own define!
 * 4. UTILS
 * 5. ANSI
 * 6. FILES
 * 7. LOGGING
 * 8. CLI
 * */

#ifndef CLIB_H
#define CLIB_H

#define CLIB_VERSION_MAJOR 0
#define CLIB_VERSION_MINOR 1
#define CLIB_VERSION_PATCH 1
#define CLIB_VERSION  "0.1.1"

#ifndef CLIBAPI
    #define CLIBAPI static
#endif

#pragma GCC diagnostic ignored "-Wunused-function"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <time.h>

// START [TYPES] START //
typedef const char * Cstr;
typedef uint8_t Bool;

typedef struct {
    Cstr* items;
    size_t count;
} CstrArray;

typedef struct {
    char* help;
    char* full;
    char abr;
    size_t argument_required;
} CliArg;

typedef struct {
    CliArg** args;
    size_t count;
    size_t capacity;
} CliArguments;

// END [TYPES] END//

// START [DECLARATIONS] START //
#ifndef _WIN32
    #define PATH_SEP "/"
#else 
    #define PATH_SEP "\\"
#endif // _WIN32

#define true 1
#define false 0
#define BOOL(x) (x ? "true" : "false")

// ANSI
#define ANSI_RESET "\e[0;39m"
#define ANSI_BOLD "\e[1m"
#define ANSI_UNDERLINE "\e[4m"
#define ANSI_ITALIC "\e[3m"
#define ANSI_CLEAR "\e[2J"
#define ANSI_ERASE_LINE "\e[2K"
#define ANSI_HIDE_CURSOR() printf("\e[?25l")
#define ANSI_SHOW_CURSOR() printf("\e[?25h")
#define ANSI_GOTOXY(x,y) printf("\e[%d;%dH", (y), (x))
#define ANSI_MOVE_CURSOR_UP(x) printf("\e[%zuA", x)
#define ANSI_MOVE_CURSOR_DOWN(x) printf("\e[%dB", x);
#define ANSI_MOVE_CURSOR_RIGHT(x) printf("\e[%dC", x);
#define ANSI_MOVE_CURSOR_LEFT(x) printf("\e[%dD", x);
#define ANSI_CLEAR_BELOW_CURSOR printf("\e[J")
#define ANSI_CURSOR_BLOCK() printf("\033[1 q");
#define ANSI_CURSOR_UNDERSCORE() printf("\033[4 q");
#define ANSI_CURSOR_BAR() printf("\033[5 q");

CLIBAPI char* clib_ansi_combine(const char* seq1, const char* seq2);
#define ANSI_COMBINE(seq1, seq2) \
    clib_ansi_combine(seq1, seq2)

#define ANSI_BLACK "\e[30m"
#define ANSI_RED "\e[31m"
#define ANSI_GREEN "\e[32m"
#define ANSI_YELLOW "\e[33m"
#define ANSI_BLUE "\e[34m"
#define ANSI_PURPLE "\e[35m"
#define ANSI_CYAN "\e[36m"
#define ANSI_LGREY "\e[37m"
#define ANSI_DGREY "\e[38m"

CLIBAPI Cstr clib_ansi_color(int color, int bg);
CLIBAPI void clib_ansi_clear_screen();
CLIBAPI void clib_ansi_print_color_table();

#define COLOR_BG(c) clib_ansi_color(c, 1)
#define COLOR_FG(c) clib_ansi_color(c, 0)

// SYSTEM
#ifndef _WIN32
CLIBAPI char* clib_system_execute_command(const char* command);
CLIBAPI char* clib_system_get_env(const char* varname);
CLIBAPI int clib_system_set_env(const char* varname, const char* value, int overwrite);
CLIBAPI int clib_system_unset_env(const char* varname);
#endif

// SAFE MEMORY
CLIBAPI void* clib_safe_malloc(size_t size);
CLIBAPI void* clib_safe_calloc(size_t nmemb, size_t size);
CLIBAPI void* clib_safe_realloc(void *ptr, size_t size);
CLIBAPI void clib_safe_free(void **ptr);

// FILES
CLIBAPI int clib_file_create_directory(const char *path);
CLIBAPI int clib_file_directory_exists(const char *path);
CLIBAPI void clib_file_create(const char *filename);
CLIBAPI void clib_file_write(const char *filename, const char *data, Cstr mode);
CLIBAPI char* clib_file_read(const char *filename, const char* mode);
CLIBAPI void clib_file_delete(const char *filename);
CLIBAPI void clib_file_append(const char *filename, const char *data);
CLIBAPI void clib_file_copy(const char *source, const char *destination);
CLIBAPI void clib_file_move(const char *source, const char *destination);
CLIBAPI long clib_file_file(const char *filename);
CLIBAPI int clib_file_exists(const char *filename);

// STRINGS
#define ITOA(i) clib_str_format("%d", i) 
#define FTOA(f) clib_str_format("%f", f)
#define STR(x) #x
#define STREQ(x, y) (strcmp(x, y) == 0) 
CLIBAPI char* clib_str_format(const char *format, ...);
CLIBAPI char* clib_str_buffer_init();
CLIBAPI void clib_str_append_ln(char** buffer, Cstr text);
CLIBAPI void clib_str_append(char** buffer, const char* text);
CLIBAPI void clib_str_clean(char** buffer);
CLIBAPI char* clib_str_replace(const char* str, const char* search, const char* replace);
CLIBAPI void println(const char* fmt, ...);

// UTILS
#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
#define SWAP(x, y) do { \
    typeof(x) temp = x; \
    x = y; \
    y = temp; \
} while (0)

#if defined(__GNUC__) || defined(__clang__)
    #define LIKELY(x) __builtin_expect(!!(x), 1)
#else
    #define LIKELY(x) (x)
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
    #define UNLIKELY(x) (x)
#endif

#define freec(ptr) \
    free((void*) ptr)

CLIBAPI int clib_eu_mod(int a, int b);

// CLI

#define LOOP_ARGS(opt, args) \
    while((opt = getopt_long(argc, argv, clib_cli_generate_format_string(args), clib_cli_get_options(args), NULL)) != -1)

CLIBAPI char* clib_cli_shift_args(int *argc, char ***argv);
CLIBAPI CliArg* clib_cli_create_argument(char abr, Cstr full, Cstr help, size_t argument_required);
CLIBAPI void clib_cli_clean_arguments(CliArguments* arguments);
CLIBAPI void clib_cli_add_arg(CliArg* arg, CliArguments* arguments);
CLIBAPI CliArguments clib_cli_make_arguments(size_t capacity, CliArg* first, ...);
CLIBAPI struct option* clib_cli_get_options(CliArguments args);
CLIBAPI char* clib_cli_generate_format_string(CliArguments args);
CLIBAPI void clib_cli_help(CliArguments args, Cstr usage, Cstr footer);

// LOGGING
#define HANDLE_ERROR(msg) \
    do { \
        perror(CONCAT("[ERRO] ", msg)); \
        printf("\n"); \
        exit(1); \
    } while (0) \

typedef enum {
    CLIB_INFO,
    CLIB_WARN,
    CLIB_ERRO,
    CLIB_DEBU,
    CLIB_PANIC,
} ClibLog;

CLIBAPI void clib_log(int log_level, char* format, ...);

#define LOG(stream, type, format, ...) \
    do { \
        fprintf(stream, "[%s] ", type); \
        fprintf(stream, format, ##__VA_ARGS__); \
        fprintf(stream, "\n"); \
    } while(0)

#define INFO(format, ...) \
    LOG(stdout, "INFO", format, ##__VA_ARGS__)

#define ERRO(format, ...) \
    LOG(stderr, "ERRO", format, ##__VA_ARGS__)

#define WARN(format, ...) \
    LOG(stderr, "WARN", format, ##__VA_ARGS__)

#ifdef DEBUG
    #define DEBU(format, ...) \
        LOG(stderr, "DEBU", format, ##__VA_ARGS__)
#else
    #define DEBU(format, ...) 
#endif // DEBUG

#define PANIC(format, ...)                            \
    do {                                              \
        LOG(stderr, "PANIC", format, ##__VA_ARGS__);  \
        exit(1);                                      \
    } while(0)

#define DEMO(expr)                    \
    do {                              \
        LOG(stdout, "DEMO", #expr);   \
        expr;                         \
    } while(0)

CLIBAPI void remote_log(const char *term, const char *format, ...);
#ifdef DEBUG
    #define REMOTE_LOG(term, format, ...) \
        remote_log(term, format, ##__VA_ARGS__)
#else
    #define REMOTE_LOG(term, format, ...)
#endif // DEBUG


// MENUS
#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>

    CLIBAPI int _getch();
#endif

// TODO: Add more keys
typedef enum {
    // arrows (defined later)
    CLIB_KEY_ARROW_UP,
    CLIB_KEY_ARROW_DOWN,
    CLIB_KEY_ARROW_LEFT,
    CLIB_KEY_ARROW_RIGHT,

    CLIB_KEY_UNKNOWN = -1,
    CLIB_KEY_ENTER = 10,
    CLIB_KEY_BACKSPACE = 127,
    CLIB_KEY_TAB = 9,
    CLIB_KEY_ESC = 27,
    CLIB_KEY_SPACE = 32,

    // numbers
    CLIB_KEY_0 = '0',
    CLIB_KEY_1 = '1',
    CLIB_KEY_2 = '2',
    CLIB_KEY_3 = '3',
    CLIB_KEY_4 = '4',
    CLIB_KEY_5 = '5',
    CLIB_KEY_6 = '6',
    CLIB_KEY_7 = '7',
    CLIB_KEY_8 = '8',
    CLIB_KEY_9 = '9',

    // uppercase
    CLIB_KEY_A = 'A',
    CLIB_KEY_B = 'B',
    CLIB_KEY_C = 'C',
    CLIB_KEY_D = 'D',
    CLIB_KEY_E = 'E',
    CLIB_KEY_F = 'F',
    CLIB_KEY_G = 'G',
    CLIB_KEY_H = 'H',
    CLIB_KEY_I = 'I',
    CLIB_KEY_J = 'J',
    CLIB_KEY_K = 'K',
    CLIB_KEY_L = 'L',
    CLIB_KEY_M = 'M',
    CLIB_KEY_N = 'N',
    CLIB_KEY_O = 'O',
    CLIB_KEY_P = 'P',
    CLIB_KEY_Q = 'Q',
    CLIB_KEY_R = 'R',
    CLIB_KEY_S = 'S',
    CLIB_KEY_T = 'T',
    CLIB_KEY_U = 'U',
    CLIB_KEY_V = 'V',
    CLIB_KEY_W = 'W',
    CLIB_KEY_X = 'X',
    CLIB_KEY_Y = 'Y',
    CLIB_KEY_Z = 'Z',

    // lowercase
    CLIB_KEY_a = 'a',
    CLIB_KEY_b = 'b',
    CLIB_KEY_c = 'c',
    CLIB_KEY_d = 'd',
    CLIB_KEY_e = 'e',
    CLIB_KEY_f = 'f',
    CLIB_KEY_g = 'g',
    CLIB_KEY_h = 'h',
    CLIB_KEY_i = 'i',
    CLIB_KEY_j = 'j',
    CLIB_KEY_k = 'k',
    CLIB_KEY_l = 'l',
    CLIB_KEY_m = 'm',
    CLIB_KEY_n = 'n',
    CLIB_KEY_o = 'o',
    CLIB_KEY_p = 'p',
    CLIB_KEY_q = 'q',
    CLIB_KEY_r = 'r',
    CLIB_KEY_s = 's',
    CLIB_KEY_t = 't',
    CLIB_KEY_u = 'u',
    CLIB_KEY_v = 'v',
    CLIB_KEY_w = 'w',
    CLIB_KEY_x = 'x',
    CLIB_KEY_y = 'y',
    CLIB_KEY_z = 'z',

    // special characters
    CLIB_KEY_EXCLAMATION = '!',
    CLIB_KEY_AT = '@',
    CLIB_KEY_HASH = '#',
    CLIB_KEY_DOLLAR = '$',
    CLIB_KEY_PERCENT = '%',
    CLIB_KEY_CARET = '^',
    CLIB_KEY_AMPERSAND = '&',
    CLIB_KEY_ASTERISK = '*',
    CLIB_KEY_LEFT_PAREN = '(',
    CLIB_KEY_RIGHT_PAREN = ')',
    CLIB_KEY_DASH = '-',
    CLIB_KEY_UNDERSCORE = '_',
    CLIB_KEY_EQUAL = '=',
    CLIB_KEY_PLUS = '+',
    CLIB_KEY_LEFT_BRACKET = '[',
    CLIB_KEY_RIGHT_BRACKET = ']',
    CLIB_KEY_LEFT_BRACE = '{',
    CLIB_KEY_RIGHT_BRACE = '}',
    CLIB_KEY_SEMICOLON = ';',
    CLIB_KEY_COLON = ':',
    CLIB_KEY_SINGLE_QUOTE = '\'',
    CLIB_KEY_DOUBLE_QUOTE = '"',
    CLIB_KEY_COMMA = ',',
    CLIB_KEY_PERIOD = '.',
    CLIB_KEY_LESS = '<',
    CLIB_KEY_GREATER = '>',
    CLIB_KEY_SLASH = '/',
    CLIB_KEY_QUESTION = '?',
    CLIB_KEY_BACKSLASH = '\\',
    CLIB_KEY_PIPE = '|',
    CLIB_KEY_BACKTICK = '`',
    CLIB_KEY_TILDE = '~',
} ClibKey;

typedef void (*ClibPrintOptionFunc)(Cstr option, int is_selected, int color);
CLIBAPI void clib_default_print_option(Cstr option, int is_selected, int color);
CLIBAPI void clib_arrow_print_option(Cstr option, int is_selected, int color);
CLIBAPI void clib_brackets_print_option(Cstr option, int is_selected, int color);
CLIBAPI void clib_enable_input_buffering();
CLIBAPI void clib_disable_input_buffering();
CLIBAPI int clib_getch();
CLIBAPI int clib_menu(Cstr title, int color, ClibPrintOptionFunc print_option, Cstr first_option, ...);

// END [DECLARATIONS] END//

// START [IMPLEMENTATIONS] START //
#ifdef CLIB_IMPLEMENTATION
CLIBAPI void remote_log(const char *term, const char *format, ...)
{
    static FILE *log_terminal = NULL;

    if (log_terminal == NULL) {
        log_terminal = fopen(term, "w");
        if (log_terminal == NULL) {
            perror("Error opening remote terminal for logging");
            exit(EXIT_FAILURE);
        }
    }

    // // Get current time
    // time_t t = time(NULL);
    // struct tm *tm_info = localtime(&t);
    // char buffer[26];
    // strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    //
    // // Write timestamp to log
    // fprintf(log_terminal, "[%s] ", buffer);

    va_list args;
    va_start(args, format);
    vfprintf(log_terminal, format, args);
    va_end(args);

    fprintf(log_terminal, "\n");
    fflush(log_terminal);
}

CLIBAPI char* clib_ansi_combine(const char* seq1, const char* seq2)
{
    if (seq1 == NULL || seq2 == NULL) {
        return NULL; // Handle null inputs
    }

    const char* prefix = "\e[";
    const char* suffix = "m";

    const char* seq1_inner = seq1 + 2; // Skip \e[
    size_t seq1_len = strlen(seq1_inner) - 1; // Remove the trailing 'm'

    const char* seq2_inner = seq2 + 2; // Skip \e[
    size_t seq2_len = strlen(seq2_inner) - 1; // Remove the trailing 'm'

    char* result = (char*)malloc(strlen(prefix) + seq1_len + 1 + seq2_len + strlen(suffix) + 1);
    if (!result) {
        return NULL; // Memory allocation failure
    }

    sprintf(result, "%s%.*s;%.*s%s", prefix, (int)seq1_len, seq1_inner, (int)seq2_len, seq2_inner, suffix);

    return result;
}

CLIBAPI char* clib_str_buffer_init()
{
    char* buffer = (char*) malloc(1);
    memset(buffer, 0, 1);

    return buffer;
}

CLIBAPI void clib_str_append_ln(char** buffer, Cstr text)
{
    assert(buffer != NULL && *buffer != NULL);
    assert(text != NULL);

    Cstr new_text = clib_str_format("%s\n", text);
    size_t new_size = strlen(*buffer) + strlen(new_text) + 1;
    *buffer = (char*) realloc(*buffer, new_size);
    if (*buffer == NULL) {
        PANIC("Failed to reallocate memory");
    }
    strcat(*buffer, new_text);
    free((char*) new_text);
}

CLIBAPI void clib_str_append(char** buffer, const char* text)
{
    assert(buffer != NULL && *buffer != NULL);
    assert(text != NULL);

    size_t current_size = strlen(*buffer);
    size_t text_len = strlen(text);

    *buffer = (char*) realloc(*buffer, current_size + text_len + 1);
    if (*buffer == NULL) {
        PANIC("Failed to reallocate memory");
    }

    strcat(*buffer, text);
}

CLIBAPI void clib_str_clean(char** buffer)
{
    free(*buffer);
    *buffer = NULL;
}

CLIBAPI void println(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf(fmt, args);
    va_end(args);
}

CLIBAPI char* clib_str_replace(const char* str, const char* search, const char* replace)
{
    char* result = NULL;
    char* idx = NULL;
    size_t search_len = strlen(search);
    size_t replace_len = strlen(replace);
    size_t str_len = strlen(str);
    size_t buffer_size = str_len;

    // Allocate initial buffer
    result = (char*)malloc(buffer_size + 1);
    if (result == NULL) {
        return NULL;
    }
    strcpy(result, str);

    // Find and replace all occurrences
    idx = strstr(result, search);
    while (idx != NULL) {
        size_t offset = idx - result;
        buffer_size += replace_len - search_len;
        result = (char*)realloc(result, buffer_size + 1);
        if (result == NULL) {
            return NULL;
        }
        memmove(result + offset + replace_len, result + offset + search_len, strlen(result + offset + search_len) + 1);
        memcpy(result + offset, replace, replace_len);
        idx = strstr(result + offset + replace_len, search);
    }

    return result;
}

CLIBAPI char* clib_str_format(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    size_t size = vsnprintf(NULL, 0, format, args) + 1; // +1 for the null terminator
    va_end(args);

    char* formatted_string = (char*) malloc(size);
    if (formatted_string == NULL) {
        return NULL;
    }

    va_start(args, format);
    vsnprintf(formatted_string, size, format, args);
    va_end(args);

    formatted_string[size-1] = '\0';

    return formatted_string;
}

CLIBAPI CliArg* clib_cli_create_argument(char abr, Cstr full, Cstr help, size_t argument_required)
{
    CliArg* arg = (CliArg*) clib_safe_malloc(sizeof(CliArg));

    if(full){
        arg->full = (char*) malloc(strlen(full) + 1);
        if (!arg->full) {
            free(arg);
            return NULL;
        }
        strcpy(arg->full, full);
    }

    arg->help = (char*) malloc(strlen(help) + 1);
    if (!arg->help) {
        free(arg->full);
        free(arg);
        return NULL;
    }
    strcpy(arg->help, help);

    arg->abr = abr;
    arg->argument_required = argument_required;

    return arg;
}

CLIBAPI void clib_cli_add_arg(CliArg* arg, CliArguments* arguments)
{
    if(arguments->capacity <= arguments->count) {
        ERRO("Max capacity");
        return;
    }

    arguments->args[arguments->count++] = arg;
}

CLIBAPI void clib_cli_clean_arguments(CliArguments* arguments)
{
    for(size_t i = 0; i < arguments->count; ++i){
        free(arguments->args[i]->full);
        free(arguments->args[i]->help);
        free(arguments->args[i]);
    }
    free(arguments->args);
}

CLIBAPI CliArguments clib_cli_make_arguments(size_t capacity, CliArg* first, ...)
{
    CliArguments arguments = { .capacity = capacity };

    arguments.args = (CliArg**) clib_safe_malloc(sizeof(arguments.args[0]) * arguments.capacity);
    
    if (arguments.args == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }

    if(first == NULL) return arguments;

    arguments.args[arguments.count++] = first;

    va_list args;
    va_start(args, first);
    for (CliArg* next = va_arg(args, CliArg*); next != NULL; next = va_arg(args, CliArg*)) {
        if(arguments.capacity == arguments.count) break;
        arguments.args[arguments.count++] = next;
    }
    va_end(args);


    return arguments;
}

CLIBAPI struct option* clib_cli_get_options(CliArguments args)
{
    if (args.count == 0) {
        return NULL;
    }

    struct option* options = (struct option*) malloc(sizeof(struct option) * args.count);
    if (!options) {
        return NULL;
    }

    for (size_t i = 0; i < args.count; ++i) {
        CliArg* arg = args.args[i];
        options[i].val = arg->abr;
        if(arg->full) options[i].name = arg->full;
        options[i].flag = NULL;
        options[i].has_arg = arg->argument_required;
    }

    return options;
}

static size_t get_max_length(CliArguments args)
{
    size_t max_len = 0;

    for(size_t i = 0; i < args.count; ++i){
        if(args.args[i] == NULL) continue;

        size_t current_len = 0;
        if(args.args[i]->full == NULL)
            current_len = snprintf(NULL, 0, "-%c", args.args[i]->abr);
        else
            current_len = snprintf(NULL, 0, "-%c --%s", args.args[i]->abr, args.args[i]->full);
        if(current_len > max_len) max_len = current_len;
    }

    return max_len;
}

static char* add_spaces(size_t max_len, CliArg* arg)
{
    size_t arg_len = 0;
    if(arg->full == NULL)
        arg_len = snprintf(NULL, 0, "-%c", arg->abr);
    else 
         arg_len = snprintf(NULL, 0, "-%c --%s", arg->abr, arg->full);

    size_t GAP = 4;
    size_t final_size = max_len - arg_len + GAP + 1; // +1 for '\0'
    char* spaces = (char*) malloc(final_size);
    if(spaces == NULL){
        return NULL;
    }

    for(size_t i = 0; i < final_size - 1; ++i){
        spaces[i] = ' ';
    }
    spaces[final_size-1] = '\0';

    return spaces;
}

CLIBAPI void clib_cli_help(CliArguments args, Cstr usage, Cstr footer)
{
    if(usage) printf("Usage: %s\n\n", usage);

    size_t max_len = get_max_length(args);
    for(size_t i = 0; i < args.count; ++i){
        Cstr has_arg = NULL;
        switch(args.args[i]->argument_required){
            case no_argument:
                has_arg = "no argument";
                break;
            case required_argument:
                has_arg = "requires argument";
                break;
            case optional_argument:
                has_arg = "optional argument";
                break;
        }

        char* spaces = add_spaces(max_len, args.args[i]);
        if(spaces == NULL) return;
        Cstr arg_required = COLOR_FG(args.args[i]->argument_required + 1);
        if(args.args[i]->full){
            printf("-%c --%s%s%s %s[%s]%s\n", 
                args.args[i]->abr, 
                args.args[i]->full,
                spaces,
                args.args[i]->help,
                arg_required,
                has_arg,
                ANSI_RESET
            );
        } else {
            printf("-%c%s%s %s[%s]%s\n", 
                args.args[i]->abr, 
                spaces,
                args.args[i]->help,
                arg_required,
                has_arg,
                ANSI_RESET
            );
        }
        free((char*) arg_required);
        free(spaces);
    }
    printf("\n");

    if(footer) printf("%s\n", footer);
}

CLIBAPI char* clib_cli_generate_format_string(CliArguments args)
{
    size_t length = 1;
    for (size_t i = 0; i < args.count; ++i) {
        length += 1;
        if (args.args[i]->argument_required) {
            length += 1;
        }
    }

    char* fmt = (char*) malloc(length * sizeof(char));
    if (!fmt) {
        return NULL;
    }

    fmt[0] = '\0';

    for (size_t i = 0; i < args.count; ++i) {
        char abr[2] = {args.args[i]->abr, 0};
        if(args.args[i]->argument_required == optional_argument) strcat(fmt, ":");
        strcat(fmt, abr);
        if (args.args[i]->argument_required) strcat(fmt, ":");
    }
    strcat(fmt, "\0");

    return fmt;
}

CLIBAPI void clib_log(int log_level, char* format, ...)
{
    switch(log_level){
    case CLIB_INFO:
        fprintf(stderr, "[INFO] ");
        break;
    case CLIB_WARN:
        fprintf(stderr, "[WARN] ");
        break;
    case CLIB_ERRO:
        fprintf(stderr, "[ERRO] ");
        break;
    case CLIB_DEBU:
        fprintf(stderr, "[DEBU] ");
        break;
    case CLIB_PANIC:
        fprintf(stderr, "[PANIC] ");
        break;
    default:
        assert(0 && "unreachable");
    }

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");

    if(log_level == CLIB_PANIC) exit(1);
}

#ifndef _WIN32
    CLIBAPI int _getch() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

CLIBAPI void clib_enable_input_buffering(){
    #ifdef _WIN32
        // Enable console input buffering
        HANDLE hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);

        DWORD consoleMode;
        GetConsoleMode(hConsoleInput, &consoleMode);

        consoleMode |= ENABLE_LINE_INPUT;

        SetConsoleMode(hConsoleInput, consoleMode);
    #else
        struct termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);

    #endif
    ANSI_SHOW_CURSOR();
}

CLIBAPI void clib_disable_input_buffering(){
    #ifdef _WIN32
        // Disable console input buffering
        DWORD mode;
        HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(hInput, &mode);
        SetConsoleMode(hInput, mode & ~ENABLE_ECHO_INPUT & ~ENABLE_LINE_INPUT);
    #else
        struct termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);

    #endif
    ANSI_HIDE_CURSOR();
}

CLIBAPI int clib_getch() {
    int ch;
    #ifdef _WIN32
        ch = _getch();
        if (ch == 0 || ch == 224) {
            // Handle extended keys (arrows, function keys)
            ch = _getch();
            switch (ch) {
                case 72: return CLIB_KEY_ARROW_UP;
                case 80: return CLIB_KEY_ARROW_DOWN;
                case 75: return CLIB_KEY_ARROW_LEFT;
                case 77: return CLIB_KEY_ARROW_RIGHT;
                default: return CLIB_KEY_UNKNOWN;
            }
        }
    #else
        ch = _getch();
        if (ch == 27) {
            if (getchar() == '[') {
                switch (getchar()) {
                    case 'A': return CLIB_KEY_ARROW_UP;
                    case 'B': return CLIB_KEY_ARROW_DOWN;
                    case 'C': return CLIB_KEY_ARROW_RIGHT;
                    case 'D': return CLIB_KEY_ARROW_LEFT;
                    default: return CLIB_KEY_UNKNOWN;
                }
            }
            return CLIB_KEY_ESC;
        }
    #endif

    switch (ch) {
        case 10: return CLIB_KEY_ENTER;
        case 127: return CLIB_KEY_BACKSPACE;
        case 9: return CLIB_KEY_TAB;
        case 27: return CLIB_KEY_ESC;
        case 32: return CLIB_KEY_SPACE;

        // digits
        case '0': return CLIB_KEY_0;
        case '1': return CLIB_KEY_1;
        case '2': return CLIB_KEY_2;
        case '3': return CLIB_KEY_3;
        case '4': return CLIB_KEY_4;
        case '5': return CLIB_KEY_5;
        case '6': return CLIB_KEY_6;
        case '7': return CLIB_KEY_7;
        case '8': return CLIB_KEY_8;
        case '9': return CLIB_KEY_9;

        // uppercase
        case 'A': return CLIB_KEY_A;
        case 'B': return CLIB_KEY_B;
        case 'C': return CLIB_KEY_C;
        case 'D': return CLIB_KEY_D;
        case 'E': return CLIB_KEY_E;
        case 'F': return CLIB_KEY_F;
        case 'G': return CLIB_KEY_G;
        case 'H': return CLIB_KEY_H;
        case 'I': return CLIB_KEY_I;
        case 'J': return CLIB_KEY_J;
        case 'K': return CLIB_KEY_K;
        case 'L': return CLIB_KEY_L;
        case 'M': return CLIB_KEY_M;
        case 'N': return CLIB_KEY_N;
        case 'O': return CLIB_KEY_O;
        case 'P': return CLIB_KEY_P;
        case 'Q': return CLIB_KEY_Q;
        case 'R': return CLIB_KEY_R;
        case 'S': return CLIB_KEY_S;
        case 'T': return CLIB_KEY_T;
        case 'U': return CLIB_KEY_U;
        case 'V': return CLIB_KEY_V;
        case 'W': return CLIB_KEY_W;
        case 'X': return CLIB_KEY_X;
        case 'Y': return CLIB_KEY_Y;
        case 'Z': return CLIB_KEY_Z;

        // lowercase
        case 'a': return CLIB_KEY_a;
        case 'b': return CLIB_KEY_b;
        case 'c': return CLIB_KEY_c;
        case 'd': return CLIB_KEY_d;
        case 'e': return CLIB_KEY_e;
        case 'f': return CLIB_KEY_f;
        case 'g': return CLIB_KEY_g;
        case 'h': return CLIB_KEY_h;
        case 'i': return CLIB_KEY_i;
        case 'j': return CLIB_KEY_j;
        case 'k': return CLIB_KEY_k;
        case 'l': return CLIB_KEY_l;
        case 'm': return CLIB_KEY_m;
        case 'n': return CLIB_KEY_n;
        case 'o': return CLIB_KEY_o;
        case 'p': return CLIB_KEY_p;
        case 'q': return CLIB_KEY_q;
        case 'r': return CLIB_KEY_r;
        case 's': return CLIB_KEY_s;
        case 't': return CLIB_KEY_t;
        case 'u': return CLIB_KEY_u;
        case 'v': return CLIB_KEY_v;
        case 'w': return CLIB_KEY_w;
        case 'x': return CLIB_KEY_x;
        case 'y': return CLIB_KEY_y;
        case 'z': return CLIB_KEY_z;

        // special characters
        case '!': return CLIB_KEY_EXCLAMATION;
        case '@': return CLIB_KEY_AT;
        case '#': return CLIB_KEY_HASH;
        case '$': return CLIB_KEY_DOLLAR;
        case '%': return CLIB_KEY_PERCENT;
        case '^': return CLIB_KEY_CARET;
        case '&': return CLIB_KEY_AMPERSAND;
        case '*': return CLIB_KEY_ASTERISK;
        case '(': return CLIB_KEY_LEFT_PAREN;
        case ')': return CLIB_KEY_RIGHT_PAREN;
        case '-': return CLIB_KEY_DASH;
        case '_': return CLIB_KEY_UNDERSCORE;
        case '=': return CLIB_KEY_EQUAL;
        case '+': return CLIB_KEY_PLUS;
        case '[': return CLIB_KEY_LEFT_BRACKET;
        case ']': return CLIB_KEY_RIGHT_BRACKET;
        case '{': return CLIB_KEY_LEFT_BRACE;
        case '}': return CLIB_KEY_RIGHT_BRACE;
        case ';': return CLIB_KEY_SEMICOLON;
        case ':': return CLIB_KEY_COLON;
        case '\'': return CLIB_KEY_SINGLE_QUOTE;
        case '"': return CLIB_KEY_DOUBLE_QUOTE;
        case ',': return CLIB_KEY_COMMA;
        case '.': return CLIB_KEY_PERIOD;
        case '<': return CLIB_KEY_LESS;
        case '>': return CLIB_KEY_GREATER;
        case '/': return CLIB_KEY_SLASH;
        case '?': return CLIB_KEY_QUESTION;
        case '\\': return CLIB_KEY_BACKSLASH;
        case '|': return CLIB_KEY_PIPE;
        case '`': return CLIB_KEY_BACKTICK;
        case '~': return CLIB_KEY_TILDE;

        default: return ch;
    }
}


CLIBAPI void clib_default_print_option(Cstr option, int is_selected, int color){
    is_selected ? printf("%s%s%s", COLOR_BG(color), option, ANSI_RESET) : printf("%s", option);
}

CLIBAPI void clib_arrow_print_option(Cstr option, int is_selected, int color){
    is_selected ? printf("%s>%s %s", COLOR_FG(color), ANSI_RESET, option) : printf("  %s", option);
}

CLIBAPI void clib_brackets_print_option(Cstr option, int is_selected, int color){
    is_selected ? printf("%s[%s%s%s]%s", COLOR_FG(color), ANSI_RESET, option, COLOR_FG(color), ANSI_RESET) : printf(" %s ", option);
}

CLIBAPI int clib_menu(Cstr title, int color, ClibPrintOptionFunc print_option, Cstr first_option, ...){
    clib_disable_input_buffering();

    int selected = 0;
    CstrArray options = {0};

    if (first_option == NULL) {
        return -1;
    }

    va_list args;
    va_start(args, first_option);
    options.count++;
    Cstr next = NULL;
    for (next = va_arg(args, Cstr); next != NULL; ) {
        next = va_arg(args, Cstr);
        if(next)
            options.count++;
    }
    va_end(args);

    options.items = (Cstr*) malloc(sizeof(options.items[0]) * options.count);
    if (options.items == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }
    options.count = 0;

    options.items[options.count++] = first_option;

    va_start(args, first_option);
    next = NULL;
    for (next = va_arg(args, Cstr); next != NULL;) {
        options.items[options.count] = next;
        next = va_arg(args, Cstr);
        if(next != NULL) options.count++;
    }

    va_end(args);

    while(true){
        if(title != NULL){
            printf("%s%s%s\n", COLOR_FG(color), title, ANSI_RESET);
        }
        for(size_t i = 0; i < options.count; ++i){
            print_option(options.items[i], selected == i, color);
            printf("\n");
        }
        
        int pressed = clib_getch();
        switch (pressed) {
            case CLIB_KEY_ARROW_UP:
                selected = clib_eu_mod((selected-1), options.count);
                break;
            case CLIB_KEY_ARROW_DOWN:
                selected = clib_eu_mod((selected+1), options.count);
                break;
            case CLIB_KEY_ENTER:
                clib_enable_input_buffering();
                free(options.items);
                return selected; 
            default:
                break;
        }

        ANSI_MOVE_CURSOR_UP(options.count + (title != NULL));
        ANSI_CLEAR_BELOW_CURSOR;
    }
}

CLIBAPI int clib_eu_mod(int a, int b){
    if (b == 0) {
        // Handle division by zero case
        fprintf(stderr, "Error: Division by zero is undefined.\n");
        return 0;
    }
    
    int r = a % b;
    if (r < 0) {
        r += (b > 0) ? b : -b;
    }
    return r;
}

CLIBAPI char* clib_cli_shift_args(int *argc, char ***argv) {
    assert(*argc > 0);
    char *result = **argv;
    *argc -= 1;
    *argv += 1;
    return result;
}

CLIBAPI Cstr clib_ansi_color(int color, int bg) {
    if (color < 0 || color > 255) return "";

    char* where_code = ITOA(bg + 3);
    char* color_string = ITOA(color);

    char* color_str = clib_str_format("\e[%s8;5;%sm", where_code, color_string);
    free(where_code);
    free(color_string);
    return color_str;
}

CLIBAPI void clib_ansi_clear_screen() {
#ifdef _WIN32
    system("cls"); // Clear screen for Windows
#else
    system("clear"); // Clear screen for Linux and macOS
#endif
}


CLIBAPI void clib_ansi_print_color_table(){
    for(int i = 0; i < 256; i++){
        if(i % 21 == 0) printf("\n");
        
        printf("%s%3d ", clib_ansi_color(i, 0), i);
    }
    printf("%s\n", ANSI_RESET);
}


CLIBAPI void clib_file_copy(const char *source, const char *destination) {
    FILE *srcFile = fopen(source, "r");
    if (srcFile == NULL) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    FILE *destFile = fopen(destination, "w");
    if (destFile == NULL) {
        perror("Error opening destination file");
        fclose(srcFile);
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), srcFile)) > 0) {
        if (fwrite(buffer, 1, bytesRead, destFile) != bytesRead) {
            perror("Error writing to destination file");
            fclose(srcFile);
            fclose(destFile);
            exit(EXIT_FAILURE);
        }
    }

    fclose(srcFile);
    fclose(destFile);
}

CLIBAPI void clib_file_move(const char *source, const char *destination) {
    if (rename(source, destination) != 0) {
        perror("Error moving/renaming file");
        exit(EXIT_FAILURE);
    }
}

CLIBAPI long clib_file_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

CLIBAPI int clib_file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

CLIBAPI void clib_file_append(const char *filename, const char *data) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file for appending");
        exit(EXIT_FAILURE);
    }
    if (fputs(data, file) == EOF) {
        perror("Error appending to file");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0755) // 0755 is the permission mode
#endif

CLIBAPI int clib_file_create_directory(const char *path)
{
    if (MKDIR(path) == 0) {
        return 1;
    } else {
        if (errno == EEXIST) {
            printf("Directory already exists: %s\n", path);
        } else {
            perror("Error creating directory");
        }
        return 0; 
    }
}

CLIBAPI int clib_file_directory_exists(const char *path)
{
    struct stat statbuf;

    if (stat(path, &statbuf) != 0) {
        return 0;
    }

    return S_ISDIR(statbuf.st_mode);
}

CLIBAPI void clib_file_create(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

CLIBAPI void clib_file_write(const char *filename, const char *data, Cstr mode) {
    if(
        strcmp(mode, "w") &&
        strcmp(mode, "w+") &&
        strcmp(mode, "wb") &&
        strcmp(mode, "a") &&
        strcmp(mode, "ab") &&
        strcmp(mode, "a+")
    ) {
        PANIC("Writing file using invalid mode: %s", mode);
    }

    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }
    if (fputs(data, file) == EOF) {
        perror("Error writing to file");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

CLIBAPI char* clib_file_read(const char *filename, const char* mode) {
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        perror("Error opening file for reading");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(buffer, 1, file_size, file);
    if (bytesRead != file_size) {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[file_size] = '\0';

    fclose(file);
    return buffer;
}

CLIBAPI void clib_file_delete(const char *filename) {
    if (remove(filename) != 0) {
        perror("Error deleting file");
        exit(EXIT_FAILURE);
    }
}

CLIBAPI void* clib_safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

CLIBAPI void* clib_safe_calloc(size_t nmemb, size_t size) {
    void *ptr = calloc(nmemb, size);
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

CLIBAPI void* clib_safe_realloc(void *ptr, size_t size) {
    if (size < 1) {
        fprintf(stderr, "Invalid size");
        return NULL;
    }

    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        fprintf(stderr, "Memory reallocation error\n");
        return NULL;
    }
    return new_ptr;
}

CLIBAPI void clib_safe_free(void **ptr) {
    if (ptr != NULL && *ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
    }
}

#ifndef _WIN32
CLIBAPI char* clib_system_execute_command(const char* command) {
    char buffer[128];
    char *result = NULL;
    size_t result_size = 0;
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        return NULL;
    }

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        size_t buffer_len = strlen(buffer);
        result = (char*) realloc(result, result_size + buffer_len + 1);
        if (!result) {
            pclose(pipe);
            return NULL;
        }
        strcpy(result + result_size, buffer);
        result_size += buffer_len;
    }

    pclose(pipe);
    return result;
}

CLIBAPI char* clib_system_get_env(const char* varname) {
    return getenv(varname);
}

CLIBAPI int set_envclib_(const char* varname, const char* value, int overwrite) {
    return setenv(varname, value, overwrite);
}

CLIBAPI int clib_system_unset_env(const char* varname) {
    return unsetenv(varname);
}
#endif

#endif // CLIB_IMPLEMENTATION
// END [IMPLEMENTATIONS] END//


#endif // CLIB_H


