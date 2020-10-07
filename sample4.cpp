#include <stdio.h>
#include <stdlib.h>
#include "myImageIO.h"
//compare histograms before and after processing

void Histogram(myImageData *img, int binsize){

    FILE *fp = fopen("C:\\Users\\Helindu.DESKTOP-S5054DM\\source\\repos\\Sample\\Histogram.csv","w");
	int W = img->getWidth();
	int H = img->getHeight();

	int nBins = 256/binsize;
	int * histdata = new int [nBins];

	for(int idx = 0; idx < nBins; idx++){
		histdata[idx] = 0;
	}

	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){

			int value = img->get(x,y);
			int bin = value/binsize;
			histdata[bin]++;
		}
	}

	// print the histogram

	for(int idx = 0; idx < nBins; idx++){
        int mid = idx*binsize+binsize/2;
        fprintf(fp,"%d\t,%d\n",mid,histdata[idx]);
        //fprintf(fp,"%d\t%d\n",mid,histdata[idx]);
		printf("[%.03d-%.03d]\t", idx*binsize, (idx+1)*binsize-1);

		int num = histdata[idx];
		int denom = 0.1*W*H/nBins;
		for(int cnt = 0; cnt < num/denom; cnt++){
			printf("*");
		}

		printf("\n");
	}

	delete [] histdata;
    fclose(fp);
	return;

}

void Equalization(myImageData *img, myImageData *out, int binsize){

	FILE *fp = fopen("C:\\Users\\Helindu.DESKTOP-S5054DM\\source\\repos\\Sample\\Histogram_eq.csv","w");
	int W = img->getWidth();
	int H = img->getHeight();

	int nBins = 256/binsize;
	int * histdata = new int [nBins];

	for(int idx = 0; idx < nBins; idx++){
		histdata[idx] = 0;
	}

	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){

			int value = img->get(x,y);
			int bin = value/binsize;
			histdata[bin]++;
		}
	}

	//Histogram Equalization
    int table[256];
	int sum = 0;

	for(int value = 0; value < 256; value++)
    {
        double r = sum/(double)(W*H);

        table[value] = (unsigned char)(255*r);
        sum += histdata[value];
    }

    for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){

			int value = img->get(x,y);
			int value_out = table[value];
			out->set(x,y,value_out);
		}
	}

	binsize = 8;
    nBins = 256/binsize;
    int * histdata1 = new int [nBins];

    for(int idx = 0; idx < nBins; idx++){
		histdata1[idx] = 0;
	}

	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){

			int value = out->get(x,y);
			int bin = value/binsize;
			histdata1[bin]++;
		}
	}
	// print the histogram

	for(int idx = 0; idx < nBins; idx++){
        int mid = idx*binsize+binsize/2;
        fprintf(fp,"%d\t,%d\n",mid,histdata1[idx]);
		printf("[%.03d-%.03d]\t", idx*binsize, (idx+1)*binsize-1);

		int num = histdata1[idx];
		int denom = 0.1*W*H/nBins;
		for(int cnt = 0; cnt < num/denom; cnt++){
			printf("*");
		}

		printf("\n");
	}

    fclose(fp);
	delete [] histdata;
	delete [] histdata1;
	return;

}


int main(int argc, char **argv){

	// read image data to img1

	myImageData * img1 = new myImageData();
	img1->read(argv[1]);

	int W = img1->getWidth();
	int H = img1->getHeight();
	int C = img1->getCH();

	myImageData *img2  = new myImageData();
	img2->init(W, H, C);

	// Histogram(img1, 8);
	 Equalization(img1, img2, 1);
    img2->save("result_equalization");

	delete img1;
    delete img2;
	return 1; // normal termination

}
