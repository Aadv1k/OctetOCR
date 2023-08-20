#include <octet/prep.h>
#include <octet/image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <octet/stb_image.h>

OctetData *octet_load_training_data_from_dir(const char *dirpath) {
  DIR* directory;
  struct dirent item;

  if ((directory = opendir(dirpath)) == NULL) {
    return NULL;
  }

  OctetData *data = malloc(sizeof(OctetData));
  data->characterCount = 0;

  while ((item = readdir(directory)) != NULL) {
    const char* dName = item->d_name;
    if (dName[0] == '.') continue;

    const char* ext = strrchr(dName,'.') + 1;
    if (strcmp(ext, "jpg") != 0 && strcmp(ext, "jpeg") != 0) continue;
    
    const char* filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, dName);

    int width, height, channels;
    unsigned char *image_bytes = stbi_load(filepath, &width, &height, &channels, 0);
    if (image_bytes == NULL) return NULL;

    if (channels != 1) {
        octet_convert_rgb_image_to_grayscale(image_bytes, width, height);
        channels = 1;
    }

    OctetetCharacter character = {
        .bytes = image_bytes,
        .label = 'A', // TODO: this will be changed
        .width = width,
        .height = height
    }

    data->characterCount++;
    data->characters = realloc(sizeof(OctetCharacter) * data->characterCount);
    data->characters[data->characterCount] = character;
  }

  closedir(directory);
}

void octet_free_training_data(OctetData* data) {
  if (data == NULL) return;
  for (int i = 0; i < data->characterCount; i++) {
    stbi_image_free(data->characters[i].bytes);
  }
  free(data->characters);
  free(data);
}

void octet_write_data_to_csv(OctetData *data, const char* filepath) {
  assert(0 && "octet_write_data_to_csv not implemented");
}

OctetData *octet_load_training_data_from_csv(const char *filename) {
  assert(0 && "octet_load_training_data_from_csv not implemented");
}

