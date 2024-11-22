#ifndef __CAPTCHA__
#define __CAPTCHA__

#define MAX_COLUMNS 46
#define DEFAULT_MASK_LINES 50
#define DEFAULT_MASK_COLUMNS 30

//Estrutura para acomodar os dados das imagens
struct image{
    char *ASCII_type;
    unsigned int scale,lines,columns;
    unsigned char **matrix;
};

typedef struct image IMAGE;

//Lista para as mascaras e para a imagem segmentada
struct segment_list{
    unsigned char size;
    IMAGE *elem;
};

typedef struct segment_list SEGMENT_LIST;

//Funcoes disponíveis para o usuario
IMAGE *captcha_load_image(char *);
IMAGE *median_filter(IMAGE *);
void write_image(IMAGE *, char *);
unsigned char *solve_captcha(IMAGE *, unsigned char*);
void free_image(IMAGE *);


#endif