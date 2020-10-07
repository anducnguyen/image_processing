#include <stdio.h>
#include <stdlib.h>
#include "myImageIO.h"

float thres = 150;
void binarization(myImageData * in, myImageData *out){
	int W = in->getWidth();
	int H = in->getHeight();
	int C = in->getCH();

	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
			double value = in->get(x,y);
			if (value>thres)
			{
				out->set(x, y, 255);
			}
			else
			{
				out->set(x, y, 0);
			}
        }
    }

}

void truncate(double value){
if (value > thres) 
{
    value = 255;
}
else
{
    value = 0;
}
}

void error_defusion(myImageData * in, myImageData *out){
	int W = in->getWidth();
	int H = in->getHeight();
	int C = in->getCH();
    double err = 0;
	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
			double value_in = in->get(x,y);
            truncate(value_in);
            // err = value_in - value;
            // value1 = in->get(x+1,y);
            // value2 = in->get((x-1,y+1);
            // value3 = in->get((x,y+1);
            // value4 = in->get((x+1,y+1);


            // out->get(x+1,y) += in->get((x+1,y) + 7/16.0*err;
            // out->set(x-1,y+1) += in->get((x-1,y+1) + 3/16.0*err;
            // out->set(x,y+1) += in->get((x,y+1) + 5/16.0*err;
            // out->set(x+1,y+1) += in->get((x+1,y+1) + 1/16.0*err;

        }
    }
}

int main(int argc, char **argv){
	
	// read image data to img1
	
	myImageData * img1 = new myImageData();
	img1->read(argv[1]);

	int W = img1->getWidth();
	int H = img1->getHeight();
	
	// printf("resolution: %d x %d\n", W, H);			
	
	// mean result
	myImageData *img2  = new myImageData();
	img2->init(W, H, 1);
	binarization(img1, img2);
	img2->save("./HW/binarization");

    myImageData *img3  = new myImageData();
	img3->init(W, H, 1);
	error_defusion(img1, img3);
	img3->save("./HW/error_defusion");	

    delete img1;
	delete img2;
    return 0;
}

			