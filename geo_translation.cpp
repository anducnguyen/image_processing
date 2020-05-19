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

int translation(myImageData *in, myImageData *out, double delta_x, double delta_y)
{
  int W = in->getWidth(); 
  int H = in->getHeight();

  int x, y;
  for(int x=0; x<W; x++) {
    for(int y=0; y<H; y++) {

      // shift
      double xx, yy;
      xx = x - delta_x;
      yy = y - delta_y;

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

  int W = img1->getWidth();
  int H = img1->getHeight();
	
  translation(img1, img2, 128, 128);

  img2->save("out");

  delete img1;
  delete img2;

  return 0; // normal termination
}

