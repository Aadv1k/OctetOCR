#include <octet/utils.h>

#include <stdio.h>
#include <stdlib.h>

void utils_write_character_to_ppm(OctetCharacter character, const char* filepath) {
  FILE* ppmFile = fopen(filepath, "w");

  if (!ppmFile) {
    printf("Error opening file for writing: %s\n", filepath);
    return;
  }

  // PPM header
  fprintf(ppmFile, "P3\n");
  fprintf(ppmFile, "# Image generated using utils_write_character_to_ppm\n");
  fprintf(ppmFile, "%d %d\n", character.width, character.height);
  fprintf(ppmFile, "255\n");

  // Write pixel values
  for (int i = 0; i < character.width * character.height; i++) {
    unsigned char pixelValue = character.bytes[i];
    fprintf(ppmFile, "%d %d %d\n", pixelValue, pixelValue, pixelValue);
  }

  fclose(ppmFile);
}
