#include <octet/prep.h>
#include <octet/image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <octet/stb_image.h>

#include <stdlib.h>
#include <assert.h>
#include <dirent.h>


OctetData *octet_load_training_data_from_dir(const char *dirpath) {
    DIR *directory;
    struct dirent *item;

    if ((directory = opendir(dirpath)) == NULL) {
        return NULL;
    }

    OctetData *data = malloc(sizeof(OctetData));
    data->characterCount = 0;
    data->characters = NULL;

    while ((item = readdir(directory)) != NULL) {
        const char *dName = item->d_name;
        if (dName[0] == '.')
            continue;

        const char *ext = strrchr(dName, '.') + 1;
        if (strcmp(ext, "jpg") != 0 && strcmp(ext, "jpeg") != 0)
            continue;


        char filepath[256];
        snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, dName);

        // TODO: this is very unsafe
        char *nameWithoutExt = strdup(dName);
        nameWithoutExt[strlen(nameWithoutExt) - strlen(ext) - 1] = '\0';


        int width, height, channels;
        unsigned char *image_bytes = stbi_load(filepath, &width, &height, &channels, 0);
        if (image_bytes == NULL) {
            free(nameWithoutExt);
            continue;
        }

        if (channels != 1) {
            octet_convert_rgb_image_to_grayscale(image_bytes, width, height);
            channels = 1;
        }

        OctetCharacter character = {
            .bytes = malloc(sizeof(unsigned char) * width * height),
            .label = nameWithoutExt[0],
            .width = width,
            .height = height
        };
        memcpy(character.bytes, image_bytes, sizeof(unsigned char) * width * height);

        data->characterCount++;
        data->characters = realloc(data->characters, sizeof(OctetCharacter) * data->characterCount);
        memcpy(&data->characters[data->characterCount - 1], &character, sizeof(OctetCharacter));

        free(nameWithoutExt);
    }

    closedir(directory);
    return data;
}

void octet_free_training_data(OctetData *data) {
    if (data == NULL)
        return;
    for (int i = 0; i < data->characterCount; i++) {
        free(data->characters[i].bytes);
    }
    free(data->characters);
    free(data);
}

void octet_write_training_data_to_csv(OctetData *data, const char *filepath) {
    if (data == NULL || data->characterCount == 0 || data->characters == NULL) {
        return; // Nothing to write
    }

    FILE *csvFile = fopen(filepath, "w");
    if (csvFile == NULL) {
        return;
    }

    fprintf(csvFile, "width,height,label,image_bytes_as_hex\n");

    for (int i = 0; i < data->characterCount; i++) {
        OctetCharacter *character = &data->characters[i];

        fprintf(csvFile, "%d,%d,%c,", character->width, character->height, character->label);

        for (int j = 0; j < character->width * character->height; j++) {
            fprintf(csvFile, "%02X", character->bytes[j]);
            if (j < character->width * character->height - 1) {
                fprintf(csvFile, " ");
            }
        }

        fprintf(csvFile, "\n");
    }
    fclose(csvFile);
}

OctetData *octet_load_training_data_from_csv(const char *filename) {
    FILE *csvFile = fopen(filename, "r");
    if (csvFile == NULL) {
        return NULL;
    }

    OctetData *data = malloc(sizeof(OctetData));
    if (data == NULL) {
        fclose(csvFile);
        return NULL;
    }

    int numLines = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), csvFile) != NULL) {
        numLines++;
    }
    rewind(csvFile);

    data->characterCount = numLines - 1;
    data->characters = malloc(sizeof(OctetCharacter) * data->characterCount);
    if (data->characters == NULL) {
        free(data);
        fclose(csvFile);
        return NULL;
    }

    if (fgets(buffer, sizeof(buffer), csvFile) == NULL) {
        free(data->characters);
        free(data);
        fclose(csvFile);
        return NULL;
    }

    int i = 0;
    while (fgets(buffer, sizeof(buffer), csvFile) != NULL) {
        OctetCharacter *character = &data->characters[i];
        int width, height;

        if (sscanf(buffer, "%d,%d,%c,", &width, &height, &character->label) != 3) {
            free(data->characters);
            free(data);
            fclose(csvFile);
            return NULL;
        }

        int numBytes = width * height;
        character->width = width;
        character->height = height;
        character->bytes = malloc(sizeof(unsigned char) * numBytes);
        if (character->bytes == NULL) {
            for (int j = 0; j < i; j++) {
                free(data->characters[j].bytes);
            }
            free(data->characters);
            free(data);
            fclose(csvFile);
            return NULL;
        }

        char *token = strtok(NULL, ",");
        for (int j = 0; j < numBytes && token != NULL; j++) {
            character->bytes[j] = (unsigned char)strtol(token, NULL, 16);
            token = strtok(NULL, " ,\n");
        }

        i++;
    }

    fclose(csvFile);
    return data;
}}
