#include <octet/prepare_data.h>
#include <octet/image_processing.h>

#define STB_IMAGE_IMPLEMENTATION
#include <octet/stb_image.h>  

#include <dirent.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

int utils_count_images_in_dir(const char *dirpath) {
    int count = 0;
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dirpath);
    if (dir == NULL) {
        perror("Unable to open directory");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue; // Ignore hidden files
        }

        char *fileExt = strrchr(entry->d_name, '.');
        if (fileExt != NULL) {
            if (strcmp(fileExt, ".png") == 0 ||
                strcmp(fileExt, ".jpg") == 0 ||
                strcmp(fileExt, ".jpeg") == 0) {
                count++;
            }
        }
    }

    closedir(dir);
    return count;
}

void octet_free_data(OctetData *data) {
    for (int i = 0; i < data->characterCount; i++) {
        stbi_image_free(data->characters[i].bytes);
    }
}

void octet_dump_training_data_to_file(OctetData *data, const char* filepath) {
    FILE* file = fopen(filepath, "w");

    if (file == NULL) {
        fprintf(stderr, "ERROR: Unable to open file '%s': %s\n", filepath, strerror(errno));
        exit(1);
    }

    fprintf(file, "total_labels: %d\n\n", data->characterCount);

    for (int i = 0; i < data->characterCount; i++) {
        fprintf(file, "width: %d\n", data->characters[i].width);
        fprintf(file, "height: %d\n", data->characters[i].height);
        fprintf(file, "label: %c\n", data->characters[i].label);

        // Convert and write image bytes as hexadecimal
        fprintf(file, "bytes: ");
        for (int j = 0; j < data->characters[i].width * data->characters[i].height; j++) {
            fprintf(file, "%02X", data->characters[i].bytes[j]);
        }
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
        return NULL;
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

    if (!octetData->characters) {
        fclose(file);
        free(octetData);
        return NULL;
    }

    octetData->characterCount = totalLabels;

    for (int i = 0; i < totalLabels; i++) {
        OctetCharacter *character = &octetData->characters[i];

        int width, height;
        if (fscanf(file, "width: %d\n", &width) != 1 ||
            fscanf(file, "height: %d\n", &height) != 1 ||
            fscanf(file, "label: %c\n", &character->label) != 1) {
            fprintf(stderr, "ERROR: Corrupted file format, unable to read!\n");
            fclose(file);
            octet_free_data(octetData); // Free previously allocated data
            return NULL;
        }

        character->width = width;
        character->height = height;

        // Allocate memory for image bytes
        character->bytes = (unsigned char *)malloc(width * height);
        if (!character->bytes) {
            fclose(file);
            octet_free_data(octetData); // Free previously allocated data
            return NULL;
        }

        // Read and decode hexadecimal bytes
        for (int j = 0; j < width * height; j++) {
            unsigned int byte;
            if (fscanf(file, "%2X", &byte) != 1) {
                fprintf(stderr, "ERROR: Corrupted file format, unable to read bytes!\n");
                fclose(file);
                octet_free_data(octetData); // Free previously allocated data
                return NULL;
            }
            character->bytes[j] = (unsigned char)byte;
        }

        fscanf(file, "\n"); // Read the newline character after the bytes
    }

    fclose(file);
    return octetData;
}

OctetData *octet_prepare_training_data_from_dir(const char *dirpath) {
    DIR *dir;
    struct dirent *dir_item;

    if ((dir = opendir(dirpath)) == NULL) {
        fprintf(stderr, "ERROR: Unable to open directory '%s': %s\n", dirpath, strerror(errno));
        exit(1);
    }

    OctetData *data = (OctetData*)malloc(sizeof(OctetData));

    if (!data) {
        closedir(dir);
        return NULL;
    }

    int itemCount = utils_count_images_in_dir(dirpath);
    data->characters = (OctetCharacter*)malloc(sizeof(OctetCharacter) * itemCount);

    if (!data->characters) {
        closedir(dir);
        free(data);
        return NULL;
    }

    data->characterCount = 0;

    while ((dir_item = readdir(dir)) != NULL) {
        if (dir_item->d_name[0] == '.') continue;

        char *filename = strdup(dir_item->d_name);
        char *fileExt = strrchr(filename, '.');

        if (strcmp(fileExt, ".png") != 0 && strcmp(fileExt, ".jpg") != 0 && strcmp(fileExt, ".jpeg") != 0) {
            free(filename);
            continue;
        }

        *fileExt = '\0';

        char label = dir_item->d_name[0];
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, dir_item->d_name);

        int width, height, channels;
        unsigned char *image_bytes = stbi_load(filepath, &width, &height, &channels, 0);
        if (!image_bytes) {
            fprintf(stderr, "WARNING: Unable to load image from '%s', ignoring\n", filepath);
            free(filename);
            continue;
        }

        if (channels != 1) {
            octet_convert_rgb_image_to_grayscale(image_bytes, width, height);
            channels = 1;
        }

        octet_crop_edges_grayscale(image_bytes, &width, &height);
        //octet_covert_grayscale_image_to_bits(image_bytes, width, height);

        OctetCharacter character = {
            .bytes = image_bytes,
            .label = label,
            .width = width,
            .height = height
        };

        data->characters[data->characterCount] = character;

        data->characterCount++;
        free(filename);
    }

    closedir(dir);
    return data;
}
