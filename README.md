# Octet

Octet is an OCR model training system, this includes functionality for both **preparing and training** the data.

Since this project is under development, things may change at anytime and I am planning things out as it goes.

## Build

```console
.\build.bat TEST
.\octet_test.exe
```

## Example


```c
#include <octet.h>

    /* ... */

    OctetData* data;

    if (!dataFileExists) {
        data = octet_load_training_data_from_dir("./dataset");
        octet_write_training_data_to_csv("./data.csv");
    }

    data = octet_read_training_data_from_csv("./data.csv");
    octet_free_data(data);

    /* ... */
```


