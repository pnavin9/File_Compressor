#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>
#include <time.h>

#define CHUNK_SIZE 1024
//define a function to compress the string data
//input arguments include the pointer to the input string, output string(initialy empty)
// output_size is basically the size of each block that is passed at a time.
// larger CHUNK_SIZE values can lead to better compression ratios but slower processing times
int compress_string(char *input, char *output, int output_size)
{
    z_stream stream;
    int ret;
    //Initializations

    //This is to point to the default values so that garbage is not passed that may lead to segmentation fault.
    stream.zalloc = Z_NULL; 
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    int compression_level = Z_HUFFMAN_ONLY; //set compression level
    int compression_strategy = Z_FILTERED; // set compression strategy
    ret = deflateInit(&stream, compression_level | compression_strategy);
    // Initializes a compression stream for Deflate Algorithm.

    // Z_OK is 0. The value which is returned if there is no error.
    if (ret != Z_OK){
        return ret;
    }

    //handling pointers to input and output data
    stream.avail_in = strlen(input);
    stream.next_in = (Bytef *)input;
    stream.avail_out = output_size;
    stream.next_out = (Bytef *) output;

    // Actual Compression / deflate
    ret = deflate(&stream, Z_FINISH);

    //Cleanup the resources using deflateEnd
    if (ret != Z_STREAM_END){
        deflateEnd(&stream);
        return ret == Z_OK ? Z_BUF_ERROR : ret;
    }

    ret = stream.total_out;
    deflateEnd(&stream);
    return ret;
}
 // Similarly here is a sample code for inflating i.e. decompression

 int decompress_string(char *input,char *output, int output_size)
 {
    z_stream stream;
    int ret;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    //the compressed data contains header information that includes the compression level and strategy 
    //hence we are not expected o pass it in inflateInit.
    ret = inflateInit(&stream);

    if (ret != Z_OK)
    return ret;

    stream.avail_in = strlen(input);
    stream.next_in = (Bytef *) input;
    stream.avail_out = output_size;
    stream.next_out = (Bytef *) output;

    ret = inflate(&stream, Z_FINISH);

    if (ret != Z_STREAM_END)
    {
        inflateEnd(&stream);
        return ret == Z_OK ? Z_BUF_ERROR : ret;
    }

    ret = stream.total_out;
    inflateEnd(&stream);
    return ret;
 }
int main( int argc, char *argv[])
{
    FILE *file = fopen(argv[1],"r");
    if (file == NULL)
    return 1;
    
    fseek(file,0,SEEK_END); //Set the pointer to EOF
    long file_size = ftell(file); // Determine the size of file
    fseek(file,0,SEEK_SET); // Back to beginning of file
    
    char *input_string = (char * )malloc(file_size + 1); 
    fread(input_string,1,file_size,file);
    input_string[file_size] = '\0';
    
    fclose(file);

    char compressed_string[CHUNK_SIZE];
    int compressed_size;

    clock_t start_time = clock(); // To measure compression speed

    compressed_size = compress_string(input_string, compressed_string, CHUNK_SIZE);

    clock_t end_time = clock(); // End of compression time recorded

    FILE *compressed_file = fopen(argv[2],"wb");
    fwrite(compressed_string,1,compressed_size,compressed_file);

    double compression_time = (double) (end_time - start_time)/CLOCKS_PER_SEC;
    double compression_speed = (file_size+1) / compression_time;
    double compression_ratio = (double) ((file_size+1) - compressed_size)/(file_size+1);
    printf("Compression ratio: %.2f%%\n",compression_ratio*100);
    printf("Compression Speed: %.2f bytes/sec\n",compression_speed);
    
    return 0;


}