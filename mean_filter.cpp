#include <stdio.h>
#include <stdlib.h>
#include "myImageIO.h"

double kernel_mean[3][3] = {{1/9, 1/9, 1/9}, 
                       {1/9, 1/9, 1/9}, 
                       {1/9, 1/9, 1/9}};

float kernel_laplace1[3][3] = {{1, 1, 1}, 
                       {1, -8, 1}, 
                       {1, 1, 1}};

float kernel_laplace2[3][3] = {{1, 1, 1}, 
                       {1, -4, 1}, 
                       {1, 1, 1}};

float kernel_x[3][3] = {{-1, 0, 1}, 
                       {-2, 0, 2}, 
                       {-1, 0, 1}};

float kernel_y[3][3] = {{1, 0, -1}, 
                       {2, 0, -2}, 
                       {1, 0, -1}};



void mean_filter(myImageData * in, myImageData *out){
	int W = in->getWidth();
	int H = in->getHeight();
	int C = in->getCH();
	const int winsize = 5;
	
	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
			
			double sum = 0;
			int cnt = 0;
			
			for(int j = -winsize; j <= winsize; j++){
		
				int yp = y + j;		
				if(yp < 0) yp = yp + H;
				if(yp > H-1) yp = yp - H;
				
				for(int i = -winsize; i <= winsize; i++){
					
					int xp = x + i;
					if(xp < 0) xp = xp + W;
					if(xp > W-1) xp = xp - W;
					
					double val_in = in->get(xp, yp);
					
					sum = sum + val_in ;
					// sum = sum + val_in * kernelx[i+1][j+1];
					cnt = cnt + 1;
					
				}
			}
			double origin  = in->get(x,y);
			// double val_out = double(origin) + (double(origin)- double(sum));
			double mean = double(sum)/double(cnt) ;
			// double val_out = double(sum) +127 ;
			double val_out = double(mean);
			out->set(x, y, val_out);
			
		}
	
	}

}


void detail_enhance(myImageData * in, myImageData *out){
	int W = in->getWidth();
	int H = in->getHeight();
	int C = in->getCH();
	const int winsize = 5;
	
	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
			
			double sum = 0;
			int cnt = 0;
			
			for(int j = -winsize; j <= winsize; j++){
		
				int yp = y + j;		
				if(yp < 0) yp = yp + H;
				if(yp > H-1) yp = yp - H;
				
				for(int i = -winsize; i <= winsize; i++){
					
					int xp = x + i;
					if(xp < 0) xp = xp + W;
					if(xp > W-1) xp = xp - W;
					
					double val_in = in->get(xp, yp);
					
					sum = sum + val_in ;
					// sum = sum + val_in * kernelx[i+1][j+1];
					cnt = cnt + 1;
					
				}
			}
			double origin  = in->get(x,y);
			// double val_out = double(origin) + (double(origin)- double(sum));
			double mean = double(sum)/double(cnt) ;
			// double val_out = double(sum) +127 ;
			double val_out = double(origin) + 2*(double(origin) - double(mean));
			out->set(x, y, val_out);
			
		}
	
	}

}

void kernel_mean_filter(myImageData * in, myImageData *out){
	int W = in->getWidth();
	int H = in->getHeight();
	int C = in->getCH();
	const int winsize = 1;
	
	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
			
			double sum = 0;
			
			
			for(int j = -winsize; j <= winsize; j++){
		
				int yp = y + j;		
				if(yp < 0) yp = yp + H;
				if(yp > H-1) yp = yp - H;
				
				for(int i = -winsize; i <= winsize; i++){
					
					int xp = x + i;
					if(xp < 0) xp = xp + W;
					if(xp > W-1) xp = xp - W;
					
					double val_in = in->get(xp, yp);
					
					
					// sum = sum + val_in * kernel_mean[i+1][j+1];
					sum = sum + val_in * kernel_laplace1[i+1][j+1];
					
				}
			}
			double origin  = in->get(x,y);
			double mean = double(sum) ;
			double val_out = double(mean) +127;
			out->set(x, y, val_out);
			
		}
	
	}

}

void detail_enhance_using_kernel(myImageData * in, myImageData *out){
	int W = in->getWidth();
	int H = in->getHeight();
	int C = in->getCH();
	const int winsize = 1;


	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
			
			double sum = 0;
			
			double cnt =0;
			for(int j = -winsize; j <= winsize; j++){
		
				int yp = y + j;		
				if(yp < 0) yp = yp + H;
				if(yp > H-1) yp = yp - H;
				
				for(int i = -winsize; i <= winsize; i++){
					
					int xp = x + i;
					if(xp < 0) xp = xp + W;
					if(xp > W-1) xp = xp - W;
					
					double val_in = in->get(xp, yp);
					
					
					// sum = sum + val_in * kernel_mean[i+1][j+1];
					sum = sum + val_in * kernel_x[i+1][j+1]*kernel_x[i+1][j+1]*kernel_y[i+1][j+1]*kernel_laplace1[i+1][j+1]*kernel_laplace2[i+1][j+1];
					cnt = cnt +1;
				}
			}
			double origin  = in->get(x,y);
			// double mean = double(sum);
			double edge = double(sum);
			double val_out = double(origin) +  double(edge);
			// double val_out = double(sum) +127 ;
			// double val_out = double(origin) + 2*(double(origin) - double(mean))+127;
			out->set(x, y, val_out);
			
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
	mean_filter(img1, img2);
	img2->save("./HW/mean_filter_result");	

	// detail enhance using mean
	myImageData *img3  = new myImageData();
	img3->init(W, H, 1);
	detail_enhance(img1, img3);
	img3->save("./HW/detail_enhance_using _mean_result");	


	// mean result using kernel
	myImageData *img4  = new myImageData();
	img4->init(W, H, 1);
	kernel_mean_filter(img1, img4);
	img4->save("./HW/kernel_mean_filter_results");	
	
		// detail enhance result using kernel
	myImageData *img5  = new myImageData();
	img5->init(W, H, 1);
	detail_enhance_using_kernel(img1, img5);
	img5->save("./HW/detail_enhance_using _kernel_result");


	delete img1;
	delete img2;
	delete img3;
	delete img4;
	delete img5;
	
	return 0; // normal termination
	
}




