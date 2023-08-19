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

    if (!dataFileExists) {
        OctetData* data = octet_prepare_training_data_from_dir("./data");
        octet_dump_training_data_to_file("./data.dat");
    }


    octet_free_data(data);

    /* ... */
```


