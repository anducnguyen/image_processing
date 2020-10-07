#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "myImageIO.h"
//This Program takes an Input Image and creates an Output Image by applying Non-Uniform Smoothing

double sigmas = 100;    //Value of (Sigma(s)^2)
double sigmar = 400;    //Value of (Sigma(r)^2)

//Gaussian Filter based Non-Uniform Smoothing
void gaussian(myImageData * in, myImageData *out){

	int W = in->getWidth();
	int H = in->getHeight();
	int C = in->getCH();
	const int winsize = 5;
    double gauo;
    double gaui;

	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){

			double sum = 0;
			double gau = 0;

			for(int j = -winsize; j <= winsize; j++){

				int yp = y + j;
				if(yp < 0) yp = yp + H;
				if(yp > H-1) yp = yp - H;

				for(int i = -winsize; i <= winsize; i++){

					int xp = x + i;
					if(xp < 0) xp = xp + W;
					if(xp > W-1) xp = xp - W;

					double val_in = in->get(xp, yp);
					gaui = -(((i*i)+(j*j))/sigmas);
					gauo = exp(gaui);
					gau = gau + gauo;
					sum = sum + gauo*val_in;
				}
			}

			double val_out = (double)sum/gau;
			out->set(x, y, val_out);

		}
	}
}

//Bilateral Filter based Non-Uniform Smoothing
void bilateral(myImageData * in, myImageData *out){

    int W = in->getWidth();
	int H = in->getHeight();
	int C = in->getCH();
	const int winsize = 5;
    double gauo;
    double gaui;
    double bili;
    double bilo;

	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){

			double sum = 0;
			double gau = 0;
            double bil = 0;

			for(int j = -winsize; j <= winsize; j++){

				int yp = y + j;
				if(yp < 0) yp = yp + H;
				if(yp > H-1) yp = yp - H;

				for(int i = -winsize; i <= winsize; i++){

					int xp = x + i;
					if(xp < 0) xp = xp + W;
					if(xp > W-1) xp = xp - W;

					double val_in = in->get(xp, yp);
					double val_in2 = in->get(x, y);

					gaui = -(((i*i)+(j*j))/sigmas);
					gauo = exp(gaui);
					gau = gau + gauo;

					bili = val_in - val_in2;
					if(bili < 0) bili = - bili;
					bili = -((bili*bili)/sigmar);
					bilo = exp(bili);
					bil = bil + gauo*bilo;

					sum = sum + gauo*bilo*val_in;
				}
			}

			double val_out = (double)sum/bil;

			out->set(x, y, val_out);

		}
	}
}

//Guided Filter based Non-Uniform Smoothing
void guided(myImageData * in, myImageData *out){

    int W = in->getWidth();
	int H = in->getHeight();
	int C = in->getCH();
	const int winsize = 2;

	float eps = 0.1;
	int win = (winsize + 1)*(winsize + 1);

	double mu;
	double sigma;

    double a;
    double avga;
    double b;
    double avgb;

	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){

			double sum = 0;

            double musum = 0;
            double sigmasum = 0;
            double temp;

			for(int j = -winsize; j <= winsize; j++){

				int yp = y + j;
				if(yp < 0) yp = yp + H;
				if(yp > H-1) yp = yp - H;

				for(int i = -winsize; i <= winsize; i++){

					int xp = x + i;
					if(xp < 0) xp = xp + W;
					if(xp > W-1) xp = xp - W;

					double val_in = in->get(xp, yp);

					musum = musum + val_in;
				}
			}

			mu = musum/win;

			for(int j = -winsize; j <= winsize; j++){

				int yp = y + j;
				if(yp < 0) yp = yp + H;
				if(yp > H-1) yp = yp - H;

				for(int i = -winsize; i <= winsize; i++){

					int xp = x + i;
					if(xp < 0) xp = xp + W;
					if(xp > W-1) xp = xp - W;

					double val_in = in->get(xp, yp);

					temp = val_in - mu;
					if(temp < 0) temp = -temp;
					sigmasum = sigmasum + (temp*temp);
				}
			}
			sigma = sigmasum/win;

			a = sigma/(sigma + eps);
			b = (eps/(sigma + eps))*mu;
			
				for(int j = -winsize; j <= winsize; j++){

				int yp = y + j;
				if(yp < 0) yp = yp + H;
				if(yp > H-1) yp = yp - H;

				for(int i = -winsize; i <= winsize; i++){

					int xp = x + i;
					if(xp < 0) xp = xp + W;
					if(xp > W-1) xp = xp - W;

					double val_in = in->get(xp, yp);

					sum = sum + ((a*val_in) + b);
				}
			}
			double val_out = (double)sum/win;

			out->set(x, y, val_out);

		}
	}
}

int main(int argc, char **argv){

	// read image data to img1
	myImageData * img1 = new myImageData();
	img1->read(argv[1]);

	int W = img1->getWidth();                                                       //Get Image Width (Pixels) of Input Image
	int H = img1->getHeight();                                                      //Get Image Height (Pixels) of Input Image
    int C = img1->getCH();                                                          //Get No. of Colour Channels of Input Image
    double cnst = 1;

	printf("resolution: %d x %d\n", W, H);

	// prepare img2

	myImageData *img2  = new myImageData();
	img2->init(W, H, C);

	// processing


	// gaussian(img1, img2);
	// bilateral(img1, img2);
	guided(img1, img2);

	// save
	img2->save("result");


	delete img1;
	delete img2;

	return 0; // normal termination

}




