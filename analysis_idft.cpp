//
// Inverse Fourier Transform of Image
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "myImageIO.h"

#define MAXSIZE 1024

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

#define MAXAMP 8.0e06

// --- main
int main(int argc, char* argv[])
{
  
  double data[MAXSIZE+1]; // Caution: The index starts from 1.
  
  if(argc<4) {
    printf("Usage: sample7 (in_file amp.pgm) (in_file phase.pgm) (out_file.pgm) \n");
    exit(0);
  }

  int x, y;

  // ----- read image and set complex images
  myImageData *amp_img   = new myImageData(); // amplitude image
  myImageData *phase_img = new myImageData(); // phase image
  amp_img->read(argv[1]);
  phase_img->read(argv[2]);

  int W = amp_img->getWidth();
  int H = amp_img->getHeight();

  myImageData *amp_c    = new myImageData();
  myImageData *phase_c  = new myImageData();
  amp_c->init(W, H, 1);
  phase_c->init(W, H, 1);
 
  //--- image data to value
  // set image data
  for(x=0; x<W; x++) {
    for(y=0; y<H; y++) {
      int value;
      double tmp;

      value = amp_img->get(x, y);
      tmp = exp(value/255.0*log(MAXAMP)); // Inverse of : value = int(log(tmp)/log(maxamp)*255.0);
      amp_c->set(x, y, tmp);

      value = phase_img->get(x, y);
      tmp = (value/255.0 * 360.0) - 180.0; // Inverse of : value = int((tmp + 180.0)/360.0*255.0);
      phase_c->set(x, y, tmp);
    }
  }  
  
  // ----- Move DC from the center to the top-left. <==== Point 1 !!
  myImageData * amp   = new myImageData(); // amplitude
  myImageData * phase = new myImageData(); // phase
  amp->init(W, H, 1);
  phase->init(W, H, 1);
  for(x=0; x<W; x++) {
    for(y=0; y<H; y++) {
      double amp_value, phase_value;
      int m, n;
      amp_value   = amp_c->get(x, y);
      phase_value = phase_c->get(x, y);
      m = (W/2 + x) % W;
      n = (H/2 + y) % H;
      amp->set(m, n, amp_value);
      phase->set(m, n, phase_value);
    }
  }

  // Calculate (re, im) image
  myImageData * re_img = new myImageData(); // real part
  myImageData * im_img = new myImageData(); // imaginary part
  re_img->init(W, H, 1);
  im_img->init(W, H, 1);
  for(x=0; x<W; x++) {
    for(y=0; y<H; y++) {
      double re, im;

      double amplitude;
      amplitude = amp->get(x, y);

      double phase_value;
      phase_value = phase->get(x, y)*M_PI/180.0;

      re = amplitude * cos(phase_value);
      im = amplitude * sin(phase_value);

      re_img->set(x, y, re);
      im_img->set(x, y, im);
    }
  }
  
  // ----- 2D Inverse Fourier Transform
  // Horizontal direction
  for(y=0; y<H; y++) {
    int m;
    for(x=0; x<W; x++) {
      m = x*2 + 1;
      data[m]   = re_img->get(x, y);
      data[m+1] = im_img->get(x, y);
    }
    four1(data, W, -1); // Inverse!
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
    four1(data, H, -1); // Inverse
    for(y=0; y<H; y++) {
      m = y*2 + 1;
      re_img->set(x, y, data[m]);
      im_img->set(x, y, data[m+1]);
    }
  }

  // save image data to file
  myImageData * img = new myImageData();
  img->init(W, H, 1);

  for(x=0; x<W; x++) {
    for(y=0; y<H; y++) {
      int value;
      value = re_img->get(x, y);
      img->set(x, y, value);
    }
  }

  img->save(argv[3]);

  return 0;
}

