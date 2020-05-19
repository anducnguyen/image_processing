#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "myImageIO.h"

int linear_interpolation(myImageData *in, double xx, double yy)
{
  int W = in->getWidth(); 
  int H = in->getHeight();

  int x, y;
  int v1, v2, v3, v4;
  x = int(xx);
  y = int(yy);
  v1 = v2 = v3 = v4 = 0;
  if(x>=0 && x<W-1 && y>=0 && y<H-1) { // Input check
    v1 = in->get(x  ,   y);
    v2 = in->get(x+1,   y);
    v3 = in->get(x  , y+1);
    v4 = in->get(x+1, y+1);
  }

  int value;
  double dx, dy;
  dx = xx - x;
  dy = yy - y;
  value = (unsigned char)(
    v1 * (1-dx)*(1-dy) +
    v2 * dx*(1-dy) +
    v3 * (1-dx)*dy +
    v4 * dx*dy
    + 0.5);

  return value;
}

int transform(myImageData *in, myImageData *out, double mat[3][3])
{
  int W = out->getWidth(); 
  int H = out->getHeight();

  for(int x=0; x<W; x++) {
    for(int y=0; y<H; y++) {
      
      double ss;
      ss = x*mat[2][0] + y*mat[2][1] + mat[2][2];

      double xx, yy;
      xx = (x*mat[0][0] + y*mat[0][1] + mat[0][2])/ss;
      yy = (x*mat[1][0] + y*mat[1][1] + mat[1][2])/ss;

      int value;
      value = linear_interpolation(in, xx, yy);
      out->set(x, y, value);
    }
  }
  return 0;
}

int main(int argc, char **argv){

  // read image data to img1
  myImageData * img1 = new myImageData();
  img1->read(argv[1]);
  myImageData * img2 = new myImageData();
  img2->init(img1->getWidth(), img1->getHeight(), 1);

  myImageData * img3 = new myImageData(); // for cards image
  img3->init(400, 600, 1);

  int W = img1->getWidth();
  int H = img1->getHeight();
	
  // ãƒ•ã‚£ãƒ«ã‚¿ä¿‚æ•°ã®è¨­å®š
  double matrix[3][3] = {
			 { 1.0, 0.0, 0.0}, 
			 { 0.0, 1.0, 0.0},
			 { 0.0, 0.0, 1.0}
			 // *** for card "7"
			 // { 2.21e-1,  9.12e-2, 2.20e2}, 
			 // {-6.98e-2,  1.14e-1, 1.79e2},
			 // { 8.99e-5, -1.46e-4, 7.70e-1}
			 // *** for card "6"
			 //{ 2.20e-1,  1.11e-1, 5.50e1},
			 //{-8.22e-2,  1.10e-1, 1.47e2},
			 //{ 9.10e-5, -1.37e-4, 8.20e-1}
  };
  transform(img1, img2, matrix);

  img2->save("out");

  delete img1;
  delete img2;
  delete img3;

  return 0; // normal termination
}

