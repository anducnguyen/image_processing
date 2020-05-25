#include <stdio.h>
#include <stdlib.h>
#include "myImageIO.h"
//Error Diffusion Function
void error_diffusion(myImageData *out){

	int W = out->getWidth();                                                                                    //Getting Width of Output Image
	int H = out->getHeight();                                                                                   //Getting Height of Output Image
	int C = out->getCH();                                                                                       //Getting no. of Colour Channels of Output Image
//------------------------------------------------------------
    int thresh = 200;                                                                                           //Threshold Setting
//------------------------------------------------------------
    int err;                                                                                                    //Error Variable
    int value, valueo;                                                                                          //Input and Output identifiers of Current Pixel
    double prop[4];                                                                                             //Propogation variables

//Processing
	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){

			if (C == 1)                                                                                         //Setting for greyscale image
            {
            value = out->get(x,y);                                                                              //Obtaining Current pixel value

            if(value > thresh)
            {
                valueo = 255;
            }
            else
            {
                valueo = 0;
            }

            err = value - valueo;                                                                              //Calculating Error to be propogated

//Obtaining pixel values from sorrounding pixels
            if(x + 1 < W)prop[0] = out->get(x+1,y);
            if(x - 1 > 0 && y + 1 < H)prop[1] = out->get(x-1,y+1);
            if(y + 1 < H)prop[2] = out->get(x,y+1);
            if(x + 1 < W && y + 1 < H)prop[3] = out->get(x+1,y+1);

//Error Diffusion
            prop[0] += (7/16.0)*err;
            prop[1] += (3/16.0)*err;
            prop[2] += (5/16.0)*err;
            prop[3] += (1/16.0)*err;

//Correcting Propogated Values
            for(int i = 0; i < 4; i++)
            {
                if(prop[i] < 0)prop[i] = 0;
                if(prop[i] > 255)prop[i] = 255;
            }

//Setting Pixel Values
            out->set(x,y,valueo);
            if(x + 1 < W)out->set(x+1,y,(int)prop[0]);
            if(x - 1 > 0 && y + 1 < H)out->set(x-1,y+1,(int)prop[1]);
            if(y + 1 < H)out->set(x,y+1,(int)prop[2]);
            if(x + 1 < W && y + 1 < H)out->set(x+1,y+1,(int)prop[3]);

            }//End of (C == 1)

            if (C == 3)                                                                                             //Setting for RGB image
            {
            for(int c = 0; c < C; c++)
                {
                    value = out->get(x,y,c);                                                                        //Obtaining Current pixel value

            if(value > thresh)
            {
                valueo = 255;
            }
            else
            {
                valueo = 0;
            }

            err = value - valueo;                                                                                   //Calculating Error to be propogated

//Obtaining pixel values from sorrounding pixels
            if(x + 1 < W)prop[0] = out->get(x+1,y,c);
            if(x - 1 > 0 && y + 1 < H)prop[1] = out->get(x-1,y+1,c);
            if(y + 1 < H)prop[2] = out->get(x,y+1,c);
            if(x + 1 < W && y + 1 < H)prop[3] = out->get(x+1,y+1,c);

//Error Diffusion
            prop[0] += (7/16.0)*err;
            prop[1] += (3/16.0)*err;
            prop[2] += (5/16.0)*err;
            prop[3] += (1/16.0)*err;

//Correcting Propogated Values
            for(int i = 0; i < 4; i++)
            {
                if(prop[i] < 0)prop[i] = 0;
                if(prop[i] > 255)prop[i] = 255;
            }

//Setting Pixel Values
            out->set(x,y,c,valueo);
            if(x + 1 < W)out->set(x+1,y,c,(int)prop[0]);
            if(x - 1 > 0 && y + 1 < H)out->set(x-1,y+1,c,(int)prop[1]);
            if(y + 1 < H)out->set(x,y+1,c,(int)prop[2]);
            if(x + 1 < W && y + 1 < H)out->set(x+1,y+1,c,(int)prop[3]);
                }//End of c loop
            }//End of (C == 3)

		}//End of x loop
	}//End of y loop
}//End of error_diffusion

//------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------

//Main Function
int main(int argc, char **argv){

	// Read image data to img1 (Input Image)

	myImageData * img1 = new myImageData();
	img1->read(argv[1]);

	int W = img1->getWidth();                                                                                   //Get Width of Input Image
	int H = img1->getHeight();                                                                                  //Get Height of Input Image
    int C = img1->getCH();                                                                                      //Get No. of Colour Channels of Input Image

	printf("resolution: %d x %d\n", W, H);

	// prepare img2

	myImageData *img2  = new myImageData();
	img2->init(W, H, C);                                                                                        //Initializing img2 dimensions and colour channels (Output Image)
    int value;
//------------------------------------------------------------
//Copying Image data of img1 to img2
	for(int y = 0; y < H; y++)
    {
        for(int x = 0; x < W; x++)
        {
            if(C == 1)                                                                                          //Setting for greyscale image
            {
            value = img1->get(x,y);
            img2->set(x,y,value);
            }

            if(C == 3)                                                                                          //Setting for RGB image
            {
                for(int c = 0; c < C; c++)
                {
                value = img1->get(x,y,c);
                img2->set(x,y,c,value);
                }
            }

        }
    }
//-----------------------------------------------------------
	error_diffusion(img2);                                                                                      //Calling Error Diffusion Function

	// save
	img2->save("result_errdiff");                                                                                       //Saving Output Image


	delete img1;
	delete img2;

	return 0; // normal termination

}




