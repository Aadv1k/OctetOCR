#ifndef _OCTET_PREP_H
#define _OCTET_PREP_H

typedef struct {
  unsigned char * bytes;
  int width;
  int height; 
  char label;
} OctetCharacter;

typedef struct {
  OctetCharacter* characters;
  int characterCount;
} OctetData;

OctetData *octet_load_training_data_from_dir(const char *dirpath);

OctetCharacter* octet_load_character_from_image(const char *filepath);

void octet_free_character(OctetCharacter* character);

void octet_write_training_data_to_csv(OctetData *data, const char* filepath);

OctetData *octet_load_training_data_from_csv(const char *filename);

void octet_free_training_data(OctetData* data);

#endif // _OCTET_PREPARE_DATA_H
