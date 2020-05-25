#include <stdio.h>
#include <stdlib.h>
#include "myImageIO.h"

int thres = 150;


void CH1_error_defu(myImageData * in) 

{
	int W = in->getWidth();
	int H = in->getHeight();
	int C = in->getCH();
printf("Number of Chanel: %d",C);
	int value_out, err;
	double prop[4];
	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
		int value = in->get(x,y);
		if (value>thres){value_out = 255;}
		else {value_out =0;}
		err = value -value_out;
            if(x + 1 < W)prop[0] = in->get(x+1,y);
            if(x - 1 > 0 && y + 1 < H)prop[1] = in->get(x-1,y+1);
            if(y + 1 < H)prop[2] = in->get(x,y+1);
            if(x + 1 < W && y + 1 < H)prop[3] = in->get(x+1,y+1);

 	    prop[0] += (7/16.0)*err;
            prop[1] += (3/16.0)*err;
            prop[2] += (5/16.0)*err;
            prop[3] += (1/16.0)*err;
   		
		 for(int i = 0; i < 4; i++)
            {
                if(prop[i] < 0)prop[i] = 0;
                if(prop[i] > 255)prop[i] = 255;
            }
		in->set(x,y,value_out);
            if(x + 1 < W)in->set(x+1,y,(int)prop[0]);
            if(x - 1 > 0 && y + 1 < H)in->set(x-1,y+1,(int)prop[1]);
            if(y + 1 < H)in->set(x,y+1,(int)prop[2]);
            if(x + 1 < W && y + 1 < H)in->set(x+1,y+1,(int)prop[3]);
		}

	}

}

void CH3_error_defu(myImageData * in) 
{
	int W = in->getWidth();
	int H = in->getHeight();
	int C = in->getCH();
printf("Number of Chanel: %d",C);
	int value_out, err;
	double prop[4];
	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
			for(int c=1; c<C;c++){
		int value = in->get(x,y,c);
		if (value>thres){value_out = 255;}
		else {value_out =0;}
		err = value -value_out;
            if(x + 1 < W)prop[0] = in->get(x+1,y,c);
            if(x - 1 > 0 && y + 1 < H)prop[1] = in->get(x-1,y+1,c);
            if(y + 1 < H)prop[2] = in->get(x,y+1,c);
            if(x + 1 < W && y + 1 < H)prop[3] = in->get(x+1,y+1,c);

 	    prop[0] += (7/16.0)*err;
            prop[1] += (3/16.0)*err;
            prop[2] += (5/16.0)*err;
            prop[3] += (1/16.0)*err;
   		
		 for(int i = 0; i < 4; i++)
            {
                if(prop[i] < 0)prop[i] = 0;
                if(prop[i] > 255)prop[i] = 255;
            }
		in->set(x,y,c,value_out);
            if(x + 1 < W)in->set(x+1,y,c,(int)prop[0]);
            if(x - 1 > 0 && y + 1 < H)in->set(x-1,y+1,c,(int)prop[1]);
            if(y + 1 < H)in->set(x,y+1,c,(int)prop[2]);
            if(x + 1 < W && y + 1 < H)in->set(x+1,y+1,c,(int)prop[3]);
			}
		}

	}

}

int main(int argc, char **argv){
	
	// read image data to img1
	
	myImageData * img1 = new myImageData();
	img1->read(argv[1]);

	int W = img1->getWidth();
	int H = img1->getHeight();
	int C = img1->getCH();
	if(C==1){CH1_error_defu(img1);}
	if(C==3){CH3_error_defu(img1);}

	img1->save("./HW/error_defu"); 
	// printf("resolution: %d x %d\n", W, H);			
	

	delete img1;
	//delete img2;
	//delete img3;
	//delete img4;
	//delete img5;
	
	return 0; // normal termination
	
}





