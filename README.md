# Commands
- ./compression.out -c <file_1> <file_2>
    - Compresses file_1 and puts the compressed contents into file_2
- ./compression.out -d <file_1> <file_2>
    - Decompresses file_1 and puts the decompressed contents into file_2

# Compressed file structure
[symcount size][symbol count][eof bit][sym][count][data]