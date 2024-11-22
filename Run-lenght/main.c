#include <run_lenght.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){

    IMAGE *original_image, *output_image;

    original_image = read_image();

    if(!(strcmp(original_image->ASCII_type,"P8"))) output_image = decode_image(original_image);
    else output_image = encode_image(original_image,3);

    print_image(output_image);
    free_image(original_image);
    free_image(output_image);

    return 0;
}