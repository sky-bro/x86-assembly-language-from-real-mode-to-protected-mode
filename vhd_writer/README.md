## My VHD writer for the book.

* Write all bytes (up to 512 bytes) from src_file to to the beginning of tar_file. 
* This is not to insert bytes into tar_file, but to overwrite it.
* Just what we need for this book...

## Usage

* compile
    ```shell
        gcc writer.c -o writer
    ```
* run

    ```shell
        ./writer src.bin LEECHUNG.vhd
    ```