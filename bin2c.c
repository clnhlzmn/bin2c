#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

static void help(void) {
    printf("an error occurred\r\n");
    exit(-1);
}

int main(int argc, char** argv) {
    uint64_t max_output_count_per_file = UINT64_MAX;
    bool max_output_count_specified = false;
    char *output_extension = NULL;
    int o;
    while ((o = getopt(argc, argv, "s:e:")) != -1) {
        switch (o) {
        case 's': {
            int n = sscanf(optarg, "%llx", &max_output_count_per_file);
            if (n != 1) {
                help();
            }
            max_output_count_specified = true;
            break;
        }
        case 'e':
            output_extension = optarg;
            break;
        default: 
            help();
            break;
        }
    }
    printf("%"PRIx64"\r\n", max_output_count_per_file);
    printf("%d %d\r\n", optind, argc);
    printf("%s %s\r\n", argv[optind], argv[optind + 1]);
    printf("%s\r\n", output_extension ? output_extension : "NULL");
    if (argc - optind != 2) {
        help();
    }
    
    /*open input file*/
    char* input_file_name = argv[optind];
    FILE* input_file = fopen(input_file_name, "rb");
    if (!input_file) {
        printf("unable to open input file %s\r\n", input_file_name);
        exit(-1);
    }
    
    /*get file size*/
    fseek(input_file, 0L, SEEK_END);
    long input_file_size = ftell(input_file);
    fseek(input_file, 0L, SEEK_SET);
    
    /*allocate buffer for input file*/
    uint8_t *input = malloc(input_file_size);
    if (!input) {
        fclose(input_file);
        printf("out of memory\r\n");
        exit(-1);
    }
    
    /*read file*/
    int err = fread(input, 1, input_file_size, input_file);
    if (err != input_file_size) {
        fclose(input_file);
        printf("unable to read input file %s\r\n", input_file_name);
        exit(-1);
    }
    
    long output_file_count = 1;
    if (max_output_count_specified) {
        output_file_count = input_file_size / max_output_count_per_file;
    }
    
    
    
    exit(1);
    char *output_file_name = argv[optind + 1];
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