//
// Fourier Transform of Image
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "myImageIO.h"

#define MAXSIZE 1024

#define TEST

//**********************************
// Fourier Transform function (FFT)
// Reference: Numerical Recipes in C (Japanese edition), æŠ€è¡“è©•è«–ç¤¾ã€1993
// 
//  void four1(double data[], unsigned long nn, int isign)
//    data : odd => real part, even => imaginary part
//    nn   : N-point DFT => nn=N. nn must be the power of 2.
//    isign: = -1 : inverse Fourier transform
//**********************************

#define SWAP(a, b) tempr=(a);(a)=(b);(b)=tempr

void four1(double data[], unsigned long nn, int isign)
{
  unsigned long n, mmax, m, j, istep, i;
  double wtemp, wr, wpr, wpi, wi, theta;
  double tempr, tempi;

  n = nn << 1;
  j = 1;
  for(i=1; i<n; i+=2) {
    if(j > i) {
      SWAP(data[j], data[i]);
      SWAP(data[j+1], data[i+1]);
    }
    m = n >> 1;
    while(m >= 2 && j > m) {
      j -= m;
      m >>= 1;
    }
    j += m;
  }
  mmax = 2;
  while(n > mmax) {
    istep = mmax << 1;
    theta = isign*(M_PI*2/mmax);
    wtemp = sin(0.5*theta);
    wpr = -2.0 * wtemp * wtemp;
    wpi = sin(theta);
    wr = 1.0;
    wi = 0.0;
    for(m=1; m<mmax; m+=2) {
      for(i=m; i<=n; i+=istep) {
	j = i + mmax;
	tempr = wr*data[j] - wi*data[j+1];
	tempi = wr*data[j+1] + wi*data[j];
	data[j] = data[i] - tempr;
	data[j+1] = data[i+1] - tempi;
	data[i] += tempr;
	data[i+1] += tempi;
      }
      wr = (wtemp=wr)*wpr - wi*wpi + wr;
      wi = wi*wpr + wtemp*wpi + wi;
    }
    mmax = istep;
  }

  if(isign==-1) {
    for(i=1; i<=n; i++) {
      data[i] /= nn;
    }
  }

}

// --- main
int main(int argc, char* argv[])
{
  
  double data[MAXSIZE+1]; // Caution: The index starts from 1.
  
#ifdef TEST
  // ----------------- for "four1()" test ------------------------
  for(int i=0; i<=16; i++) { // clear data
    data[i] = 0.0;
  }
  
  data[1]  =  1.0;
  data[3]  =  1.0;
  data[5]  =  1.0;
  data[7]  =  1.0;
  data[9]  =  1.0;
  data[11] =  1.0;
  data[13] =  1.0;
  data[15] =  1.0;
  
  four1(data, 8, 1);

  for(int i=1; i<=16; i+=2) {
    printf("data[%d] = %10.5lf + %10.5lf j \n", (i+1)/2, data[i], data[i+1]);
  }
  
  printf("\n");
  exit(0);
  // ------------------------------------------------------------
#endif  


#ifndef TEST
  // ------------------------------------------------------------  
  if(argc<4) {
    printf("Usage: sample6 (in_file.pgm) (out_file amp.pgm) (out_file phase.pgm)\n");
    exit(0);
  }

  // ----- read image and set complex images
  // read image data to img
  myImageData * img = new myImageData();
  img->read(argv[1]);
  
  int W = img->getWidth();
  int H = img->getHeight();
  
  // Set (re, im) image
  myImageData * re_img = new myImageData(); // real part
  myImageData * im_img = new myImageData(); // imaginary part
  re_img->init(W, H, 1);
  im_img->init(W, H, 1);

  int x, y;

  // ----- 2D Fourier Transform
  // Horizontal direction
  for(y=0; y<H; y++) {
    int m;
    for(x=0; x<W; x++) {
      m = x*2 + 1;
      data[m]   = img->get(x, y); // real part = pixel value
      data[m+1] = 0;              // imaginary part = 0
    }
    four1(data, W, 1);
    for(x=0; x<W; x++) {
      m = x*2 + 1;
      re_img->set(x, y, data[m]);
      im_img->set(x, y, data[m+1]);
    }
  }

  // Vertical direction
  for(x=0; x<W; x++) {
    int m;
    for(y=0; y<H; y++) {
      m = y*2 + 1;
      data[m]   = re_img->get(x, y);
      data[m+1] = im_img->get(x, y);
    }
    four1(data, H, 1);    
    for(y=0; y<H; y++) {
      m = y*2 + 1;
      re_img->set(x, y, data[m]);
      im_img->set(x, y, data[m+1]);
    }
  }

  // caluculate amplitude and phase
  myImageData * amp   = new myImageData(); // amplitude
  myImageData * phase = new myImageData(); // phase
  amp->init(W, H, 1);
  phase->init(W, H, 1);

  for(x=0; x<W; x++) {
    for(y=0; y<H; y++) {
      double re, im;
      re = re_img->get(x, y);
      im = im_img->get(x, y);

      double amplitude;
      amplitude = sqrt(re*re + im*im);
      amp->set(x, y, amplitude);

      double phase_value;
      phase_value = atan2(im, re)*180.0/M_PI; // use atan2() function!
      phase->set(x, y, phase_value);
    }
  }

  // ----- Move DC to the center. Make amp_c, phase_c. <==== Point 1 !!
  myImageData *amp_c    = new myImageData();
  myImageData *phase_c  = new myImageData();
  amp_c->init(W, H, 1);
  phase_c->init(W, H, 1);
  for(x=0; x<W; x++) {
    for(y=0; y<H; y++) {
      double amp_value, phase_value;
      int m, n;
      amp_value   = amp->get(x, y);
      phase_value = phase->get(x, y);
      m = (W/2 + x) % W;
      n = (H/2 + y) % H;
      amp_c->set(m, n, amp_value);
      phase_c->set(m, n, phase_value);
    }
  }

  //--- Visualize as images
  myImageData *amp_img   = new myImageData(); // amplitude image
  myImageData *phase_img = new myImageData(); // phase image
  amp_img->init(W, H, 1);
  phase_img->init(W, H, 1);
  
  // Find maximum value
  double maxamp;
  maxamp = 0;
  for(x=0; x<W; x++) {
    for(y=0; y<H; y++) {
      double tmp;
      tmp = amp_c->get(x, y);
      if((tmp > maxamp)) {
        maxamp = tmp;
      }
    }
  }
  // printf("maxamp= %lf\n", maxamp);

  // set image data
  for(x=0; x<W; x++) {
    for(y=0; y<H; y++) {
      int value;
      double tmp;

      tmp = amp_c->get(x, y);
      // value = int(tmp/maxamp * 255.0);// visualize amp value as it is.
      value = int(log(tmp)/log(maxamp)*255.0);// take log of amp. <== Point 2 !!
      amp_img->set(x, y, value);

      tmp = phase_c->get(x, y);
      value = int((tmp + 180.0)/360.0*255.0);
      phase_img->set(x, y, value);

    }
  }  

  amp_img->save(argv[2]);
  phase_img->save(argv[3]);

#endif

  return 0;
}

