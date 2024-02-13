<div align="center"><h1>CtoCPUSimulation</h1></div>
<div align="center">Simulate how a C code runs on CPU</div>
<div align="center" style="color: grey"><sub>Version: Beta 1.2.0</sub></div>
<div align="center">
    •
    <a href="https://docs.google.com/spreadsheets/d/1Gy-awnp90zUcDBTf9CqYeFb6_v5o62O8cm85PChxw18/edit?usp=sharing">Doc</a>
    </strong>
</div>

## Description
CtoCPUSimulation is a project that simulates the execution of C code. It simulates the conversion of how a C code converts into assembly code then into machine code and then runs on a CPU.

## Features
- Show the assembly code of the C code
- Show the machine code of the assembly code

## Usage
1. Clone the repository: `git clone https://github.com/fardinkamal62/CtoCPUSimulation.git`
2. Navigate to the project directory: `cd CtoCPUSimulation`
3. Put your C code in `io/input.c` file. Check `io/input.c` for example.
3. Run `gcc main.c -o build/main && build/main` to compile and run the program.

## Contributing
Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License
This project is licensed under the [GNU General Public License v3.0](LICENSE).

## Changelog
### Beta 1.2.0
- Load binary instructions into memory
- Simulate executing binary instructions on CPU
- Show the output of the execution

### Beta 1.0.0

Able to convert C code to assembly code and machine code and show the output.
