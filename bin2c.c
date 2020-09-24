#include <stdio.h>
#include <assert.h>

int main(int argc, char** argv) {
    assert(argc == 3);
    char* input_file_name = argv[1];
    char *output_file_name = argv[2];
    FILE* input_file = fopen(input_file_name, "rb");
    FILE *output_file = fopen(output_file_name, "w+");
    unsigned long n = 0;
    while(!feof(input_file)) {
        unsigned char c;
        if(fread(&c, 1, 1, input_file) == 0) break;
        fprintf(output_file, "0x%.2X,", (int)c);
        ++n;
        if(n % 0x10 == 0) fprintf(output_file, "\n");
    }
    fclose(input_file);
    fclose(output_file);
}