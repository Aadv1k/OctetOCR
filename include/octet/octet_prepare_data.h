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

void octet_dump_training_data_to_file(OctetData *data, const char* filepath);

OctetData *octet_read_training_data_from_file(const char *filename);

void octet_free_data(OctetData* data);

#endif // _OCTET_PREPARE_DATA_H
