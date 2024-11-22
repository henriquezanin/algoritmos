#ifndef __RUN_LENGHT__
#define __RUN_LENGHT__

//Estrutura para acomodar os dados das imagens
struct image{
    char *ASCII_type;
    unsigned int scale,lines,columns;
    char **matrix;
};

typedef struct image IMAGE;


IMAGE *read_image();
IMAGE *encode_image(IMAGE *, int);
IMAGE *decode_image(IMAGE *);
void print_image(IMAGE *);
int free_image(IMAGE *);

#endif