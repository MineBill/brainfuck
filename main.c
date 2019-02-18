#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MEMORY_SIZE 30000

typedef struct parser {
    char *program;

    unsigned char mem[MEMORY_SIZE];
    unsigned char *pointer;

    char *curr;
} parser;

parser *parser_new(char *_program) {
    parser *new = (parser *)malloc(sizeof(parser));
    new->program = _program;
    memset(new->mem, 0, MEMORY_SIZE * sizeof(unsigned char));
    new->pointer = new->mem;
    new->curr = _program;
    return new;
}

void parser_parse(parser *parser) {
    switch (*parser->curr) {
        case '+':
            (*parser->pointer)++;
            break;
        case '-':
            (*parser->pointer)--;
            break;
        case '>':
            if (parser->pointer >= parser->mem + MEMORY_SIZE) return;
            parser->pointer++;
            break;
        case '<':
            if (parser->pointer <= parser->mem) return;
            parser->pointer--;
            break;

        case '.':
            putchar(*parser->pointer);
            break;

        case ',':
            *parser->pointer = getchar();
            break;

        case '[':
            if (*parser->pointer == 0) {
                while (*parser->curr != ']') {
                    parser->curr++;
                    if (*parser->curr == '[') {
                        parser_parse(parser);
                    }
                }
            }
            break;

        case ']':
            if (*parser->pointer != 0) {
                while (*parser->curr != '[') {
                    parser->curr--;
                    if (*parser->curr == ']') {
                        parser_parse(parser);
                        parser->curr -= 2;
                    }
                }
            }
            break;

        default:
            break;
    }
    parser->curr++;
}

int parser_eof(parser *parser) { return *parser->curr == '\0'; }

int get_file_size(const char *file) {
    struct stat st;
    if (stat(file, &st)) {  // Returns -1 on error, 0 on success
        printf("Failed to get file size\n");
        return -1;
    }
    return st.st_size;
}

char *load_file(FILE *file, const char *fileName) {
    int fileSize = get_file_size(fileName);
    char *buffer = (char *)malloc(sizeof(char) * (fileSize + 1));
    fread(buffer, fileSize, 1, file);
    return buffer;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("Usage: parserfuck {program file}\n");
        return -1;
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error while opening file\n");
        return -1;
    }

    char *buffer = load_file(file, argv[1]);

    parser *parser = parser_new(buffer);
    while (!parser_eof(parser)) {
        parser_parse(parser);
    }
    return 0;
}
