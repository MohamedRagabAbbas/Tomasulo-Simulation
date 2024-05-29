# Tomosulo Algorithm Implementation

## Overview

The Tomosulo algorithm is a hardware algorithm for dynamic scheduling of instructions to achieve parallelism at the instruction level. This implementation simulates the Tomosulo algorithm, demonstrating how instructions are issued, executed, and written back to achieve out-of-order execution and resolve data hazards.

## Key Features

- **Instruction Issuing**: Instructions are fetched and issued to the appropriate reservation stations. If the reservation station is busy, the instruction waits until it becomes available.
- **Execution**: Once all operands for an instruction are ready, the instruction is executed. The execution unit computes the result based on the operation type (e.g., LOAD, STORE, ADD, etc.).
- **Write-back**: After the execution is completed, the result is written back to the register file. If there are dependent instructions waiting for this result, they are updated accordingly.
- **Branch Handling**: The algorithm handles branches and jumps, allowing speculative execution and resolving mispredictions by flushing the pipeline as needed.

## Components

1. **Reservation Stations**: Temporary storage buffers for instructions that are waiting to execute. Each reservation station holds information about the instruction, its operands, and whether it is ready to execute.
2. **Register Status Table**: Keeps track of which reservation station is producing a value for each register. This helps in determining if an operand is ready or if it needs to wait for a preceding instruction to complete.
3. **Execution Units**: Functional units that perform the operations specified by the instructions. These include arithmetic units, load/store units, and branch units.
4. **Memory**: Simulates the main memory from which instructions can load data or to which they can store data.
5. **Instruction Queue**: Holds the instructions to be executed. Instructions are read from a file and parsed into the queue.

## Algorithm Workflow

1. **Instruction Fetch**: Instructions are fetched from the instruction memory and placed in the instruction queue.
2. **Issue**: Instructions are issued to reservation stations if there are available slots. If the required reservation station is busy, the instruction waits in the queue.
3. **Execute**: When all operands for an instruction in a reservation station are ready, the instruction is executed by the corresponding functional unit.
4. **Write-back**: The result of the execution is written back to the register file, and any instructions waiting for this result are notified and updated.
5. **Branch Handling**: Branch instructions are handled by speculatively executing them and resolving any mispredictions by flushing the pipeline if necessary.

## Usage

To run the Tomosulo algorithm simulation, follow these steps:

1. **Compile the Code**: Use a C++ compiler to compile the source files. For example, you can use the following command:
    ```bash
    g++ -o tomosulo main.cpp
    ```

2. **Prepare Input Files**: Ensure you have the instruction and memory files (`instructions.txt` and `memory.txt`). These files contain the list of instructions to execute and the initial memory state, respectively.

3. **Run the Program**: Execute the compiled program. The program will read the instructions and memory files, then simulate the execution of the instructions using the Tomosulo algorithm.
    ```bash
    ./tomosulo
    ```

4. **View Results**: The program will output the state of the reservation stations, register file, and other internal states at each cycle. This helps in understanding how instructions are being processed and how the algorithm resolves data hazards and executes instructions out of order.

## Conclusion

This implementation of the Tomosulo algorithm demonstrates a key technique in modern processor design for achieving high performance through dynamic scheduling and out-of-order execution. It provides a practical example of how to handle instruction-level parallelism and resolve various types of data hazards in a pipelined processor.
