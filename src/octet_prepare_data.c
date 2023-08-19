#include <octet/octet_prepare_data.h>
#include <octet/stb/stb_image.h>

#include <dirent.h>
#include <assert.h>
#include <stdlib.h>

OctetData *octet_prepare_training_data_from_dir(const char *dirpath) {
  DIR *dir;
  struct dirent *dir_item;
  if ((dir = opendir(dirpath)) == NULL) {
    fprintf(stderr, "ERROR: Unable to open directory '%s': %s\n", dirpath, strerror(errno));
    exit(1);
  }

  OctetData *data = malloc(sizeof(OctetData));
  data->characters = NULL;
  data->characterCount = 0;

  while ((dir_item = readdir(dir)) != NULL) {
    if (dir_item->d_name[0] == '.') continue;

    char *fileExt = strrchr(dir_item->d_name, '.');

    if (strcmp(fileExt, ".png") != 0 && strcmp(fileExt, ".jpg") != 0 && strcmp(fileExt, ".jpeg") != 0)
      continue;

    fileExt[0] = '\0';

    if (strlen(dir_item->d_name) <= 4) {
      fprintf(stderr, "WARNING: Ignoring file with invalid name: %s\n", dir_item->d_name);
      continue;
    }

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
      fprintf(stderr, "WARNING: Ignoring image '%s': Training data expects binary or single-channel images\n", filepath);
      stbi_image_free(image_bytes);
      continue;
    }

    octet_crop_edges_grayscale(image_bytes, &width, &height);

    data->characters = realloc(data->characters, (data->characterCount + 1) * sizeof(OctetCharacter));
    data->characters[data->characterCount] = (OctetCharacter){
      .bytes = image_bytes,
      .label = label,
      .width = width,
      .height = height
    };
    data->characterCount++;
  }

  closedir(dir);
  return data;
}

void octet_serialize_training_data_to_file(OctetData *data, const char filepath) {
  assert(0 && "Not implemented");
}

void octet_free_data(OctetData* data) {
  assert(0 && "Not implemented");
}

void octet_prepare_training_data_from_file(const char *filepath) {
  assert(0 && "Not implemented");
}
