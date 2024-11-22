#ifndef __FOURIER__
#define __FOURIER__

unsigned char * get_audio(char*, unsigned int*);
double complex *fourier_transform(unsigned char*, unsigned int);
float *magnitude(double complex *, unsigned int);
double complex *cBiggest(float *, int, unsigned int, double complex *);
unsigned char *inverse_fourier_transform(double complex *, unsigned int);
void export_audio(char *, unsigned char *, unsigned int);

#endif