#include <octet/image_processing.h> 

#include <string.h>

void octet_crop_edges_grayscale(unsigned char* data, int *width, int *height) {
  // crop the left edge
  for (int x = 0; x < *width; x++) {
    for (int y = 0; y < *height; y++) {
      if (data[y * (*width) + x] != 255) {
        memmove(data, data + x, (*width - x) * (*height));
        *width -= x;
        break;
      }
    }
  }

  // crop the right edge
  for (int x = *width - 1; x >= 0; x--) {
    for (int y = 0; y < *height; y++) {
      if (data[y * (*width) + x] != 255) {
        *width = x + 1;
        break;
      }
    }
  }
}
