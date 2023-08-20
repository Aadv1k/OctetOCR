#include <octet/image.h> 

#include <string.h>
#include <stdlib.h>

#define OCTET_THRESHOLD 128

void octet_convert_rgb_image_to_grayscale(unsigned char* data, int width, int height) {
  unsigned char* grayscale_data = malloc(width * height); // Only one channel is needed

  if (!grayscale_data) {
    // Handle memory allocation error
    return;
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int rgb_pixel_index = (y * width + x) * 3; // RGB format index
      int grayscale_pixel_index = y * width + x; // Grayscale format index

      unsigned char red = data[rgb_pixel_index];
      unsigned char green = data[rgb_pixel_index + 1];
      unsigned char blue = data[rgb_pixel_index + 2];

      unsigned char grayscale = (unsigned char)(0.299 * red + 0.587 * green + 0.114 * blue);

      grayscale_data[grayscale_pixel_index] = grayscale;
    }
  }

  free(data);
  data = grayscale_data;
}

void octet_threshold_grayscale_image(unsigned char* data, int width, int height, const int threshold) {
  for (int i = 0; i < width*height; i++) {
    data[i] = data[i] >= threshold ? 255 : 0;
  }
}


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
