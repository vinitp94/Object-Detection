#include "L138_LCDK_aic3106_init.h"
#include "evmomapl138_gpio.h"
#include "bmp.h"
#include <math.h>

#define PI 3.1415926

unsigned char* bitmap;
int i;
int j;
int k;
int l;
int height;
int width;
uint16_t xcount = 0;
uint16_t ycount = 1;
uint16_t pixel = 0;
int theta;
int r_table[12000][13] = {0};
#pragma DATA_SECTION(r_table,".EXT_RAM");
uint16_t sobel[12000] = {0};
#pragma DATA_SECTION(sobel,".EXT_RAM");
int scount = 0;
double hough[600][13] = {0};
double temphough = 0;
int counter = 0;

interrupt void interrupt4(void)
{
output_left_sample(0);
return;
}

int main(void)
{
for(i = 0; i < 12000; i++){	// initialize to zero
	  	for(j = 0; j < 13; j++){
		  	r_table[i][j] = 0;
	  	}
  	}
  	for(i = 0; i < 600; i++){	// initialize to zero
	  	for (j = 0; j < 13; j++){
		  	hough[i][j] = 0;
	  	}
  	}

bitmap = imread("E:\\Test2.bmp");
  	height = InfoHeader.Height;
  	width = InfoHeader.Width;

  	while(pixel < (height*width)){			// start sobel filter
pixel = (ycount-1)*width + (xcount+1);	// give current pixel // location
  	  	if (xcount < width){
  		  	xcount++;
  	  	}
  	  	else{
  		  	xcount = 1;
  		  	ycount++;
  	  	}
if(xcount != 1 || xcount != width || ycount != 1 || ycount != height){
  	    		if(bitmap[(pixel*3)-1] != 255){
if(bitmap[((pixel-width+1)*3)-1] != 255 && bitmap[((pixel-width)*3)-1] != 255 &&
bitmap[((pixel-width-1)*3)-1] != 255 && bitmap[((pixel-1)*3)-1] != 255 &&
bitmap[((pixel+1)*3)-1] != 255 && bitmap[((pixel+width-1)*3)-1] != 255 &&
bitmap[((pixel+width)*3)-1] != 255 && bitmap[((pixel+width+1)*3)-1] != 255){
sobel[scount] = pixel;
  	    				scount++;
  	    			}
  	    		}
  	  	}
  	}
for (l=0; l<scount;l++){ 	// delete all pixels surrounded by black
	  	bitmap[((sobel[l])*3)-1] = 255;
	  	bitmap[((sobel[l])*3)-2] = 255;
	  	bitmap[((sobel[l])*3)-3] = 255;
  	}	
    	xcount = 0;
  	ycount = 1;
  	pixel = 0;
while(pixel < (height*width)){	// start hough transform
	  						//printf("entering while \n");
	pixel = (ycount-1)*width + (xcount+1);// give current pixel location
	  	if (xcount < width){
		  	xcount++;
	  	}
	  	else{
		  	xcount = 1;
		  	ycount++;
	  	}
	  	if(pixel < (height*width)){
		  	if(bitmap[(pixel*3)-1] != 255){	// if not white
			  	for (theta = 0; theta <= 180; theta += 15){										// check 13 theta values
r_table[counter][theta/(15)] = round((ycount * sin(theta*(PI/180))) + (xcount * cos(theta*(PI/180))));
			  	}
counter++;	//keep track of where to store rho //values for each black point
		  	}
	  	}
  	}
for (j = 0; j < 13; j++){	// store values in hough table
	  	for (k = 0; k < counter; k++){
		 	(hough[((r_table[k][j]) + 300)][j])++;
	  	}
  	}
for(j=0;j < 600;j++){		// print hough table
	  	for(i=0;i<13;i++){
		  	printf("%f ", hough[j][i]);
	  	}
	}
L138_initialise_intr(FS_48000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  	while(1);
}
