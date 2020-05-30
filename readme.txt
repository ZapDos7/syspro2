Ioanna Zapalidi
sdi1400044

2nd System Programming Assignment

Used: C++11 (Developed on Linux Ubuntu 16.04), used git for version control and valgrind for debugging purposes.

Contents:
- this readme
- Makefile
- create_infiles.sh (the required bash script)
- previous assignment's files (bb, date, heap, ht, record, tree .cpp & .h files)
- main_aggregator.cpp, main_worker.cpp/.h
- date_format, quicksort files
- Pair.h/.cpp (class that combines info about: country, p_id, in and out fds)
- Triplette.h/.cpp (class that connects a process with its in and out fds)
- Array.h which is a generic class of arrays used in PairArray, PidArray, TripleArray and StringArray.

Use:
- While in the project folder, run "$make" to create the .o files and the executable
- execute "$make run" to execute (to see the parameters values and alter them, see the Makefile file)
- execute "$make val" to execute using valgrind (requires it to be installed: sudo apt-get install valgrind)
- execute "$make clean" to clean the working folder.
- execute "$make script" (doesn't require "$make" beforehand) to execute the script and create the input directory.

Notes:
- The script file allowσ for EXIT records that don't correspond to an ENTRY record to exist. In order to produce only proper matches, lines: 52, 85-98 should be un-commented.

Communication Protocol:
- The "Communication" class handles the writing and reading on the pipes. It creates a buffer, on which processes can write, and sends the message through the pipe, until another process reads it.
- When the parent process (main_aggregator) sends the countries to each worker, when the latter receive "BYE" they realise it's time to stop reading from the pipe.
- When any worker sends "IDK" to the parent, it means that the command yields no result (a failed execution).

Sources:
- class' notes
- My previous project for this course.
- functions "readall" and "writeall" from the book "Unix Programming" by M.J. Rochkind used in the Communication class