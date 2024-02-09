#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME "CtoCPUSimulator"
#define VERSION "Beta 1.0.0"
#define AUTHOR "Fardin Kamal"

#define MAX_LINE_LENGTH 1024

#define INPUT_FILE "io/input.c"
#define ASSEMBLY_FILE "io/assembly.s"
#define BINARY_FILE "io/binary.o"

/**
 * Print the binary representation of a byte
 * @param byte The byte to print
*/
void print_binary(unsigned char byte)
{
    for(int i = 7; i >= 0; --i)
    {
        putchar((byte & (1 << i)) ? '1' : '0');
    }
    putchar(' ');
}

/**
 * View the contents of the binary file in human readable format in hexadecimal
*/
void view_binary_file()
{
    FILE *file = fopen(BINARY_FILE, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open binary file\n");
        exit(EXIT_FAILURE);
    }

    printf("\nBinary code:\n");
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *buffer = malloc(file_size);
    if (buffer == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, file_size, file);
    fclose(file);

    for (int i = 0; i < file_size; i++)
    {
        print_binary(buffer[i]);
    }
    printf("\n");

    free(buffer);
}

/**
 * View the contents of the preprocessed file
*/
void view_preprocessed_file()
{
    FILE *file = fopen(ASSEMBLY_FILE, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open assembly file\n");
        exit(EXIT_FAILURE);
    }

    printf("\nPreprocessed code:\n");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }

    fclose(file);
    printf("\n\n");
}

/**
 * View the contents of the assembly file
*/
void view_assembly_file()
{
    FILE *file = fopen(ASSEMBLY_FILE, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open assembly file\n");
        exit(EXIT_FAILURE);
    }

    printf("\nAssembly code:\n");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }

    fclose(file);
    printf("\n\n");
}
/**
 * Generate the binary machine code from the assembly code
 * The assembler (e.g., GNU as) translates the assembly code into binary machine code
*/
void *generate_binary_code()
{
    printf("\nGenerating binary code...");
    // Write the assembly code to a temporary file
    FILE *assembly_file = fopen(ASSEMBLY_FILE, "r");
    fclose(assembly_file);

    // Prepare the command to invoke the assembler (e.g., GNU as)
    char command[MAX_LINE_LENGTH];
    snprintf(command, sizeof(command), "as -o %s %s", BINARY_FILE, ASSEMBLY_FILE); // Use appropriate assembler command

    // Execute the assembler and check for errors
    int result = system(command);
    if (result != 0)
    {
        fprintf(stderr, "Error: Assembler failed to assemble the code\n");
        exit(EXIT_FAILURE);
    }

    // Read the binary machine code from the output file
    FILE *binary_file = fopen(BINARY_FILE, "rb");
    if (binary_file == NULL)
    {
        fprintf(stderr, "Error: Unable to open binary file\n");
        exit(EXIT_FAILURE);
    }
    fseek(binary_file, 0, SEEK_END);
    long binary_size = ftell(binary_file);
    rewind(binary_file);
    char *binary_code = malloc(binary_size);
    if (binary_code == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(binary_file);
        exit(EXIT_FAILURE);
    }
    fread(binary_code, 1, binary_size, binary_file);
    fclose(binary_file);

    free(binary_code);
    printf("\nBinary code generated!\n");
}

/**
 * Preprocess the input C code and write the preprocessed code to the output file
 * Before compilation, the C preprocessor handles directives like #include and #define
 * @param input_file The input file
 * @param output_file The output file
*/
void preprocess(const char *input_file_path, const char *output_file_path)
{
    FILE *input_file = fopen(input_file_path, "r");
    if (input_file == NULL)
    {
        fprintf(stderr, "Error: Unable to open input file\n");
        exit(EXIT_FAILURE);
    }
    FILE *output_file = fopen(output_file_path, "w");
    if (output_file == NULL)
    {
        fprintf(stderr, "Error: Unable to create output file\n");
        fclose(input_file);
        exit(EXIT_FAILURE);
    }
    char line[MAX_LINE_LENGTH];

    printf("\nPreprocessing...\n");
    while (fgets(line, sizeof(line), input_file) != NULL)
    {
        // Check if the line starts with #include or #define
        if (strncmp(line, "#include", strlen("#include")) == 0)
        {
            // Handle #include directive (read and output contents of included file)
            char filename[MAX_LINE_LENGTH];
            int scanned = sscanf(line, "#include \"%[^\"]\"", filename);
            if (scanned == 1)
            {
                FILE *included_file = fopen(filename, "r");
                if (included_file != NULL)
                {
                    preprocess(filename, output_file_path);
                    fclose(included_file);
                }
                else
                {
                    fprintf(stderr, "Error: Unable to open included file: %s\n", filename);
                }
            }
        }
        else if (strncmp(line, "#define", strlen("#define")) == 0)
        {
            // Handle #define directive (store macro and replacement text)
            // (You may need to implement a data structure to store macros)
        }
        else
        {
            // Output the line as-is (after processing any macro replacements)
            fprintf(output_file, "%s", line);
        }
    }
    printf("\nPreprocessing done!\n");
    fclose(input_file);
    fclose(output_file);
}

/**
 * Compile the input C code and return the generated assembly code
 * The C compiler (e.g., GCC) translates the preprocessed C code into assembly code
 * @param input_filename The name of the input file
*/
void *generate_assembly(const char *input_filename)
{
    printf("\nGenerating assembly code...");

    FILE *output_file = fopen(ASSEMBLY_FILE, "w");
    if (output_file == NULL)
    {
        fprintf(stderr, "Error: Unable to create temporary file\n");
        exit(EXIT_FAILURE);
    }

    // Rewind the output file to the beginning
    rewind(output_file);

    // Prepare the command to invoke GCC
    char command[MAX_LINE_LENGTH];
    snprintf(command, sizeof(command), "gcc -S -o %s %s", ASSEMBLY_FILE, input_filename);

    // Execute GCC and capture its output
    system(command);

    // Open the file containing the generated assembly code
    FILE *assembly_file = fopen(ASSEMBLY_FILE, "r");
    if (assembly_file != NULL)
    {
        // Read the assembly code and store it in a dynamically allocated buffer
        char *assembly_code = NULL;
        char line[MAX_LINE_LENGTH];
        size_t assembly_size = 0;
        while (fgets(line, sizeof(line), assembly_file) != NULL)
        {
            size_t line_length = strlen(line);
            // Allocate memory for the assembly code buffer and concatenate the current line
            assembly_code = realloc(assembly_code, assembly_size + line_length + 1);
            if (assembly_code == NULL)
            {
                fprintf(stderr, "Error: Memory allocation failed\n");
                fclose(assembly_file);
                fclose(output_file);
                exit(EXIT_FAILURE);
            }
            memcpy(assembly_code + assembly_size, line, line_length);
            assembly_size += line_length;
        }
        // Null-terminate the assembly code string
        assembly_code[assembly_size] = '\0';

        // Close the assembly file and the temporary file
        fclose(assembly_file);
        fclose(output_file);

        free(assembly_code);
        printf("\nAssembly code generated!\n");
    }
    else
    {
        fprintf(stderr, "Error: Unable to open assembly file\n");
        fclose(output_file);
        exit(EXIT_FAILURE);
    }
}

int main()
{
    printf("%s %s\n", NAME, VERSION);

    preprocess(INPUT_FILE, ASSEMBLY_FILE);
    view_preprocessed_file();

    generate_assembly(INPUT_FILE);
    view_assembly_file();

    generate_binary_code();
    view_binary_file();

    return EXIT_SUCCESS;
}
