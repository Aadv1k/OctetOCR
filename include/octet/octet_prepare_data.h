#ifndef _OCTET_PREPARE_DATA_H
#define _OCTET_PREPARE_DATA_H

typedef struct {
  unsigned char * bytes;
  char label;
} OctetCharacter;

typedef struct {
  OctetCharacter* characters;
  int characterCount;
} OctetData;

OctetData *octet_prepare_training_data_from_dir(const char *dirpath);

void octet_serialize_training_data_to_file(OctetData *data, const char filepath);

void octet_free_data(OctetData* data);

void octet_prepare_training_data_from_file(const char *filepath);


#endif // _OCTET_PREPARE_DATA_H
