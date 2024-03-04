<div align="center"><h1>CtoCPUEmulation</h1></div>
<div align="center">Emulate how a C code runs on CPU</div>
<div align="center" style="color: grey"><sub>Version: Beta 1.2.0</sub></div>
<div align="center">
    •
    <a href="https://docs.google.com/document/d/1nFtFiSOII9s5k_dbEd0aYju97Wj9iX1dYfyFI1vBiF0/edit#heading=h.6ll8yxyq48ox" target='_blank'>Documentation</a>
    </strong>
</div>

## Description
CtoCPUEmulation is a project that emulates the execution of C code. It emulates the conversion of how a C code converts into assembly code then into machine code and then runs on a CPU & what instructions does CPU executes

## Features
- Show the assembly code of the C code
- Show the machine code of the assembly code

## Usage
1. Clone the repository: `git clone https://github.com/fardinkamal62/CtoCPUEmulation.git`
2. Navigate to the project directory: `cd CtoCPUEmulation`
3. Put your C code in `io/input.c` file. Check `io/input.c` for example.
3. Run `gcc main.c -o build/main && build/main` to compile and run the program.

## Contributing
Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License
This project is licensed under the [GNU General Public License v3.0](LICENSE).

## Changelog
### Beta 1.2.0
- Load binary instructions into memory
- Emulate executing binary instructions on CPU
- Show the output of the execution

### Beta 1.0.0

Able to convert C code to assembly code and machine code and show the output.
