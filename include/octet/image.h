#ifndef _OCTET_IMAGE_PROCESSING_H
#define _OCTET_IMAGE_PROCESSING_H

void octet_crop_edges_grayscale(unsigned char* data, int *width, int *height);

void octet_covert_grayscale_image_to_bits(unsigned char* data, int width, int height);

void octet_convert_rgb_image_to_grayscale(unsigned char* data, int width, int height);

#endif // _OCTET_IMAGE_PROCESSING_H
