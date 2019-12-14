# textencoder

Compile:
g++ -o trans main.cpp

TO RUN: 
./trans -input format- -output format- -input file name-

Format options: UTF-8 (1), UTF-32 (2), Baudot (3), Morse (4)

Examples:
UTF8 from file "mUtf.txt" to Morse 
./trans 1 4 mUtf.txt

Morse from file "mMorse.txt" to UTF32
./trans 4 2 mMorse.txt

Once the command is run, you will be prompted to enter a file name for the encoded file.
If successful, a new file will be created under your entered file name.

NOTE:
Translator will not work properly if your entered input format is different from the format in the file.







