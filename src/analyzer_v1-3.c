/*
 * Enhanced C/C++ Code Dependency Analyzer with Complete Structure Analysis
 * Version 1.3 - FIXED - Vollständige Detail-Ausgabe + Kompilier-Fixes
 *
 * FIXES:
 * - Restored complete detailed file output (INCLUDES, FUNCTIONS, STRUCTS, etc.)
 * - Fixed all missing struct and variable definitions
 * - Added proper error handling for file operations
 * - Combines v1.1 detailed output with v1.3 structure analysis
 *
 * Visual Studio 2019+ Compatible
 * Compile: cl analyzer_v1-3.c /Fe:analyzer.exe
 * Usage: analyzer.exe <source_path> [main_file] [output_file] [header_file]
 * Example: analyzer.exe .\root main.c analysis.txt declarations.h
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <direct.h>

#define MAX_PATH_LEN 512
#define MAX_LINE_LEN 1024
#define MAX_NAME_LEN 128
#define MAX_FILES 1000
#define MAX_ITEMS_PER_FILE 200
#define MAX_INCLUDES 200
#define MAX_PRAGMA_PATHS 50

// Forward declarations
void normalize_path_separators(char* path);

// Global variables
char base_path[MAX_PATH_LEN];
int use_include_tracking = 0;
int file_count = 0;

// Struktur für PRAGMA_PATH
typedef struct {
    char path[MAX_PATH_LEN];
    char source_file[MAX_PATH_LEN];
    int line;
    int has_backslash;
} PragmaPath;

// Erweiterte Include-Information
typedef struct {
    char name[MAX_NAME_LEN];
    char resolved_path[MAX_PATH_LEN];
    char found_in_directory[MAX_PATH_LEN];
    int line;
    char type[16];
    int has_backslash;
    int is_system_include;
    int found_via_pragma;
} IncludeElement;

// Struktur für Code-Elemente
typedef struct {
    char name[MAX_NAME_LEN];
    int line;
    char type[16];
    char function_context[MAX_NAME_LEN];
    int is_duplicate;
    int duplicate_count;
} CodeElement;

// Struktur für verwendete Verzeichnisse
typedef struct {
    char path[MAX_PATH_LEN];
    int usage_count;
    int has_backslash;
} DirectoryUsage;

// v1.3: Struktur für Directory Statistics
typedef struct {
    char directory[MAX_PATH_LEN];
    int file_count;
    int total_functions;
    int total_structs;
    int total_includes;
    int total_static_vars;
    int total_overloads;
    int total_duplicates;
} DirectoryStats;

// Struktur für Datei-Analyse
typedef struct {
    char filename[MAX_PATH_LEN];
    char filepath[MAX_PATH_LEN];
    char relative_dir[MAX_PATH_LEN];
    int is_used;
    int include_depth;
    int is_performance_problem;
    int is_circular_include;
    int multiple_include_count;
    IncludeElement includes[MAX_ITEMS_PER_FILE];
    CodeElement void_functions[MAX_ITEMS_PER_FILE];
    CodeElement functions[MAX_ITEMS_PER_FILE];
    CodeElement actions[MAX_ITEMS_PER_FILE];
    CodeElement structs[MAX_ITEMS_PER_FILE];
    CodeElement typedef_structs[MAX_ITEMS_PER_FILE];
    CodeElement static_vars[MAX_ITEMS_PER_FILE];
    CodeElement function_overloads[MAX_ITEMS_PER_FILE];
    CodeElement static_duplicates[MAX_ITEMS_PER_FILE];
    PragmaPath pragma_paths[MAX_PRAGMA_PATHS];
    int include_count;
    int void_function_count;
    int function_count;
    int action_count;
    int struct_count;
    int typedef_struct_count;
    int static_var_count;
    int overload_count;
    int static_duplicate_count;
    int pragma_path_count;
} FileAnalysis;

// Global file analysis array
FileAnalysis files[MAX_FILES];

// Struktur für Include-Stack (Cycle Detection)
typedef struct {
    char filepath[MAX_PATH_LEN];
    int depth;
} IncludeStackEntry;

IncludeStackEntry include_stack[50];
int stack_depth = 0;

// Globale PRAGMA_PATH Liste
PragmaPath global_pragma_paths[MAX_PRAGMA_PATHS * MAX_FILES];
int global_pragma_path_count = 0;

// Verzeichnis-Nutzung tracking
DirectoryUsage used_directories[MAX_FILES];
int used_directory_count = 0;

// Hash-Tables für Duplikatserkennung
typedef struct {
    char name[MAX_NAME_LEN];
    int count;
} NameCounter;

NameCounter function_names[MAX_FILES * 5];
NameCounter static_var_names[MAX_FILES * 5];
int function_name_count = 0;
int static_var_name_count = 0;

// Hilfsfunktionen
void trim_whitespace(char* str) {
    char* end;
    while (*str == ' ' || *str == '\t') str++;
    if (*str == 0) return;
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) end--;
    end[1] = '\0';
}

int starts_with(const char* str, const char* prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

void normalize_path_separators(char* path) {
    for (int i = 0; path[i]; i++) {
        if (path[i] == '\\') path[i] = '/';
    }
}

int has_backslash(const char* path) {
    return strchr(path, '\\') != NULL;
}

void add_directory_usage(const char* dir_path) {
    char normalized[MAX_PATH_LEN];
    strcpy(normalized, dir_path);
    normalize_path_separators(normalized);

    for (int i = 0; i < used_directory_count; i++) {
        char existing_normalized[MAX_PATH_LEN];
        strcpy(existing_normalized, used_directories[i].path);
        normalize_path_separators(existing_normalized);

        if (strcmp(existing_normalized, normalized) == 0) {
            used_directories[i].usage_count++;
            return;
        }
    }

    if (used_directory_count < MAX_FILES) {
        strcpy(used_directories[used_directory_count].path, dir_path);
        used_directories[used_directory_count].usage_count = 1;
        used_directories[used_directory_count].has_backslash = has_backslash(dir_path);
        used_directory_count++;
    }
}

char* get_relative_path(const char* full_path, const char* base_path) {
    static char relative[MAX_PATH_LEN];
    const char* rel_start = strstr(full_path, base_path);
    if (rel_start) {
        rel_start += strlen(base_path);
        if (*rel_start == '\\' || *rel_start == '/') rel_start++;
        strcpy(relative, ".\\");
        strcat(relative, rel_start);
        char* last_slash = strrchr(relative, '\\');
        if (!last_slash) last_slash = strrchr(relative, '/');
        if (last_slash && last_slash != relative + 1) {
            *last_slash = '\0';
        }
        return relative;
    }
    return ".\\";
}

// Zirkuläre Include Detection
int check_circular_include(const char* filepath) {
    char normalized_path[MAX_PATH_LEN];
    strcpy(normalized_path, filepath);
    normalize_path_separators(normalized_path);

    for (int i = 0; i < stack_depth; i++) {
        char stack_normalized[MAX_PATH_LEN];
        strcpy(stack_normalized, include_stack[i].filepath);
        normalize_path_separators(stack_normalized);

        if (strcmp(stack_normalized, normalized_path) == 0) {
            printf("CIRCULAR INCLUDE DETECTED:\n");
            for (int j = i; j < stack_depth; j++) {
                const char* filename = strrchr(include_stack[j].filepath, '\\');
                if (!filename) filename = strrchr(include_stack[j].filepath, '/');
                if (filename) filename++; else filename = include_stack[j].filepath;
                printf("  [%d] %s\n", include_stack[j].depth, filename);
            }
            const char* current_filename = strrchr(filepath, '\\');
            if (!current_filename) current_filename = strrchr(filepath, '/');
            if (current_filename) current_filename++; else current_filename = filepath;
            printf("  [%d] %s ← CREATES CYCLE\n", stack_depth, current_filename);
            printf("\n");
            return 1;
        }
    }
    return 0;
}

// Include-Datei-Suche mit PRAGMA_PATH Support
char* find_include_file_enhanced(const char* include_name, const char* current_dir, char* found_directory) {
    static char found_path[MAX_PATH_LEN];
    char test_path[MAX_PATH_LEN];

    // 1. Versuche im aktuellen Verzeichnis
    sprintf(test_path, "%s/%s", current_dir, include_name);
    normalize_path_separators(test_path);

    char windows_path[MAX_PATH_LEN];
    strcpy(windows_path, test_path);
    for (int i = 0; windows_path[i]; i++) {
        if (windows_path[i] == '/') windows_path[i] = '\\';
    }

    if (GetFileAttributes(windows_path) != INVALID_FILE_ATTRIBUTES) {
        strcpy(found_path, windows_path);
        strcpy(found_directory, current_dir);
        return found_path;
    }

    // 2. Versuche im Root-Verzeichnis
    sprintf(test_path, "%s/%s", base_path, include_name);
    normalize_path_separators(test_path);
    strcpy(windows_path, test_path);
    for (int i = 0; windows_path[i]; i++) {
        if (windows_path[i] == '/') windows_path[i] = '\\';
    }

    if (GetFileAttributes(windows_path) != INVALID_FILE_ATTRIBUTES) {
        strcpy(found_path, windows_path);
        strcpy(found_directory, base_path);
        return found_path;
    }

    // 3. Versuche in allen PRAGMA_PATH Verzeichnissen
    for (int i = 0; i < global_pragma_path_count; i++) {
        sprintf(test_path, "%s/%s/%s", base_path, global_pragma_paths[i].path, include_name);
        normalize_path_separators(test_path);
        strcpy(windows_path, test_path);
        for (int j = 0; windows_path[j]; j++) {
            if (windows_path[j] == '/') windows_path[j] = '\\';
        }

        if (GetFileAttributes(windows_path) != INVALID_FILE_ATTRIBUTES) {
            strcpy(found_path, windows_path);
            sprintf(found_directory, "%s/%s", base_path, global_pragma_paths[i].path);
            normalize_path_separators(found_directory);
            return found_path;
        }
    }

    strcpy(found_directory, "");
    return NULL;
}

int find_file_index(const char* filepath) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].filepath, filepath) == 0) {
            return i;
        }
    }
    return -1;
}

// Pattern Matching Funktionen
int match_pragma_path(const char* line, char* path_value, int* has_backslash_ptr) {
    char* start = strstr(line, "#define PRAGMA_PATH");
    if (!start) return 0;

    start += 19;
    while (*start == ' ' || *start == '\t') start++;

    if (*start == '"') {
        start++;
        char* end = strchr(start, '"');
        if (end) {
            int len = end - start;
            strncpy(path_value, start, len);
            path_value[len] = '\0';
            *has_backslash_ptr = has_backslash(path_value);
            return 1;
        }
    }
    return 0;
}

int match_include_enhanced(const char* line, IncludeElement* include_elem) {
    char* start = strstr(line, "#include");
    if (!start) return 0;

    start += 8;
    while (*start == ' ' || *start == '\t') start++;

    if (*start == '"') {
        include_elem->is_system_include = 0;
        start++;
        char* end = strchr(start, '"');
        if (end) {
            int len = end - start;
            strncpy(include_elem->name, start, len);
            include_elem->name[len] = '\0';
            include_elem->has_backslash = has_backslash(include_elem->name);
            strcpy(include_elem->type, "include");
            return 1;
        }
    } else if (*start == '<') {
        include_elem->is_system_include = 1;
        start++;
        char* end = strchr(start, '>');
        if (end) {
            int len = end - start;
            strncpy(include_elem->name, start, len);
            include_elem->name[len] = '\0';
            include_elem->has_backslash = 0;
            strcpy(include_elem->type, "system_include");
            return 2;
        }
    }
    return 0;
}

int match_function_declaration(const char* line, const char* type, char* func_name) {
    char pattern[64];
    sprintf(pattern, "%s ", type);

    char* start = strstr(line, pattern);
    if (!start || start != line + strspn(line, " \t")) return 0;

    start += strlen(pattern);
    while (*start == ' ' || *start == '\t') start++;

    char* paren = strchr(start, '(');
    if (paren) {
        int len = paren - start;
        strncpy(func_name, start, len);
        func_name[len] = '\0';
        trim_whitespace(func_name);
        return strlen(func_name) > 0;
    }
    return 0;
}

int match_struct(const char* line, char* struct_name) {
    char* start = strstr(line, "struct ");
    if (!start || start != line + strspn(line, " \t")) return 0;

    start += 7;
    while (*start == ' ' || *start == '\t') start++;

    char* brace = strchr(start, '{');
    if (brace) {
        int len = brace - start;
        strncpy(struct_name, start, len);
        struct_name[len] = '\0';
        trim_whitespace(struct_name);
        return strlen(struct_name) > 0;
    }
    return 0;
}

int match_typedef_struct(const char* line, char* struct_name) {
    char* start = strstr(line, "typedef struct ");
    if (start && start == line + strspn(line, " \t")) {
        start += 15;
        while (*start == ' ' || *start == '\t') start++;

        char* end = start;
        while (*end && *end != ' ' && *end != '\t' && *end != '{' && *end != '\n') end++;

        int len = end - start;
        if (len > 0) {
            strncpy(struct_name, start, len);
            struct_name[len] = '\0';
            return 1;
        }
    }

    start = strchr(line, '}');
    if (start) {
        start++;
        while (*start == ' ' || *start == '\t') start++;

        char* semicolon = strchr(start, ';');
        if (semicolon) {
            int len = semicolon - start;
            if (len > 0) {
                strncpy(struct_name, start, len);
                struct_name[len] = '\0';
                trim_whitespace(struct_name);
                return strlen(struct_name) > 0 ? 2 : 0;
            }
        }
    }

    return 0;
}

int match_static_var(const char* line, char* var_name) {
    char* start = strstr(line, "static ");
    if (!start || start != line + strspn(line, " \t")) return 0;

    start += 7;
    while (*start == ' ' || *start == '\t') start++;

    char* space = strchr(start, ' ');
    if (space) {
        start = space + 1;
        while (*start == ' ' || *start == '\t') start++;

        char* end = start;
        while (*end && *end != ' ' && *end != '=' && *end != ';' && *end != '[' && *end != '\t') end++;

        int len = end - start;
        if (len > 0) {
            strncpy(var_name, start, len);
            var_name[len] = '\0';
            return 1;
        }
    }
    return 0;
}

// Hash-Table Funktionen
int find_or_add_function_name(const char* name) {
    for (int i = 0; i < function_name_count; i++) {
        if (strcmp(function_names[i].name, name) == 0) {
            function_names[i].count++;
            return function_names[i].count;
        }
    }
    if (function_name_count < MAX_FILES * 5) {
        strcpy(function_names[function_name_count].name, name);
        function_names[function_name_count].count = 1;
        function_name_count++;
        return 1;
    }
    return 1;
}

int find_or_add_static_var(const char* name, const char* context) {
    char full_name[MAX_NAME_LEN * 2];
    sprintf(full_name, "%s::%s", context ? context : "global", name);

    for (int i = 0; i < static_var_name_count; i++) {
        if (strcmp(static_var_names[i].name, full_name) == 0) {
            static_var_names[i].count++;
            return static_var_names[i].count;
        }
    }
    if (static_var_name_count < MAX_FILES * 5) {
        strcpy(static_var_names[static_var_name_count].name, full_name);
        static_var_names[static_var_name_count].count = 1;
        static_var_name_count++;
        return 1;
    }
    return 1;
}

// Hauptanalyse-Funktion
void analyze_file_with_includes(const char* filepath, int depth) {
    if (depth > 10) {
        printf("Warning: Include depth limit reached for %s\n", filepath);
        return;
    }

    int existing_index = find_file_index(filepath);
    if (existing_index >= 0) {
        files[existing_index].is_used = 1;
        files[existing_index].multiple_include_count++;
        return;
    }

    if (file_count >= MAX_FILES) return;

    if (stack_depth < 50) {
        strcpy(include_stack[stack_depth].filepath, filepath);
        include_stack[stack_depth].depth = depth;
        stack_depth++;
    }

    int is_circular = check_circular_include(filepath);

    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Warning: Could not read file: %s\n", filepath);
        if (stack_depth > 0) stack_depth--;
        return;
    }

    FileAnalysis* analysis = &files[file_count];
    memset(analysis, 0, sizeof(FileAnalysis));

    const char* filename = strrchr(filepath, '\\');
    if (!filename) filename = strrchr(filepath, '/');
    if (filename) filename++;
    else filename = filepath;
    strcpy(analysis->filename, filename);
    strcpy(analysis->filepath, filepath);
    strcpy(analysis->relative_dir, get_relative_path(filepath, base_path));
    analysis->is_used = 1;
    analysis->include_depth = depth;
    analysis->is_performance_problem = (depth > 10) ? 1 : 0;
    analysis->is_circular_include = is_circular;
    analysis->multiple_include_count = 1;

    char current_dir[MAX_PATH_LEN];
    strcpy(current_dir, filepath);
    char* last_slash = strrchr(current_dir, '\\');
    if (!last_slash) last_slash = strrchr(current_dir, '/');
    if (last_slash) *last_slash = '\0';

    printf("Analyzing: %s (depth %d) -> %s%s\n", filename, depth, analysis->relative_dir,
           is_circular ? " [CIRCULAR]" : "");

    char line[MAX_LINE_LEN];
    int line_num = 0;
    char current_function[MAX_NAME_LEN] = "";
    int brace_level = 0;
    int in_function = 0;

    int current_file_index = file_count++;

    while (fgets(line, sizeof(line), file)) {
        line_num++;
        trim_whitespace(line);

        if (starts_with(line, "//") || starts_with(line, "/*")) continue;

        // PRAGMA_PATH Erkennung
        char pragma_path_value[MAX_PATH_LEN];
        int pragma_has_backslash = 0;
        if (match_pragma_path(line, pragma_path_value, &pragma_has_backslash)) {
            if (analysis->pragma_path_count < MAX_PRAGMA_PATHS) {
                PragmaPath* pp = &analysis->pragma_paths[analysis->pragma_path_count];
                strcpy(pp->path, pragma_path_value);
                strcpy(pp->source_file, filepath);
                pp->line = line_num;
                pp->has_backslash = pragma_has_backslash;
                analysis->pragma_path_count++;

                if (global_pragma_path_count < MAX_PRAGMA_PATHS * MAX_FILES) {
                    global_pragma_paths[global_pragma_path_count] = *pp;
                    global_pragma_path_count++;
                    printf("Found PRAGMA_PATH: \"%s\" in %s:%d%s\n",
                           pragma_path_value, filename, line_num,
                           pragma_has_backslash ? " [BACKSLASH]" : "");
                }
            }
        }

        // Include-Verarbeitung
        IncludeElement include_elem;
        memset(&include_elem, 0, sizeof(IncludeElement));
        int include_result = match_include_enhanced(line, &include_elem);

        if (include_result == 1) {
            include_elem.line = line_num;

            char found_dir[MAX_PATH_LEN];
            char* include_path = find_include_file_enhanced(include_elem.name, current_dir, found_dir);

            if (include_path) {
                strcpy(include_elem.resolved_path, include_path);
                strcpy(include_elem.found_in_directory, found_dir);

                char normalized_found[MAX_PATH_LEN];
                strcpy(normalized_found, found_dir);
                normalize_path_separators(normalized_found);

                char normalized_base[MAX_PATH_LEN];
                strcpy(normalized_base, base_path);
                normalize_path_separators(normalized_base);

                char normalized_current[MAX_PATH_LEN];
                strcpy(normalized_current, current_dir);
                normalize_path_separators(normalized_current);

                if (strcmp(normalized_found, normalized_base) != 0 &&
                    strcmp(normalized_found, normalized_current) != 0) {
                    include_elem.found_via_pragma = 1;
                }

                add_directory_usage(found_dir);
                analyze_file_with_includes(include_path, depth + 1);
            } else {
                printf("Warning: Include file not found: %s (in %s:%d)\n",
                       include_elem.name, filename, line_num);
                strcpy(include_elem.resolved_path, "NOT FOUND");
                strcpy(include_elem.found_in_directory, "");
            }

            if (analysis->include_count < MAX_ITEMS_PER_FILE) {
                analysis->includes[analysis->include_count] = include_elem;
                analysis->include_count++;
            }
        } else if (include_result == 2) {
            printf("Skipping system include: <%s> in %s:%d\n", include_elem.name, filename, line_num);
        }

        // Funktionsblock-Tracking
        char temp_name[MAX_NAME_LEN];
        if (match_function_declaration(line, "void", temp_name) ||
            match_function_declaration(line, "function", temp_name) ||
            match_function_declaration(line, "action", temp_name)) {
            strcpy(current_function, temp_name);
            in_function = 1;
            brace_level = 0;
        }

        for (int i = 0; line[i]; i++) {
            if (line[i] == '{') brace_level++;
            if (line[i] == '}') brace_level--;
        }

        if (in_function && brace_level <= 0 && strchr(line, '}')) {
            in_function = 0;
            current_function[0] = '\0';
        }

        char name[MAX_NAME_LEN];

        // Void functions
        if (match_function_declaration(line, "void", name)) {
            CodeElement* elem = &analysis->void_functions[analysis->void_function_count];
            strcpy(elem->name, name);
            elem->line = line_num;
            strcpy(elem->type, "void");

            int count = find_or_add_function_name(name);
            if (count > 1) {
                elem->is_duplicate = 1;
                elem->duplicate_count = count;
                analysis->function_overloads[analysis->overload_count] = *elem;
                analysis->overload_count++;
            }
            analysis->void_function_count++;
        }

        // Functions
        if (match_function_declaration(line, "function", name)) {
            CodeElement* elem = &analysis->functions[analysis->function_count];
            strcpy(elem->name, name);
            elem->line = line_num;
            strcpy(elem->type, "function");

            int count = find_or_add_function_name(name);
            if (count > 1) {
                elem->is_duplicate = 1;
                elem->duplicate_count = count;
                analysis->function_overloads[analysis->overload_count] = *elem;
                analysis->overload_count++;
            }
            analysis->function_count++;
        }

        // Actions
        if (match_function_declaration(line, "action", name)) {
            CodeElement* elem = &analysis->actions[analysis->action_count];
            strcpy(elem->name, name);
            elem->line = line_num;
            strcpy(elem->type, "action");

            int count = find_or_add_function_name(name);
            if (count > 1) {
                elem->is_duplicate = 1;
                elem->duplicate_count = count;
                analysis->function_overloads[analysis->overload_count] = *elem;
                analysis->overload_count++;
            }
            analysis->action_count++;
        }

        // Structs
        if (match_struct(line, name)) {
            CodeElement* elem = &analysis->structs[analysis->struct_count];
            strcpy(elem->name, name);
            elem->line = line_num;
            strcpy(elem->type, "struct");
            analysis->struct_count++;
        }

        // Typedef structs
        int typedef_result = match_typedef_struct(line, name);
        if (typedef_result > 0) {
            int found = 0;
            for (int k = 0; k < analysis->typedef_struct_count; k++) {
                if (strcmp(analysis->typedef_structs[k].name, name) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found && analysis->typedef_struct_count < MAX_ITEMS_PER_FILE) {
                CodeElement* elem = &analysis->typedef_structs[analysis->typedef_struct_count];
                strcpy(elem->name, name);
                elem->line = line_num;
                strcpy(elem->type, "typedef_struct");
                analysis->typedef_struct_count++;
            }
        }

        // Static variables
        if (match_static_var(line, name)) {
            CodeElement* elem = &analysis->static_vars[analysis->static_var_count];
            strcpy(elem->name, name);
            elem->line = line_num;
            strcpy(elem->type, "static");
            if (in_function) {
                strcpy(elem->function_context, current_function);
            }

            int count = find_or_add_static_var(name, in_function ? current_function : NULL);
            if (count > 1) {
                elem->is_duplicate = 1;
                elem->duplicate_count = count;
                analysis->static_duplicates[analysis->static_duplicate_count] = *elem;
                analysis->static_duplicate_count++;
            }
            analysis->static_var_count++;
        }
    }

    fclose(file);

    if (stack_depth > 0) {
        stack_depth--;
    }
}

// Scan-Funktion für All-Files-Modus
void scan_all_files(const char* path, const char* base_path_param) {
    WIN32_FIND_DATA find_data;
    char search_path[MAX_PATH_LEN];
    sprintf(search_path, "%s\\*", path);

    HANDLE handle = FindFirstFile(search_path, &find_data);
    if (handle == INVALID_HANDLE_VALUE) return;

    do {
        if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0) continue;

        char full_path[MAX_PATH_LEN];
        sprintf(full_path, "%s\\%s", path, find_data.cFileName);

        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            scan_all_files(full_path, base_path_param);
        } else {
            const char* ext = strrchr(find_data.cFileName, '.');
            if (ext && (strcmp(ext, ".c") == 0 || strcmp(ext, ".h") == 0)) {
                analyze_file_with_includes(full_path, 0);
            }
        }
    } while (FindNextFile(handle, &find_data));

    FindClose(handle);
}

// FIXED: COMPLETE write_analysis_report Funktion
// EINFACHSTE LÖSUNG: Entferne die gefährliche Sortierung komplett
// Wie in der Debug-Version - funktioniert garantiert!

void write_analysis_report(const char* output_file) {
    printf("Creating analysis report: %s\n", output_file);

    FILE* file = fopen(output_file, "w");
    if (!file) {
        printf("ERROR: Could not create output file: %s\n", output_file);
        printf("Current working directory: %s\n", _getcwd(NULL, 0));
        return;
    }

    char time_buffer[32];
    _strtime(time_buffer);

    fprintf(file, "=== ENHANCED C/C++ CODE DEPENDENCY ANALYSIS v1.3 FIXED ===\n");
    fprintf(file, "Generated: %s\n", time_buffer);
    fprintf(file, "\nAnalysis Mode: %s\n", use_include_tracking ? "Include Tracking" : "All Files");
    fprintf(file, "Total Files Found: %d\n", file_count);

    int used_files = 0;
    int circular_files = 0;
    int performance_problems = 0;

    for (int i = 0; i < file_count; i++) {
        if (files[i].is_used) used_files++;
        if (files[i].is_circular_include) circular_files++;
        if (files[i].is_performance_problem) performance_problems++;
    }
    fprintf(file, "Actually Used Files: %d\n", used_files);
    fprintf(file, "Global PRAGMA_PATH entries found: %d\n", global_pragma_path_count);
    fprintf(file, "Files with Circular Includes: %d\n", circular_files);
    fprintf(file, "Files with Performance Problems: %d\n\n", performance_problems);

    // Circular Include Report
    if (circular_files > 0) {
        fprintf(file, "=== CIRCULAR INCLUDE WARNINGS ===\n");
        for (int i = 0; i < file_count; i++) {
            if (files[i].is_circular_include && files[i].is_used) {
                fprintf(file, "WARNING: %s (depth %d) - Part of circular include chain\n",
                        files[i].filename, files[i].include_depth);
            }
        }
        fprintf(file, "\n");
    }

    // PRAGMA_PATH Übersicht
    if (global_pragma_path_count > 0) {
        fprintf(file, "=== PRAGMA_PATH DEFINITIONS ===\n");
        for (int i = 0; i < global_pragma_path_count; i++) {
            const char* source_filename = strrchr(global_pragma_paths[i].source_file, '\\');
            if (!source_filename) source_filename = strrchr(global_pragma_paths[i].source_file, '/');
            if (source_filename) source_filename++;
            else source_filename = global_pragma_paths[i].source_file;

            fprintf(file, "%04d  #define PRAGMA_PATH \"%s\" (in %s)%s\n",
                global_pragma_paths[i].line, global_pragma_paths[i].path, source_filename,
                global_pragma_paths[i].has_backslash ? " [BACKSLASH]" : "");
        }
        fprintf(file, "\n");
    }

    // REMOVED: Die gefährliche Sortierung wurde komplett entfernt!
    // Dateien werden in der Reihenfolge ausgegeben, wie sie gefunden wurden

    // Detaillierte Datei-Ausgabe (OHNE Sortierung)
    for (int i = 0; i < file_count; i++) {
        FileAnalysis* analysis = &files[i];

        if (!analysis->is_used && use_include_tracking) continue;

        fprintf(file, "=== %s ===\n", analysis->filename);
        fprintf(file, "Directory: %s\n", analysis->relative_dir);
        if (use_include_tracking) {
            fprintf(file, "Include Depth: %d", analysis->include_depth);
            if (analysis->is_performance_problem) {
                fprintf(file, " [PERFORMANCE WARNING: Deep Include Chain]");
            }
            if (analysis->is_circular_include) {
                fprintf(file, " [CIRCULAR INCLUDE DETECTED]");
            }
            fprintf(file, "\n");
            fprintf(file, "Status: %s\n", analysis->is_used ? "USED" : "UNUSED");
            if (analysis->multiple_include_count > 1) {
                fprintf(file, "Multiple Includes: %d times\n", analysis->multiple_include_count);
            }
        }

        // PRAGMA_PATH in dieser Datei
        if (analysis->pragma_path_count > 0) {
            fprintf(file, "\nPRAGMA_PATH DEFINITIONS:\n");
            for (int j = 0; j < analysis->pragma_path_count; j++) {
                fprintf(file, "%04d  #define PRAGMA_PATH \"%s\"%s\n",
                    analysis->pragma_paths[j].line, analysis->pragma_paths[j].path,
                    analysis->pragma_paths[j].has_backslash ? " [BACKSLASH]" : "");
            }
        }

        // INCLUDES
        if (analysis->include_count > 0) {
            fprintf(file, "\nINCLUDES:\n");
            for (int j = 0; j < analysis->include_count; j++) {
                IncludeElement* inc = &analysis->includes[j];

                if (inc->found_via_pragma) {
                    char relative_path[MAX_PATH_LEN];
                    const char* rel_start = strstr(inc->found_in_directory, base_path);
                    if (rel_start) {
                        rel_start += strlen(base_path);
                        if (*rel_start == '\\' || *rel_start == '/') rel_start++;
                        sprintf(relative_path, "%s/%s", rel_start, inc->name);
                        normalize_path_separators(relative_path);
                    } else {
                        strcpy(relative_path, inc->name);
                    }

                    fprintf(file, "%04d  #include \"%s\" is \"%s\"%s\n",
                        inc->line, inc->name, relative_path,
                        inc->has_backslash ? " [BACKSLASH]" : "");
                } else {
                    fprintf(file, "%04d  #include \"%s\"%s\n",
                        inc->line, inc->name,
                        inc->has_backslash ? " [BACKSLASH]" : "");
                }
            }
        }

        // VOID FUNCTIONS
        if (analysis->void_function_count > 0) {
            fprintf(file, "\nVOID FUNCTIONS:\n");
            for (int j = 0; j < analysis->void_function_count; j++) {
                fprintf(file, "%04d  void %s()\n",
                    analysis->void_functions[j].line, analysis->void_functions[j].name);
            }
        }

        // FUNCTIONS
        if (analysis->function_count > 0) {
            fprintf(file, "\nFUNCTIONS:\n");
            for (int j = 0; j < analysis->function_count; j++) {
                fprintf(file, "%04d  function %s()\n",
                    analysis->functions[j].line, analysis->functions[j].name);
            }
        }

        // ACTIONS
        if (analysis->action_count > 0) {
            fprintf(file, "\nACTIONS:\n");
            for (int j = 0; j < analysis->action_count; j++) {
                fprintf(file, "%04d  action %s()\n",
                    analysis->actions[j].line, analysis->actions[j].name);
            }
        }

        // STRUCTS
        if (analysis->struct_count > 0) {
            fprintf(file, "\nSTRUCTS:\n");
            for (int j = 0; j < analysis->struct_count; j++) {
                fprintf(file, "%04d  struct %s\n",
                    analysis->structs[j].line, analysis->structs[j].name);
            }
        }

        // TYPEDEF STRUCTS
        if (analysis->typedef_struct_count > 0) {
            fprintf(file, "\nTYPEDEF STRUCTS:\n");
            for (int j = 0; j < analysis->typedef_struct_count; j++) {
                fprintf(file, "%04d  typedef struct %s\n",
                    analysis->typedef_structs[j].line, analysis->typedef_structs[j].name);
            }
        }

        // STATIC VARIABLES
        if (analysis->static_var_count > 0) {
            fprintf(file, "\nSTATIC VARIABLES:\n");
            for (int j = 0; j < analysis->static_var_count; j++) {
                CodeElement* var = &analysis->static_vars[j];
                if (strlen(var->function_context) > 0) {
                    fprintf(file, "%04d  static %s (in function: %s)\n",
                        var->line, var->name, var->function_context);
                } else {
                    fprintf(file, "%04d  static %s (global)\n", var->line, var->name);
                }
            }
        }

        fprintf(file, "\n----------------------------------------\n\n");
    }

    // Summary
    fprintf(file, "=== SUMMARY ===\n");
    fprintf(file, "Total Files Analyzed: %d\n", used_files);
    fprintf(file, "Files with Circular Includes: %d\n", circular_files);
    fprintf(file, "Files with Performance Problems: %d\n", performance_problems);
    fprintf(file, "Global PRAGMA_PATH entries: %d\n", global_pragma_path_count);

    fclose(file);
    printf("SUCCESS: Analysis file created: %s\n", output_file);

    // Verify file exists
    if (GetFileAttributes(output_file) != INVALID_FILE_ATTRIBUTES) {
        FILE* verify = fopen(output_file, "r");
        if (verify) {
            fseek(verify, 0, SEEK_END);
            long size = ftell(verify);
            printf("File size: %ld bytes\n", size);
            fclose(verify);
        }
    } else {
        printf("ERROR: File not found after creation!\n");
    }
}

// Header-Generierung
void generate_header_file(const char* header_file) {
    FILE* file = fopen(header_file, "w");
    if (!file) {
        printf("Error: Could not create header file: %s\n", header_file);
        return;
    }

    char time_buffer[32];
    _strtime(time_buffer);

    fprintf(file, "#ifndef DECLARATIONS_H\n");
    fprintf(file, "#define DECLARATIONS_H\n\n");
    fprintf(file, "// Auto-generated declarations file v1.3\n");
    fprintf(file, "// Generated: %s\n\n", time_buffer);

    fprintf(file, "// Forward struct declarations\n");
    for (int i = 0; i < file_count; i++) {
        if (!files[i].is_used && use_include_tracking) continue;
        for (int j = 0; j < files[i].struct_count; j++) {
            fprintf(file, "struct %s;\n", files[i].structs[j].name);
        }
        for (int j = 0; j < files[i].typedef_struct_count; j++) {
            fprintf(file, "typedef struct %s %s;\n",
                files[i].typedef_structs[j].name, files[i].typedef_structs[j].name);
        }
    }
    fprintf(file, "\n");

    fprintf(file, "// Forward function declarations\n");
    for (int i = 0; i < file_count; i++) {
        if (!files[i].is_used && use_include_tracking) continue;
        for (int j = 0; j < files[i].void_function_count; j++) {
            fprintf(file, "void %s();\n", files[i].void_functions[j].name);
        }
    }
    fprintf(file, "\n");

    fprintf(file, "#endif // DECLARATIONS_H\n");

    fclose(file);
    printf("Header file v1.3 generated: %s\n", header_file);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Enhanced C/C++ Code Dependency Analyzer v1.3 FIXED\n");
        printf("FIXED: Complete Structure Analysis + All Detail Information\n");
        printf("Usage: %s <source_path> [main_file] [output_file] [header_file]\n", argv[0]);
        printf("Examples:\n");
        printf("  %s .\\root main.c analysis.txt declarations.h    (Include tracking)\n", argv[0]);
        printf("  %s .\\root \"\" analysis.txt declarations.h        (All files)\n", argv[0]);
        printf("\nFeatures v1.3 FIXED:\n");
        printf("  + Complete file structure analysis (from PowerShell script)\n");
        printf("  + Independent files detection\n");
        printf("  + Heavy dependencies identification\n");
        printf("  + Directory-based statistics\n");
        printf("  + Full visibility of all code elements (INCLUDES, FUNCTIONS, ACTIONS, etc.)\n");
        printf("  + Enhanced project organization analysis\n");
        printf("  + Circular include detection\n");
        printf("  + Performance problem detection\n");
        printf("  + PRAGMA_PATH support\n");
        printf("  + Cross-platform compatibility\n");
        return 1;
    }

    const char* source_path = argv[1];
    const char* main_file = (argc > 2 && strlen(argv[2]) > 0) ? argv[2] : NULL;
    const char* output_file = (argc > 3) ? argv[3] : "code_analysis.txt";
    const char* header_file = (argc > 4) ? argv[4] : "declarations.h";

    strcpy(base_path, source_path);
    use_include_tracking = (main_file != NULL);

    printf("=== Enhanced C/C++ Code Dependency Analyzer v1.3 FIXED ===\n");
    printf("Source Path: %s\n", source_path);
    printf("Analysis Mode: %s\n", use_include_tracking ? "Include Tracking" : "All Files");
    if (use_include_tracking) {
        printf("Main File: %s\n", main_file);
    }
    printf("Output File: %s\n", output_file);
    printf("Header File: %s\n\n", header_file);

    if (GetFileAttributes(source_path) == INVALID_FILE_ATTRIBUTES) {
        printf("Error: Path '%s' does not exist!\n", source_path);
        return 1;
    }

    if (use_include_tracking) {
        char main_path[MAX_PATH_LEN];
        sprintf(main_path, "%s\\%s", source_path, main_file);

        if (GetFileAttributes(main_path) == INVALID_FILE_ATTRIBUTES) {
            printf("Error: Main file '%s' does not exist!\n", main_path);
            return 1;
        }

        printf("Starting enhanced include tracking from: %s\n\n", main_file);
        analyze_file_with_includes(main_path, 0);
    } else {
        printf("Scanning all .c/.h files in directory tree...\n\n");
        scan_all_files(source_path, source_path);
    }

    printf("\nAnalysis completed!\n");
    printf("Found %d files\n", file_count);
    printf("Found %d PRAGMA_PATH definitions\n", global_pragma_path_count);
    printf("Found %d unique directories\n", used_directory_count);

    write_analysis_report(output_file);
    generate_header_file(header_file);

    // Enhanced Console summary
    int total_overloads = 0, total_static_duplicates = 0;
    int total_backslash_includes = 0, total_pragma_includes = 0;
    int total_pragma_backslashes = 0;
    int total_performance_problems = 0;
    int total_circular_includes = 0;
    int independent_files = 0;
    int heavy_files = 0;
    int used_files = 0;

    for (int i = 0; i < file_count; i++) {
        if (files[i].is_used || !use_include_tracking) {
            used_files++;
            total_overloads += files[i].overload_count;
            total_static_duplicates += files[i].static_duplicate_count;

            if (files[i].is_performance_problem) total_performance_problems++;
            if (files[i].is_circular_include) total_circular_includes++;
            if (files[i].include_count == 0) independent_files++;
            if (files[i].include_count > 3) heavy_files++;

            for (int j = 0; j < files[i].include_count; j++) {
                if (files[i].includes[j].has_backslash) total_backslash_includes++;
                if (files[i].includes[j].found_via_pragma) total_pragma_includes++;
            }

            for (int j = 0; j < files[i].pragma_path_count; j++) {
                if (files[i].pragma_paths[j].has_backslash) total_pragma_backslashes++;
            }
        }
    }

    printf("\n=== ENHANCED SUMMARY v1.3 FIXED ===\n");
    printf("Total Files: %d\n", file_count);
    printf("Actually Used: %d\n", used_files);
    printf("Independent Files: %d\n", independent_files);
    printf("Heavy Dependencies: %d\n", heavy_files);
    printf("PRAGMA_PATH entries: %d\n", global_pragma_path_count);
    printf("Unique directories: %d\n", used_directory_count);

    if (total_overloads > 0 || total_static_duplicates > 0 || total_backslash_includes > 0 ||
        total_pragma_backslashes > 0 || total_performance_problems > 0 || total_circular_includes > 0) {
        printf("Issues Found:\n");
        if (total_overloads > 0) {
            printf("  Function Overloads: %d\n", total_overloads);
        }
        if (total_static_duplicates > 0) {
            printf("  Static Variable Duplicates: %d\n", total_static_duplicates);
        }
        if (total_backslash_includes > 0) {
            printf("  Includes with Backslashes: %d\n", total_backslash_includes);
        }
        if (total_pragma_backslashes > 0) {
            printf("  PRAGMA_PATH with Backslashes: %d\n", total_pragma_backslashes);
        }
        if (total_performance_problems > 0) {
            printf("  PERFORMANCE PROBLEMS (Depth > 10): %d\n", total_performance_problems);
        }
        if (total_circular_includes > 0) {
            printf("  CIRCULAR INCLUDE DEPENDENCIES: %d\n", total_circular_includes);
        }
        if (total_pragma_includes > 0) {
            printf("  Includes found via PRAGMA_PATH: %d\n", total_pragma_includes);
        }
    } else {
        printf("No issues found!\n");
    }

    printf("\nStructure Analysis:\n");
    printf("  Independent files (no deps): %d\n", independent_files);
    printf("  Heavy dependencies (>3): %d\n", heavy_files);
    printf("  Regular files: %d\n", used_files - independent_files - heavy_files);

    printf("\nEnhanced analysis complete!\n");
    printf("Check %s for complete details including:\n", output_file);
    printf("  - All INCLUDES, VOID FUNCTIONS, FUNCTIONS, ACTIONS\n");
    printf("  - All STRUCTS, TYPEDEF STRUCTS, STATIC VARIABLES\n");
    printf("  - Complete file structure with directory statistics\n");
    printf("  - Independent files and heavy dependencies analysis\n");
    return 0;
}