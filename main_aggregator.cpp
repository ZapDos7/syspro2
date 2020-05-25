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
#include "TripleArray.h"
#include "Triplette.h"

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
        exit(-1);
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
        exit(-1);
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
    TripleArray pid_in_out(w);

    // create pipes - onomatodosia vasei AGGREGATOR (out = pros worker, in = apo worker)
    for (int i = 0; i < w; i++)
    { //he normal, successful return value from mkfifo is 0 . In the case of an error, -1 is returned.
        string name_in = to_string(i) + "_in.fifo";
        int test = mkfifo(name_in.c_str(), 0644);
        if (test == -1)
        {
            perror(" Failed to make pipe_in");
            exit(1);
        }

        string name_out = to_string(i) + "_out.fifo";
        test = mkfifo(name_out.c_str(), 0644);
        if (test == -1)
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
            //cout << "child started with PID " << child_pid << endl;
            //kalw ti main tou paidiou
            return main_worker(in_dir, b, names_out.items[i].c_str(), names_in.items[i].c_str());
        }
        else //gonios
        {    //krataw ta pIDs twn paidiwn m
            processIds.insert(child_pid);
            Triplette t(child_pid);
            pid_in_out.insert(t);
        }
    }

    for (int i = 0; i < w; i++)
    {
        //xwria oi onomasies twn pipes
        //edw ANOIGEI ta PIPES o AGGR kai 8a ta ksanakleisei sto telos
        int out_fd = open(names_out.items[i].c_str(), O_WRONLY);
        pid_in_out.items[i].out = out_fd;
        int in_fd = open(names_in.items[i].c_str(), O_RDONLY);
        pid_in_out.items[i].in = in_fd;

        //cout << "aggregator opened pipes for worker: " << processIds.items[i] << endl;

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
    //names_in.print();
    //names_out.print();
    //std::cerr << "countries:\n";
    //countries.print();
    //std::cerr << "processIds:\n";
    //processIds.print();
    std::cerr << "triplettes:\n";
    pid_in_out.print();

    //std::cerr << countries.size;

    for (int i = 0; i < w; i++)
    {
        char *buf = communicator.createBuffer();
        communicator.put(buf, "message\n");
        communicator.send(buf, pid_in_out.items[i].out);
        communicator.destroyBuffer(buf);
    }
    //commands
    std::string com; //command
    //anagnosi tis apo ton aggr (pipe)
    return 0;
    while (/*printf("?") && */ std::getline(std::cin, com)) //to "?" einai prompt gia ton user
    {
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
            countries.print_lc(); //den xreiazetai epikoinwnia me workers giati exw "skonaki" ti domi countries
        }
        else if (comms[0] == "/exit")
        {
            //workers -> log files ara to stelnw se olous na kanoun douleia tous
            for (int i = 0; i < countries.size; i++)
            {
                //steile se kathe worker to mnm aplws
                char *buf = communicator.createBuffer();
                communicator.put(buf, comms[0]);
                communicator.send(buf, countries.items[i].out);
                communicator.destroyBuffer(buf);
            }

            //free memory again as needed
            delete[] cstr;
            std::cout << "exiting\n";
            break;
        }
        else if (comms[0] == "/diseaseFrequency") //8. /diseaseFrequency virusName date1 date2 [country]
        {                                         /*
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
                        std::cerr << comms[4] << "\n";
                        char minima[com.size() + 1];
                        strcpy(minima, com.c_str());                    //eidallws nmzei oti to com.c_str() einai const char *
                        char *buf = communicator.createBuffer();        //ftiaxnw ton buffer gia na steilw to mnm
                        communicator.put(buf, minima);                  //vazw to minima sto buf
                        communicator.send(buf, countries.items[i].out); //to kanw send ara write
                        communicator.destroyBuffer(buf);                //yeet
                       //o,ti lavw to tupwnw
                    }
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
                    
                    //twra tupwnw oti m poun oi workers
                }
            }*/
        }
        else if (comms[0] == "/topk-AgeRanges") //mono se 1 xwra afora
        {                                       // /topk-AgeRanges k country disease d1 d2
            /*while (pch != NULL) //kovw tin entoli sta parts tis
            {
                comms[counter] = pch;
                counter++;
                pch = strtok(NULL, delim);
            }
            if (counter != 6)
                std::cerr << "error\n";
            for (int i = 0; i < countries.size; i++) //gia kathe xwra
            {
                if (countries.items[i].country == comms[2]) //an eisai auti pou psaxnw
                {
                    //send to worker
                    std::cerr << comms[4] << "\n";
                    char minima[com.size() + 1];
                        strcpy(minima, com.c_str());                    //eidallws nmzei oti to com.c_str() einai const char *
                        char *buf = communicator.createBuffer();        //ftiaxnw ton buffer gia na steilw to mnm
                        communicator.put(buf, minima);                  //vazw to minima sto buf
                        communicator.send(buf, countries.items[i].out); //to kanw send ara write
                        communicator.destroyBuffer(buf);                //yeet
                        
                       //o,ti mou pei o worker einai i apantisi
                } //else continue
            }*/
        }
        else if (comms[0] == "/searchPatientRecord")
        {
            //send se olous
            //opoios epistrefei "IDK", skip
            //an kapoios ton vrei, print else print oti den uparxei autos o patient
        }
        else if (comms[0] == "/numPatientAdmissions")
        {
            //  /numPatientAdmissions disease d1 d2 [country]
        }
        else if (comms[0] == "/numPatientDischarges")
        {
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