#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME "CtoCPUSimulator"
#define VERSION "Beta 1.2.0"
#define AUTHOR "Fardin Kamal"

#define MAX_LINE_LENGTH 1024
#define MEMORY_SIZE 1024

#define INPUT_FILE "io/input.c"
#define ASSEMBLY_FILE "io/assembly.s"
#define BINARY_FILE "io/binary.o"

typedef struct
{
    int pc; // Program Counter
    int memory[MEMORY_SIZE];
    int accumulator; // Accumulator register
} CPU;

/**
 * Load a program from a binary file into the CPU's memory
 * @param cpu The CPU
 * @param filename The name of the binary file
 */
void load_program(CPU *cpu, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }

    fread(cpu->memory, sizeof(int), MEMORY_SIZE, file);
    fclose(file);
}

/**
 * Execute the next instruction in the CPU's memory
 * @param cpu The CPU
 */
void execute_instruction(CPU *cpu)
{
    int instruction = cpu->memory[cpu->pc];
    int opcode = instruction >> 24;       // Extract opcode (first 8 bits)
    int operand = instruction & 0xFFFFFF; // Extract operand (last 24 bits)

    switch (opcode)
    {
    case 0: // Load instruction
        cpu->accumulator = operand;
        printf("Loaded %d into accumulator\n", operand);
        break;
    case 1: // Add instruction
        cpu->accumulator += operand;
        printf("Added %d to accumulator\n", operand);
        break;
    case 2: // Subtract instruction
        cpu->accumulator -= operand;
        printf("Subtracted %d from accumulator\n", operand);
        break;
    case 3: // Halt instruction
        printf("Halted execution\n");
        cpu->pc = MEMORY_SIZE; // Terminate execution
        break;
    default:
        printf("Unknown opcode %d\n", opcode);
        break;
    }

    cpu->accumulator += instruction;
    cpu->pc++;
}

/**
 * Print the binary representation of a byte
 * @param byte The byte to print
 */
void print_binary(unsigned char byte)
{
    for (int i = 7; i >= 0; --i)
    {
        putchar((byte & (1 << i)) ? '1' : '0');
    }
    putchar(' ');
}

/**
 * View the contents of the binary file in human readable format in hexadecimal
 */
void view_binary_file(const char *binary_file)
{
    FILE *file = fopen(binary_file, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open binary file\n");
        exit(EXIT_FAILURE);
    }
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

    free(buffer);
}

/**
 * View the contents of the preprocessed file
 */
void view_preprocessed_file(const char *assembly_file)
{
    FILE *file = fopen(assembly_file, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open assembly file\n");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }

    fclose(file);
}

/**
 * View the contents of the assembly file
 */
void view_assembly_file(const char *assembly_file)
{
    FILE *file = fopen(assembly_file, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open assembly file\n");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }

    fclose(file);
}
/**
 * Generate the binary machine code from the assembly code
 * The assembler (e.g., GNU as) translates the assembly code into binary machine code
 */
void *generate_binary_code(const char *binary_output_file, const char *assembly_file)
{
    // Prepare the command to invoke the assembler (e.g., GNU as)
    char command[MAX_LINE_LENGTH];
    snprintf(command, sizeof(command), "as -o %s %s", binary_output_file, assembly_file);

    // Execute the assembler and check for errors
    int result = system(command);
    if (result != 0)
    {
        fprintf(stderr, "Error: Assembler failed to assemble the code\n");
        exit(EXIT_FAILURE);
    }

    // Read the binary machine code from the output file
    FILE *binary_file = fopen(binary_output_file, "rb");
    if (binary_file == NULL)
    {
        fprintf(stderr, "Error: Unable to open binary file %s\n", binary_output_file);
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
    fclose(input_file);
    fclose(output_file);
}

/**
 * Compile the input C code and return the generated assembly code
 * The C compiler (e.g., GCC) translates the preprocessed C code into assembly code
 * @param input_filename The name of the input file
 */
void *generate_assembly(const char *input_filename, const char *assembly_output_file)
{
    // Prepare the command to invoke GCC
    char command[MAX_LINE_LENGTH];
    snprintf(command, sizeof(command), "gcc -S -o %s %s", assembly_output_file, input_filename);

    // Execute GCC
    if (system(command) != 0)
    {
        fprintf(stderr, "Error: GCC failed to generate assembly code.\n");
        return NULL;
    }

    // Open the file containing the generated assembly code
    FILE *assembly_file = fopen(assembly_output_file, "r");
    if (assembly_file == NULL)
    {
        fprintf(stderr, "Error: Unable to open assembly file %s\n", assembly_output_file);
        return NULL;
    }

    // Read the assembly code and store it in a dynamically allocated buffer
    char *assembly_code = NULL;
    char line[MAX_LINE_LENGTH];
    size_t assembly_size = 0;
    while (fgets(line, sizeof(line), assembly_file) != NULL)
    {
        size_t line_length = strlen(line);
        assembly_code = realloc(assembly_code, assembly_size + line_length + 1);
        if (assembly_code == NULL)
        {
            fprintf(stderr, "Error: Memory allocation failed\n");
            fclose(assembly_file);
            return NULL;
        }
        memcpy(assembly_code + assembly_size, line, line_length);
        assembly_size += line_length;
    }
    // Null-terminate the assembly code string
    assembly_code[assembly_size] = '\0';

    // Close the assembly file
    fclose(assembly_file);

    return assembly_code;
}

int main(int argc, char *argv[])
{
    int arguments = 3;
    int i;
    char *input_path = argv[1];
    char *assembly_path = argv[2];
    char *binary_path = argv[3];

    preprocess(input_path, assembly_path);
    view_preprocessed_file(assembly_path);
    printf("##OUTPUT##\n");

    generate_assembly(input_path, assembly_path);
    view_assembly_file(assembly_path);
    printf("##OUTPUT##\n");

    generate_binary_code(binary_path, assembly_path);
    view_binary_file(binary_path);
    printf("##OUTPUT##\n");

    CPU cpu;
    cpu.pc = 0;
    cpu.accumulator = 0;
    load_program(&cpu, binary_path);

    // Emulate execution loop
    // while (cpu.pc < MEMORY_SIZE)
    // {
    //     execute_instruction(&cpu);
    // }

    printf("Result in accumulator: %d\n", cpu.accumulator);
    printf("##OUTPUT##");

    return EXIT_SUCCESS;
}
