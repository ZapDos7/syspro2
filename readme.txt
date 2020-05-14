Ioanna Zapalidi
sdi1400044

2nd System Programming Assignment

Used: C++11 (Developed on Linux Ubuntu 16.04), used git for version control and valgrind for debugging purposes.

Contents:
- this readme
- Makefile
- create_infiles.sh (the required bash script)

Use:
- While in the project folder, run "$make" to create the .o files and the executable
- execute "$make run" to execute (to see the parameters values and alter them, see the Makefile file)
- execute "$make val" to execute using valgrind (requires it to be installed: sudo apt-get install valgrind)
- execute "$make clean" to clean the working folder.
- execute "$make script" (doesn't require "$make" beforehand) to execute the script and create the input directory.

Notes:
- The bash script can create records with the second field as exit without having a correpsonding entry record, which is then handled properly by the rest of the program, as instructed.

Conclusions:

Sources:
- Disease Names: https://www.cdc.gov/diseasesconditions/az/ (selection)
- Country Names: https://history.state.gov/countries/all (selection)
- My previous project for this course.