# File_Compressor
A simple file that can be used to compress other files

This file uses Zlib - 1.2.13 for compressing an input file using Z_HUFFMAN_ONLY compression level and Z_FILTERED compression strategy. 
This program also has support for decompression which can be done in a similar fashion as that of compression. Instruction for the same are given in file_compress.c
This has a comand line interface. The .exe file can be run using :
./compress_file <original_file_name> <compresed_file_destination.bin>

The output will be parameters Compression ratio and Compression speed and these can be increased or decreased using other compression_level and compression_strategy.

