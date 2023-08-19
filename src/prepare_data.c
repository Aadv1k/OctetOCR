#include <octet/prepare_data.h>
#include <octet/image_processing.h>

#define STB_IMAGE_IMPLEMENTATION
#include <octet/stb_image.h>  

#include <dirent.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

OctetData *octet_prepare_training_data_from_dir(const char *dirpath) {
  DIR *dir;
  struct dirent *dir_item;
  if ((dir = opendir(dirpath)) == NULL) {
    fprintf(stderr, "ERROR: Unable to open directory '%s': %s\n", dirpath, strerror(errno));
    exit(1);
  }

  OctetData *data = malloc(sizeof(OctetData));
  if (!data) {
    closedir(dir); // Close the directory before exiting due to memory allocation failure
    return NULL;
  }
  data->characters = NULL;
  data->characterCount = 0;

  while ((dir_item = readdir(dir)) != NULL) {
    if (dir_item->d_name[0] == '.') continue;

    char *fileExt = strrchr(dir_item->d_name, '.');

    if (strcmp(fileExt, ".png") != 0 && strcmp(fileExt, ".jpg") != 0 && strcmp(fileExt, ".jpeg") != 0)
      continue;

    char label = dir_item->d_name[0];
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, dir_item->d_name);

    int width, height, channels;
    unsigned char *image_bytes = stbi_load(filepath, &width, &height, &channels, 0);
    if (!image_bytes) {
      fprintf(stderr, "WARNING: Unable to load image from '%s', ignoring\n", filepath);
      continue;
    }

    if (channels != 1) {
      octet_convert_rgb_image_to_grayscale(image_bytes, width, height);
      channels = 1;
    }

    //octet_crop_edges_grayscale(image_bytes, &width, &height);
    octet_covert_grayscale_image_to_bits(image_bytes, width, height);

    OctetCharacter newCharacter = {
      .bytes = image_bytes,
      .label = label,
      .width = width,
      .height = height
    };

    OctetCharacter *temp = realloc(data->characters, (data->characterCount + 1) * sizeof(OctetCharacter));
    if (!temp) {
      stbi_image_free(image_bytes); // Free the image_bytes if realloc fails
      continue;
    }

    data->characters = temp;
    data->characters[data->characterCount] = newCharacter;
    data->characterCount++;
  }

  closedir(dir);
  return data;
}

void octet_free_data(OctetData *data) {
  for (size_t i = 0; i < data->characterCount; i++) {
    stbi_image_free(data->characters[i].bytes);
  }
  free(data->characters);
  free(data);
}

void octet_dump_training_data_to_file(OctetData *data, const char* filepath) {
  FILE* file = fopen(filepath, "w");

  if (file == NULL) {
    fprintf(stderr, "ERROR: Unable to open file '%s': %s\n", filepath, strerror(errno));
    exit(1);
  }

  fprintf(file, "total_labels: %d\n\n", data->characterCount);

  for (int i = 0; i < data->characterCount; i++) {
    fprintf(file, "bytes: %.*s\n", data->characters[i].width * data->characters[i].height, data->characters[i].bytes);
    fprintf(file, "width: %d\n", data->characters[i].width);
    fprintf(file, "height: %d\n", data->characters[i].height);
    fprintf(file, "label: %c\n", data->characters[i].label);
    fprintf(file, "\n\n");
  }

  if (fclose(file) != 0) {
    fprintf(stderr, "ERROR: Unable to close file '%s': %s\n", filepath, strerror(errno));
    exit(1);
  }
}


OctetData *octet_read_training_data_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
      fprintf(stderr, "ERROR: Unable to open file '%s': %s\n", filename, strerror(errno));
      exit(1);
    }

    OctetData *octetData = (OctetData *)malloc(sizeof(OctetData));

    int totalLabels;

    if (fscanf(file, "total_labels: %d\n", &totalLabels) != 1) {
        fprintf(stderr, "ERROR: Unable to read total_labels\n");
        fclose(file);
        free(octetData);
        return NULL;
    }

    octetData->characters = (OctetCharacter *)malloc(totalLabels * sizeof(OctetCharacter));

    octetData->characterCount = totalLabels;

    for (int i = 0; i < totalLabels; i++) {
        OctetCharacter *character = &octetData->characters[i];

        int width, height;
        if (fscanf(file, "bytes: <binary_pixel_data_%*d>\n") != 0 &&
            fscanf(file, "width: %d\n", &width) == 1 &&
            fscanf(file, "height: %d\n", &height) == 1 &&
            fscanf(file, "label: %c\n", &character->label) == 1) {
            
            character->bytes = (unsigned char *)malloc(width * height * sizeof(unsigned char));
            fread(character->bytes, sizeof(unsigned char), width * height, file);
        } else {
            fprintf(stderr, "ERROR: Corrupted file format, unable to read!\n");
            fclose(file);
            free(octetData->characters);
            free(octetData);
            return NULL;
        }
    }

    fclose(file);
    return octetData;
}

