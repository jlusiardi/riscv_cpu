# Simple RISC-V cpu implementation in Verilog

## Testing

Go to the *test/* directory and run `./run_tests.sh` to run the tests.

### ROM Tests

-t RAMADDRESS:BYTE_COUNT:EXPECTED_VALUE
-t ByteCount@RamAddress=ExpectedValue   -t 4@0=123

### Compile Toolchain

Follow the instructions from the [riscv-gnu-toolchain repository ](https://github.com/riscv/riscv-gnu-toolchain).

Configure with:
```
./configure --prefix=${YOUR_PATH} --with-arch=rv32i --with-abi=ilp32
```

After that, add `${YOUR_PATH}/bin` to the PATH environment variable.


READING:
https://stackoverflow.com/tags/inline-assembly/info


