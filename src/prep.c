#include <octet/prep.h>
#include <octet/image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <octet/stb_image.h>

#include <stdlib.h>
#include <assert.h>
#include <dirent.h>

OctetCharacter octet_load_character_from_image(const char *filepath) {
  int width, height, channels;
  unsigned char *image_bytes = stbi_load(filepath, &width, &height, &channels, 0);

  if (image_bytes == NULL) {
    return (OctetCharacter){
      .label = '\0',
      .bytes = NULL,
      .width = 0,
      .height = 0
    };
  }

  if (channels != 1) {
    octet_convert_rgb_image_to_grayscale(image_bytes, width, height);
    channels = 1;
  }

  octet_threshold_grayscale_image(image_bytes, width, height,  /* threshold */ 128);

  return (OctetCharacter){
    .bytes = image_bytes,
    .width = width,
    .height = height,
    .label = '\0',
  };
}

void octet_free_character(OctetCharacter character) {
  free(character.bytes);
}

OctetData *octet_load_training_data_from_dir(const char *dirpath) {
    DIR *directory;
    struct dirent *item;

    if ((directory = opendir(dirpath)) == NULL) {
        return NULL;
    }

    OctetData *data = malloc(sizeof(OctetData));
    if (data == NULL) {
        closedir(directory);
        return NULL;
    }
    data->characterCount = 0;
    data->characters = NULL;

    while ((item = readdir(directory)) != NULL) {
        const char *dName = item->d_name;
        if (dName[0] == '.')
            continue;

        const char *ext = strrchr(dName, '.') + 1;
        if (strcmp(ext, "jpg") != 0 && strcmp(ext, "jpeg") != 0)
            continue;

        char filepath[512];
        snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, dName);

        OctetCharacter loadedCharacter = octet_load_character_from_image(filepath);
        if (loadedCharacter.bytes != NULL) {
            data->characterCount++;
            data->characters = realloc(data->characters, sizeof(OctetCharacter) * data->characterCount);
            data->characters[data->characterCount - 1] = loadedCharacter;
            data->characters[data->characterCount - 1].label = dName[0];
        }
    }

    closedir(directory);
    return data;
}

void octet_free_training_data(OctetData *data) {
    if (data == NULL) return;
    for (int i = 0; i < data->characterCount; i++) {
        free(data->characters[i].bytes);
    }
    free(data->characters);
    free(data);
}

void octet_write_training_data_to_csv(OctetData *data, const char *filepath) {

    if (data == NULL || data->characterCount == 0 || data->characters == NULL) {
        return;
    }

    FILE *csvFile = fopen(filepath, "w");
    if (csvFile == NULL) {
        return;
    }

    fprintf(csvFile, "width,height,label,image_bytes_as_uint8\n");

    for (int i = 0; i < data->characterCount; i++) {
        OctetCharacter *character = &data->characters[i];

        if (character->bytes == NULL) {
            fclose(csvFile);
            return;
        }

        fprintf(csvFile, "%d,%d,%c,", character->width, character->height, character->label);

        for (int j = 0; j < character->width * character->height; j++) {
            fprintf(csvFile, "%d", character->bytes[j]);
            if (j < character->width * character->height - 1) {
                fprintf(csvFile, " ");
            }
        }

        fprintf(csvFile, "\n");
    }
    fclose(csvFile);
}

OctetData *octet_load_training_data_from_csv(const char *filename) {
  (void)filename;
  assert(0 && "Not implemented");
  return NULL;
}
