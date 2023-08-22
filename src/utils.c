#include <octet/utils.h>

#include <stdio.h>
#include <stdlib.h>

void utils_write_character_to_pbm(OctetCharacter character, const char* filepath) {
  FILE* pbmFile = fopen(filepath, "w");

  if (!pbmFile) {
    printf("Error opening file for writing: %s\n", filepath);
    return;
  }
    
    // REFERENCE: https://netpbm.sourceforge.net/doc/pbm.html
  fprintf(pbmFile, "P1\n");
  fprintf(pbmFile, "# Image generated using utils_write_character_to_pbm\n");
  fprintf(pbmFile, "%d %d\n", character.width, character.height);

  for (int i = 0; i < character.width * character.height; i++) {
    unsigned char pixelValue = character.bytes[i] == 0 ? 1 : 0;
    fprintf(pbmFile, "%d ", pixelValue);
  }

  fclose(pbmFile);
}
