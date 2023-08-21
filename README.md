# Octet

Octet is an OCR model training system, this includes functionality for both **preparing and training** the data.

This system implements a rudimentary [K-NN](https://www.ibm.com/topics/knn) function to predict the similarity of an `OctetCharacter` which contains the raw image data (`unsigned char*`) against a pre-computed array of characters derived from  [`dataset/`](./dataset)

> **NOTE**
> Library under active development, things may change or break. I also appreciate any input on the code, cheers!

See:
- [Test](#Test)
- [Examples](#examples)
- [Todo](#todo)

## Test

```console
.\build.bat TEST
.\octet_test.exe
```

## Examples

### Prepare

```c
#include <octet.h>

  /* ... */

  OctetData* data;

  if (!dataFileExists) {
      data = octet_load_training_data_from_dir("./dataset");
      octet_write_training_data_to_csv("./data.csv");
  }
  octet_free_data(data);

  /* ... */
```

### Train

```c
#include <octet.h>

  /* ... */

  OctetData* trainingData = octet_load_training_data_from_dir("./dataset");

  OctetCharacter testCharacter = {
    .bytes = /* Load from an image */;
    .label = NULL;
  }

  octet_k_nearest_neighbour(testCharacter, trainingData, /* k */ 3);

  octet_free_data(trainingData);

  /* ... */
```

## TODO

- [ ] Squash the compiler warnings, run a static memory analyzer, format the code
- [ ] Further testing against varying levels of data, add more data to the [`dataset/`](./dataset)


