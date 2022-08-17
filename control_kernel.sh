git clone https://github.com/RhysHuo/gemm_Vitis_Update.git
cd gemm_Vitis_Update
cp kernelMatrixmult.cpp ..
cd ..
rm -rf gemm_Vitis_Update
v++ -t hw --platform xilinx_u250_gen3x16_xdma_4_1_202210_1 -c -k mmult_top -o'kernelMatrixmult.hw.xo' kernelMatrixmult.cpp matrix_mult.h xcl2.hpp
