/*	
Ioanna Zapalidi, sdi1400044	
System Programming Project #2, Spring 2020	
 */
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "date.h"   //my date class
#include "ht.h"     //hash table - diki mas domi
#include "aht.h"    //"advanced" hash table
#include "tree.h"   //bst
#include "record.h" //record class
#include "bb.h"     //blocks and buckets
#include "heap.h"
#include "PairArray.h"
#include "PidArray.h"
#include "StringArray.h"
#include "Communication.h"
#include "main_worker.h"

int main(int argc, char const *argv[])
{
    //Anagnwsi params
    char in_dir[256]; //input directory
    int w = -1;       //number of workers
    int b = -1;       //bufferSize
    PairArray countries(300);

    for (int i = 0; i < argc; i++)
    {
        if (strcmp("-i", argv[i]) == 0)
        {
            strcpy(in_dir, argv[i + 1]);
        }
        if (strcmp("-w", argv[i]) == 0)
        {
            w = atoi(argv[i + 1]);
        }
        if (strcmp("-b", argv[i]) == 0)
        {
            b = atoi(argv[i + 1]);
        }
    }
    if ((w < 0) || (b < 0))
    {
        std::cerr << "error\n";
        return -1;
    }

    //directory metablites
    DIR *dir;
    struct dirent *entry; //xwra
    Communication communicator(b);

    std::ifstream dataset; //edw 8a kanw open to dataset

    //anoigw to input directory pou exei mesa subfolders "Country"
    dir = opendir(in_dir);
    if (dir == NULL)
    {
        std::cerr << "error opening input directory\n";
        return -1;
    }
    //else
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            Pair p(entry->d_name); //eidallws einai country ara to vazoume sto zeugari (pid den exw akoma)
            countries.insert(p);   //add to pair stis xwres m
        }
    }

    PidArray processIds(w);
    StringArray names_in(w);
    StringArray names_out(w);

    // create pipes - onomatodosia vasei AGGREGATOR (out = pros worker, in = apo worker)
    for (int i = 0; i < w; i++)
    {//he normal, successful return value from mkfifo is 0 . In the case of an error, -1 is returned.
        string name_in = to_string(i) + "_in.fifo";
        int test = mkfifo(name_in.c_str(), 0644);
        if (test==-1)
        {
            perror(" Failed to make pipe_in");
            exit(1);
        }

        string name_out = to_string(i) + "_out.fifo";
        test = mkfifo(name_out.c_str(), 0644);
        if (test==-1)
        {
            perror(" Failed to make pipe_in");
            exit(1);
        }

        names_in.insert(name_in);
        names_out.insert(name_out);
    }

    // create children
    for (int i = 0; i < w; i++)
    {
        pid_t child_pid = fork();
        if (child_pid == -1)
        {
            perror(" Failed to fork");
            exit(1);
        }
        if (child_pid == 0)
        { //paidi
            // call worker main
            child_pid = getpid();
            cout << "child started with PID" << child_pid << endl;
            //kalw ti main tou paidiou
            return main_worker(in_dir, b, names_out.items[i].c_str(), names_in.items[i].c_str());
        }
        else //gonios
        {    //krataw ta pIDs twn paidiwn m
            processIds.insert(child_pid);
        }
    }

    for (int i = 0; i < w; i++)
    {
        //xwria oi onomasies twn pipes
        //edw ANOIGEI ta PIPES o AGGR kai 8a ta ksanakleisei sto telos
        int out_fd = open(names_out.items[i].c_str(), O_WRONLY);
        int in_fd = open(names_in.items[i].c_str(), O_RDONLY);

        cout << "aggregator opened pipes for worker: " << processIds.items[i] << endl;

        for (int j = 0; j < countries.size; j++)
        {
            if (j % w == i)
            { //round robin diaxwrismos twn xwrwn ana worker
                countries.items[j].pid = processIds.items[i];
                countries.items[j].out = out_fd;
                countries.items[j].in = in_fd;

                char *buf = communicator.createBuffer();
                communicator.put(buf, countries.items[j].country);
                communicator.send(buf, countries.items[j].out);
                communicator.destroyBuffer(buf);
            }
        }
        //aploikos elegxos gia liksi apostolis xwrwn se worker - epeidi den kseroume posa tha parei o ekastote worker
        char *buf = communicator.createBuffer();
        communicator.put(buf, "BYE");
        communicator.send(buf, out_fd);
        communicator.destroyBuffer(buf);
    }

    //elegxos
    names_in.print();
    names_out.print();
    countries.print();
    processIds.print();

    return 0;

    //commands
    std::string com; //command
    //anagnosi tis apo ton aggr (pipe)

    while (/*printf("?") && */ std::getline(std::cin, com)) //to "?" einai prompt gia ton user
    {
        //std::getline(std::cin, com); //std::cin >> com; doesn't work due to spaces    //an den dinei apo file
        if (com.length() == 0)
            continue;                            //ama m dwseis enter, sunexizw na zhtaw entoles
        char *cstr = new char[com.length() + 1]; //auto 8a kanw tokenize
        strcpy(cstr, com.c_str());               //copy as string to line sto cstr
        char *pch;
        const char delim[2] = " ";
        pch = strtok(cstr, delim);
        std::string comms[8]; //i entoli me ta perissotera orismata einai h insertPatientRecord me d2
        int counter = 0;
        comms[0] = pch;
        //check first word to match with command, check entire command if correct
        if (comms[0] == "/listCountries") // /listCountries --> for each country print PID of corresponding worker
        {
            //den xreiazetai epikoinwnia me workers giati exw "skonaki" ti domi countries
            countries.print_lc();
        }
        else if (comms[0] == "/exit")
        {
            //workers -> log files ara to stelnw se olous na kanoun douleia tous
            //free memory again as needed
            delete[] cstr;
            std::cout << "exiting\n";
            break;
        }
        else if (comms[0] == "/diseaseFrequency") //8. /diseaseFrequency virusName date1 date2 [country]
        {
            //send to all workers an den uparxei country
            while (pch != NULL) //kovw tin entoli sta parts tis
            {
                comms[counter] = pch;
                counter++;
                pch = strtok(NULL, delim);
            }
            if ((counter != 5) || (counter != 4))
                std::cerr << "error\n";
            else if (counter == 5) //exw xwra ara paw se auti ti xwra --> se auto to process na kanw tin entoli
            {
                //find se poio worker einai auti i xwra
                for (int i = 0; i < countries.size; i++) //gia kathe xwra
                {
                    if (countries.items[i].country == comms[4]) //an eisai auti pou psaxnw
                    {
                        //send to worker
                        char minima[com.size() + 1];
                        strcpy(minima, com.c_str());                    //eidallws nmzei oti to com.c_str() einai const char *
                        char *buf = communicator.createBuffer();        //ftiaxnw ton buffer gia na steilw to mnm
                        communicator.put(buf, minima);                  //vazw to minima sto buf
                        communicator.send(buf, countries.items[i].out); //to kanw send ara write
                        communicator.destroyBuffer(buf);                //yeet
                    }                                                   //else continue
                }
            }
            else //if (counter==4) //ara den exw xwra ara ti zitaw apo olous
            {
                for (int i = 0; i < countries.size; i++) //gia kathe xwra
                {
                    char minima[com.size() + 1];
                    strcpy(minima, com.c_str());                    //eidallws nmzei oti to com.c_str() einai const char *
                    char *buf = communicator.createBuffer();        //ftiaxnw ton buffer gia na steilw to mnm
                    communicator.put(buf, minima);                  //vazw to minima sto buf
                    communicator.send(buf, countries.items[i].out); //to kanw send ara write
                    communicator.destroyBuffer(buf);                //yeet
                }
            }
            //kai tr prepei na parw tin apantisi tou worker i twn workers
        }
        else if (comms[0] == "/topk-AgeRanges")
        {
            //send to worker
        }
        else if (comms[0] == "/searchPatientRecord")
        {
            //send se olous
            //opoios epistrefei null, skip
            //an kapoios ton vrei, print
        }
        else if (comms[0] == "/numPatientAdmissions")
        {
            std::cerr << "I am num patient admissions!\n";
            //  /numPatientAdmissions disease d1 d2 [country]
        }
        else if (comms[0] == "/numPatientDischarges")
        {
            std::cerr << "I am numPatientDischarges!\n";
            //  /numPatientDischarges disease d1 d2 [country]
        }
        else
        {
            //std::cerr << "Unknown Command!\n"; //doesn't exit the program, gives the user another chance to type properly this time.
            std::cerr << "error\n";
        }
        delete[] cstr; //just in case
    }                  //end while(1)

    //kleinw pipes workers' ws AGGR
    for (int j = 0; j < countries.size; j++)
    {
        close(countries.items[j].out);
        close(countries.items[j].in);
    }

    // perimenw ta paidia m
    for (int i = 0; i < w; i++)
    {
        pid_t p = processIds.items[i];
        if (waitpid(p, NULL, 0) <= 0)
        {
            perror("wait ");
        }
        else
        {
            cout << "child finished with PID" << p << endl;
        }
    }

    // destroy pipes (elegxomeni diagrafi twn files autwn) ws AGGR
    for (int i = 0; i < w; i++)
    {
        unlink(names_in.items[i].c_str());
        unlink(names_out.items[i].c_str());
    }

    //ta da
    return 0;
}