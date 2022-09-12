# GEMM Accelerator on FPGA

The kernel is reproduced from https://github.com/eejlny/gemm_spmm

The host program is contained in host.cpp. The xcl2.hpp should be included when you build host.exe.

The kernel is contained in kernelMatrixmult.cpp and matrix_mult.h in which multiple parameters are defined.

## Before You Start

Make sure you set up Vitis and XRT environment first so that libraries can be included on the system path.

## Build the Kernel

First you need to compile the kernel code using `-c` option and the Xilinx object `.xo` file will be generated.

Then you can link the Xilinx object `.xo` file to the platform using `-l` or `--link` option.
This will generate the Xilinx binary `.xclbin` file which will be used to program the FPGA.

```
v++ -t hw --platform xilinx_u250_gen3x16_xdma_4_1_202210_1 -c -k mmult_top -o'kernelMatrixmult.hw.xo' kernelMatrixmult.cpp matrix_mult.h xcl2.hpp
v++ -t hw --platform xilinx_u250_gen3x16_xdma_4_1_202210_1 --link kernelMatrixmult.hw.xo -o'kernelMatrixmult.hw.xclbin'
```

## Build the Host

Arbitrary precision type is used in this accelerator. The header file can be found in [HLS_arbitrary_Precision_Types](https://github.com/Xilinx/HLS_arbitrary_Precision_Types).

You can clone this  and add to your working path. You can include it when building the host using `-I` option.

By running the following command, you can get `host.exe`.

```
g++ -g -std=c++14 -I$XILINX_XRT/include -L${XILINX_XRT}/lib/ -I/HLS_arbitrary_Precision_Types/include -o host.exe host.cpp -lOpenCL -pthread -lrt -lstdc++
```

## Run the Application

When you successfully running commands above, you should have two files `kernelMatrixmult.hw.xclbin` and `host.exe` in your folder.

One of matrices is read from data file so you should have this data file in the right format, such as [weights_byte_099.csv](https://github.com/RhysHuo/gemm_Vitis_Update/blob/main/weights_byte_099.csv).

You can run this application using the following command:

```
./host.exe kernelMatrixmult.xclbin <data file> <precision controller> <SN> <SM> <SP>
```
You can choose the precision `<precision controller>` of data and set any matrix size `<SN>` `<SM>` `<SP>` up to the maximum `MAX_N`  `MAX_M`  `MAX_P`.

<precision controller>: `0` for 8 bits, `1` for 2 bits, `2` for 4 bits.

Matrix size: `A: SN*SM` `B: SM*SP` `C: SN*SP`
