/*	
Ioanna Zapalidi, sdi1400044	
System Programming Project #2, Spring 2020	
 */
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#include "date.h"   //my date class
#include "ht.h"     //hash table - diki mas domi
#include "aht.h"    //"advanced" hash table
#include "tree.h"   //bst
#include "record.h" //record class
#include "bb.h"     //blocks and buckets
#include "heap.h"
#include "quicksort.h"
#include "date_format.h"
#include "Communication.h"
#include "StringArray.h"
#include "main_worker.h"

int main_worker(char *in_dir, int b, string name_out, string name_in)
{
    int child_pid = getpid();
    //std::cerr << "!!i am child " << child_pid << "\n";
    Communication communicator(b);
    ht my_ht(500 * 10); //arbitrary size
    aht diseaseHT(500 * 10, 1024);
    aht countryHT(500 * 10, 1024);

    StringArray countries(300);

    int out_fd = open(name_out.c_str(), O_RDONLY); //edw diavazw ti m leei o AGGR
    int in_fd = open(name_in.c_str(), O_WRONLY);   //edw grafw apantisi tou AGGR

    //cout << "child opened pipes " << child_pid << endl;

    while (true)
    {
        char *buf = communicator.createBuffer();
        communicator.recv(buf, out_fd);
        if (string(buf) == "BYE")
        { //elava to eidiko mhnuma oti that's a nono
            break;
        }

        string country = buf;
        countries.insert(country);
        communicator.destroyBuffer(buf);
    }

    //countries.print();

    //directory metablites
    DIR *dir;
    struct dirent *entry; //xwra
    DIR *dir2;
    struct dirent *entry2; //imerominia

    int posa_arxeia = 0; //posa date arxeia exei mesa to folder gia mia xwra

    std::ifstream dataset; //edw 8a kanw open to dataset

    //anoigw to input directory pou exei mesa subfolders "Country"
    dir = opendir(in_dir);
    if (dir == NULL)
    {
        fprintf(stderr, "error opening in_dir (child %d)\n", child_pid);
        exit(-1);
    }
    //else
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue; //skip ., ..
            //fprintf(stderr, "%s by %d\n", entry->d_name, child_pid); //std::cerr << entry->d_name << " " << child_pid << "\n";
            if (countries.has(entry->d_name) == false) //an to entry->d_name einai mesa sto countries m, ta kanw auta else skip
                break;
            else //einai diki m xwra ara paw na tin xeiristw
            {
                //fprintf(stderr, "o %d brike to to %s.\n", child_pid, entry->d_name);
                snprintf(path, sizeof(path), "%s/%s", in_dir, entry->d_name); //path = "../folder2/Xwra"
                dir2 = opendir(path);
                if (dir2 == NULL)
                {
                    fprintf(stderr, "error opening in_dir/%s (child %d)\n", entry->d_name, child_pid);
                    break;
                }
                while ((entry2 = readdir(dir2)) != NULL)
                {
                    if (entry2->d_type == DT_DIR) //if it is a directory, access
                    {
                        if (strcmp(entry2->d_name, ".") == 0 || strcmp(entry2->d_name, "..") == 0)
                            continue;
                        std::cerr << "error\n"; //den prepei na tupwsei tpt gt exoume 1 level subdir mono oxi parapanw!
                    }
                    else
                    {
                        posa_arxeia++;
                    }
                }
                closedir(dir2);
                std::string *date_file_names = new string[posa_arxeia]; //std::string date_file_names[posa_arxeia];
                dir2 = opendir(path);                                   //twra ksananoigw gia na valw ta file names edw mesa
                if (dir2 == NULL)
                {
                    std::cerr << "error opening input subdirectory\n";
                    break;
                }
                //else
                unsigned int i = 0;
                while ((entry2 = readdir(dir2)) != NULL)
                {
                    if (entry2->d_type == DT_DIR) //if it is a directory, access
                    {
                        if (strcmp(entry2->d_name, ".") == 0 || strcmp(entry2->d_name, "..") == 0)
                            continue;
                    }
                    else
                    {
                        std::string entryname(entry2->d_name);
                        date_file_names[i] = entryname;
                        i++;
                    }
                }
                closedir(dir2);

                //sort ta pramata mes to array
                int megethos = sizeof(date_file_names) / sizeof(date_file_names[0]); //posa exei mesa to array
                quickSort(date_file_names, 0, megethos - 1);
                //std::cerr << path << "\n"; // /dir/Xwra
                dir2 = opendir(path);
                if (dir2 == NULL)
                {
                    std::cerr << "error opening input subdirectory\n";
                    break;
                }
                //else
                for (int i1 = 0; i1 < megethos; i1++)
                {
                    //std::cerr << date_file_names[i1] << '\t';
                    std::string path2 = path;
                    path2 += '/';
                    path2 += date_file_names[i1];
                    //std::cerr << path2 << "\n\n";  // /dir/Xwra/XX-YY-ZZZZ
                    dataset.open(path2);
                    if (dataset.is_open() == 0)
                        std::cerr << "error\n";
                    else //we gonna do the thingggg
                    {
                        std::string line;
                        while (std::getline(dataset, line)) //diavazei olo to this date --> we gotta do it for all files in this folder
                        {
                            //line read: ID ENTER fn ln disease age
                            //constructor gia record: ID FName Lname DIsease Country age entryD exitD/-
                            //std::cerr << "paw na valw " << line <<"\n";
                            std::string wannabe = "";
                            std::string help[6]; //boithitiki domi me ta tokens tou line pou 8a ftiaksw swsta
                            int counter = 0;
                            const char *c_string = line.c_str();
                            char *token = strtok((char *)c_string, " ");
                            while (token)
                            {
                                //std::cerr << token << '\n';
                                help[counter] = token;
                                token = strtok(NULL, " ");
                                counter++;
                            }
                            wannabe += help[0]; //wannabe="ID"
                            wannabe += " ";
                            wannabe += help[2]; //wannabe="ID Fname"
                            wannabe += " ";
                            wannabe += help[3]; //wannabe="ID Fname Lname"
                            wannabe += " ";
                            wannabe += help[4]; //wannabe="ID Fname Lname Disease"
                            wannabe += " ";
                            wannabe += entry->d_name; //wannabe="ID Fname Lname Disease Country"
                            wannabe += " ";
                            wannabe += help[5]; //wannabe="ID Fname Lname Disease Country Age"
                            wannabe += " ";
                            if (help[1] == "ENTER") //exoume eisodo
                            {
                                wannabe += date_file_names[i1];
                                record temp_r(wannabe);                  //temp record gia insert
                                record *elegxos = my_ht.insert(&temp_r); //edw ginetai kai elegxos gia unique IDs
                                if (elegxos == NULL)
                                {
                                    std::cerr << "den evala to " << elegxos->get_id() << "\n";
                                    break; //sto piazza eipw8ike oti an brethei kapoio ID duplicate, na proxwrame stis entoles & na mhn sunexizoun ta insertions.
                                }
                                else
                                {
                                    //std::cerr << "Eimai o " << child_pid << " & evala to " << elegxos->get_id() << "\n";
                                    diseaseHT.ainsert(elegxos, false);
                                    countryHT.ainsert(elegxos, true);
                                }
                            }
                            else if (help[1] == "EXIT")
                            {
                                ht_item *h = my_ht.search(help[0]); //bres an uparxei to record me auto to ID
                                if (h == NULL)
                                    continue; //oops this record doesn't exist, akuro, bye
                                else
                                {
                                    date d2(string(entry->d_name));         //d2 = ti paw na valw
                                    date *d1 = h->rec->get_exitDatePtr();   //to uparxon exit date
                                    bool magkas = d1->set;                  //ama eixa prin set=true ara eixa idi exit date, true, else false
                                    date *din = h->rec->get_entryDatePtr(); //to entry date
                                    if (isLater(d2, *din) == 1)             //pas na mou baleis kati pou den einai later tou entry date m
                                    {
                                        std::cerr << "error\n";
                                    }
                                    else //valid basei entry date
                                    {
                                        h->rec->set_exitD(d2.get_date_as_string()); //twra exei exit date
                                        if (magkas == false)                        //prin den imoun set ara update counters:
                                        {
                                            block *blokaki1 = diseaseHT.search(entry->d_name);
                                            if (blokaki1 != NULL)
                                            {
                                                blokaki1->update_c_in(false);
                                            }
                                            block *blokaki2 = countryHT.search(entry->d_name);
                                            if (blokaki2 != NULL)
                                            {
                                                blokaki2->update_c_in(false);
                                            }
                                        }
                                        //eidallws oi metrites den allazoun!
                                    }
                                }
                            }
                        }
                        dataset.close();
                    } //diavase to epomeno arxeio tr
                }     //end for gia chronological
                closedir(dir2);
            }
        }
        posa_arxeia = 0; //epanamhdenizw ton arithmo arxeiwn giati paw se alli xwra
    }

    char *buf = communicator.createBuffer();
    communicator.recv(buf, out_fd);

    fprintf(stderr, "Eimai o worker %d kai perimeno to xairetismo !!! \n", child_pid);

    if (string(buf) == "hi\n")
    {
        fprintf(stderr, "Eimai o worker %d kai elava to minima !!! \n", child_pid);
    }
    else
    {
        std::cerr << " eimai o " << child_pid << " kai den elava to hi !!!!!";
        exit(0);
    }

    communicator.put(buf, "yo");

    communicator.send(buf, in_fd);

    long int total = 0;
    long int success = 0;
    long int failed = 0;

    while (true)
    {
        char *buf = communicator.createBuffer();
        communicator.recv(buf, out_fd);

        if (string(buf) == "/exit")
        {
            std::cerr << " elava /exit " << child_pid << '\n';
        }
        else if (string(buf) == "message\n")
        {
            std::cerr << " eimai o " << child_pid << " kai elava to message";
        }

        std::string com(buf); //com is the command as std::string
        total++;
        fprintf(stderr, "o %d elave to %s", child_pid, com.c_str());
        communicator.destroyBuffer(buf);

        char *cstr = new char[com.length() + 1]; //auto 8a kanw tokenize
        strcpy(cstr, com.c_str());               //copy as string to line sto cstr
        //o AGGR elegxei an tha exei dwsei "\n" o user, o worker 8a parei legit entoli
        char *pch;
        const char delim[2] = " "; // \0
        pch = strtok(cstr, delim);
        std::string comms[6]; //i entoli me ta perissotera orismata einai h topk age ranges (6)
        int counter = 0;
        comms[0] = pch;
        //check first word to match with command, check entire command if correct
        if (comms[0] == "/exit")
        {
            std::cerr << "i am " << child_pid << " and i'm exiting\n";
            //workers -> log files
            ofstream logfile;
            std::string onomaarxeiou = "log_file.";
            onomaarxeiou += to_string(child_pid);
            logfile.open(onomaarxeiou);
            for (int i = 0; i < countries.size; i++) //grafw poies einai oi xwres m
            {
                logfile << countries.items[i];
            }
            logfile << "TOTAL: " << total << "\n";     //posa erwthmata mou irthan
            logfile << "SUCCESS: " << success << "\n"; //posa success
            logfile << "FAIL: " << failed << "\n";     //posa fail
            logfile.close();
            return 0;
        }
        else if (comms[0] == "/diseaseFrequency") //8. /diseaseFrequency virusName date1 date2 [country]
        {
            fprintf(stderr, "Elava to diseaseFrequency kai eimai o %d.\n", child_pid);
            while (pch != NULL)
            {
                comms[counter] = pch;
                counter++;
                pch = strtok(NULL, delim);
            }
            if (counter == 5) //mou edwses onoma xwras
            {
                std::string cntrName = comms[4];
                if (countries.has(cntrName) == false) //den exw auti ti xwra
                {
                    //eidiki episimansi oti mhn me laveis uposin AGGR
                    char *buf = communicator.createBuffer();
                    communicator.put(buf, "IDK");
                    communicator.send(buf, in_fd);
                    communicator.destroyBuffer(buf);
                    failed++;
                }
                else //auti einai i xwra m ara leggo
                {
                    std::string virusName = comms[1];
                    std::string wannabedate1 = comms[2];
                    std::string wannabedate2 = comms[3];
                    if ((date_format(wannabedate1) == false) || (date_format(wannabedate2) == false))
                    {
                        std::cerr << "error\n";
                        failed++;
                        break;
                    }
                    //else
                    date d1(wannabedate1);
                    date d2(wannabedate2);
                    if (isLater(d1, d2) == -1) //an d1>d2, epistrefei -1
                    {
                        std::cerr << "error\n";
                        failed++;
                        break;
                    }
                    //else ok
                    block *apantisi = diseaseHT.search(virusName);
                    if (apantisi == NULL) //mou zitas na brw kati pou den exw sti vasi m
                    {
                        std::string reply = virusName;
                        reply += " ";
                        reply += "0.";
                        failed++;
                        char *buf = communicator.createBuffer();
                        communicator.put(buf, reply.c_str());
                        communicator.send(buf, in_fd);
                        communicator.destroyBuffer(buf);
                    }
                    else
                    {
                        std::string reply = virusName;
                        reply += " ";
                        long int apantisi_num = apantisi->statsC(d1, d2, cntrName);
                        std::string apantisi_str = to_string(apantisi_num);
                        reply += apantisi_str;
                        char *buf = communicator.createBuffer();
                        communicator.put(buf, reply.c_str());
                        communicator.send(buf, in_fd);
                        communicator.destroyBuffer(buf);
                        success++;
                    }
                }
            }
            else if (counter == 4) // den exei xwra ara stelnw ta panta
            {
                std::string virusName = comms[1];
                std::string wannabedate1 = comms[2];
                std::string wannabedate2 = comms[3];
                if ((date_format(wannabedate1) == false) || (date_format(wannabedate2) == false))
                {
                    std::cerr << "error\n";
                    failed++;
                    break;
                }
                //else
                date d1(wannabedate1);
                date d2(wannabedate2);
                if (isLater(d1, d2) == -1) //an d1>d2, epistrefei -1
                {
                    std::cerr << "error\n";
                    failed++;
                    break;
                }
                //else ok
                block *apantisi = diseaseHT.search(virusName);
                if (apantisi == NULL) //mou zitas na brw kati pou den exw sti vasi m
                {
                    std::cerr << "error\n";
                    failed++;
                }
                //else
                std::string reply = virusName;
                reply += " ";
                reply += apantisi->stats(d1, d2);
                char *buf = communicator.createBuffer();
                communicator.put(buf, reply.c_str());
                communicator.send(buf, in_fd);
                communicator.destroyBuffer(buf);
                success++;
            }
            else //mou edwses alla m nt alla orismata
            {
                std::cerr << "error\n";
                failed++;
            }
        }
        else if (comms[0] == "/topk-AgeRanges") // /topk-AgeRanges k country disease [d1 d2]
        {
            //thelei polles alages akoma se polla epipeda domwn
            while (pch != NULL)
            {
                comms[counter] = pch;
                counter++;
                pch = strtok(NULL, delim);
            }
            if ((counter > 6) || counter < 4)
            {
                fprintf(stderr, "error\n");
                exit(-1);
            }
            //int k = atoi(comms[1].c_str());
            std::string countryName = comms[2];
            std::string diseaseName = comms[3];
            block *b = countryHT.search(countryName);
            if (b == NULL)
            {
                std::cerr << "error\n"; //else print
                //std::cerr << countryName << " 0\n";
            }
            //age ranges: 0-20, 21-40, 41-60, 60+
            else
            {
                if (counter == 6)
                {
                    std::string wannabedate1 = comms[4];
                    std::string wannabedate2 = comms[5];
                    if ((date_format(wannabedate1) == false) || (date_format(wannabedate2) == false))
                    {
                        std::cerr << "error\n";
                        break;
                    }
                    //else dates are ok
                    date d1(wannabedate1);
                    date d2(wannabedate2);
                    if (isLater(d1, d2) == -1) //an d1>d2, epistrefei -1
                    {
                        std::cerr << "error\n";
                        break;
                    } //else dates are ok ok ara kaloume
                    //return ta age ranges
                    //b->top_k_diseases(k, d1, d2);
                }
                else
                {
                    std::cerr << "error\n";
                }
            }
        }
        else if (comms[0] == "/searchPatientRecord")
        {
            //  /searchPatientRecord recordID

            while (pch != NULL) //kovw tin entoli sta parts tis
            {
                comms[counter] = pch;
                counter++;
                pch = strtok(NULL, delim);
            }
            if (counter != 2)
            {
                std::cerr << "error1\n";
                failed++;
            }
            else
            {
                ht_item *anazitisis = my_ht.search(comms[counter - 1]);
                if (anazitisis == NULL)
                {
                    std::cerr << "error2\n";
                    failed++;
                }
                else
                {
                    std::string apantisi = "";
                    apantisi += anazitisis->rec->get_id();
                    apantisi += " ";
                    apantisi += anazitisis->rec->get_fname();
                    apantisi += " ";
                    apantisi += anazitisis->rec->get_lname();
                    apantisi += " ";
                    apantisi += anazitisis->rec->get_disease();
                    apantisi += " ";
                    apantisi += anazitisis->rec->get_country();
                    apantisi += " ";
                    apantisi += anazitisis->rec->get_age();
                    apantisi += " ";
                    apantisi += anazitisis->rec->get_entryDate().get_date_as_string();
                    apantisi += " ";
                    apantisi += anazitisis->rec->get_exitDate().get_date_as_string();
                    fprintf(stderr, "%s\n", apantisi.c_str());
                    success++;
                }
            }
        }
        else if (comms[0] == "/numPatientAdmissions") //  /numPatientAdmissions disease d1 d2 [country]
        {
            while (pch != NULL)
            {
                comms[counter] = pch;
                counter++;
                pch = strtok(NULL, delim);
            }
            std::string dis(comms[1]);
            std::string wannabedate1 = comms[2];
            std::string wannabedate2 = comms[3];
            if ((date_format(wannabedate1) == false) || (date_format(wannabedate2) == false))
            {
                std::cerr << "error\n";
                failed++;
                break;
            }
            //else
            date d1(wannabedate1);
            date d2(wannabedate2);
            if (isLater(d1, d2) == -1) //an d1>d2, epistrefei -1
            {
                std::cerr << "error\n";
                failed++;
                break;
            }
            //else ok
            if ((counter > 5) || (counter < 4))
            {
                exit(-1);
                failed++;
            }
            else if (counter == 4) //oxi basei country
            {
                block *blockPtr = diseaseHT.search(dis);
                if (blockPtr == NULL)
                {
                    std::cout << dis << " 0\n";
                    //std::cout << "0\n"; //den eixame kanena krousma
                    success++;
                }
                else
                {
                    fprintf(stdout, "%ld\n", blockPtr->stats(d1, d2));
                    success++;
                }
            }
            else if (counter == 5) //nai basei country
            {
                std::string countryName(comms[4]);
                block *blockPtr = diseaseHT.search(dis);
                if (blockPtr == NULL)
                {
                    std::cout << dis << " 0\n";
                    success++;
                }
                else
                {
                    fprintf(stdout, "%ld\n", blockPtr->statsC(d1, d2, countryName));
                    success++;
                }
            }
        }
        else if (comms[0] == "/numPatientDischarges") //  /numPatientDischarges disease d1 d2 [country]
        {
            while (pch != NULL)
            {
                comms[counter] = pch;
                counter++;
                pch = strtok(NULL, delim);
            }
            std::string dis(comms[1]);
            std::string wannabedate1 = comms[2];
            std::string wannabedate2 = comms[3];
            if ((date_format(wannabedate1) == false) || (date_format(wannabedate2) == false))
            {
                std::cerr << "error\n";
                failed++;
                break;
            }
            //else
            date d1(wannabedate1);
            date d2(wannabedate2);
            if (isLater(d1, d2) == -1) //an d1>d2, epistrefei -1
            {
                std::cerr << "error\n";
                failed++;
                break;
            }
            //else ok
            if ((counter > 5) || (counter < 4))
            {
                exit(-1);
                failed++;
            }
            else if (counter == 4) //oxi basei country
            {
                block *blockPtr = diseaseHT.search(dis);
                if (blockPtr == NULL)
                {
                    std::cout << dis << " 0\n";
                    success++;
                    //std::cout << "0\n"; //den eixame kanena krousma
                }
                else
                {
                    fprintf(stdout, "%ld\n", blockPtr->statsExit(d1, d2));
                    success++;
                }
            }
            else if (counter == 5) //nai basei country
            {
                std::string countryName(comms[4]);
                block *blockPtr = diseaseHT.search(dis);
                if (blockPtr == NULL)
                {
                    std::cout << dis << " 0\n";
                    success++;
                }
                else
                {
                    fprintf(stdout, "%ld\n", blockPtr->statsExitC(d1, d2, countryName));
                    success++;
                }
            }
        }
        else
        {
            //std::cerr << "Unknown Command!\n"; //doesn't exit the program, gives the user another chance to type properly this time.
            std::cerr << "error\n";
            failed++;
        }
        delete[] buf; //just in case worker ceases for no reason
    }                 //end while(1)
}