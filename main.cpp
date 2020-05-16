/*	
Ioanna Zapalidi, sdi1400044	
System Programming Project #2, Spring 2020	
 */	
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include "date.h" //my date class
#include "ht.h" //hash table - diki mas domi
#include "aht.h" //"advanced" hash table
#include "tree.h" //bst
#include "record.h" //record class
#include "bb.h" //blocks and buckets
#include "heap.h"

//shmeiwseis mathimatos
void do_ls ( char dirname []) //displays all directories that can be opened
{
    DIR * dir_ptr ;
    struct dirent * direntp ;
    if ( ( dir_ptr = opendir ( dirname ) ) == NULL )
        fprintf ( stderr , " cannot open %s \n " , dirname ) ;
    else {
        while ( ( direntp = readdir ( dir_ptr ) ) != NULL )
        if ((strcmp(direntp->d_name, ".")!=0)||(strcmp(direntp->d_name, "..")!=0))
            printf ( "%s\n " , direntp->d_name );
        else printf("I'm dot(s).\n");
        closedir ( dir_ptr ) ;
    }
}

bool date_format(std::string str) //elegxei oti ena string einai stin morfi XX-YY-ZZZZ
{
    std::string a0="";
    //elegxw oti ta comms 1, 2 einai ok gia na perastoun ws orismata ston constructor twn dates	
    for (unsigned int j = 0; j < str.length(); j++) //trekse to mikos tou orismatos	
    {	
        if ( ( str[j]-'0' >-1 ) && ( str[j]-'0' <10 ) ) //it's a digit	
        {
            a0 = a0 + str[j];
        }
        else if (str[j] == '-')
        {
            a0 = a0 + str[j];
        }
        else
        {
            //std::cerr << "Type properly(1).\n";	 //continue to next command
            std::cout << "error\n";
        }
    }
    char * cstr = new char[a0.length() + 1]; //auto 8a kanw tokenize
    strcpy(cstr, a0.c_str()); //copy as string to a0 sto cstr
    char * pch;
    const char delim[2] = "-";
    pch = strtok(cstr, delim);
    short unsigned int counter_a = 0;
    while (pch != NULL)
    {
        counter_a++;
        pch = strtok(NULL, delim);
    }
    delete[] cstr;
    if (counter_a!=3)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int main(int argc, char const *argv[])
{
    /////////////////////////////////////
    char in_dir[256]; //input directory
    int w = -1; //number of workers
    int b = -1; //bufferSize	
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
    if ((w < 0) || (b < 0)) {
        //std::cerr << "No values for the parameters, please execute again properly.\n";
        std::cerr << "error\n";
        exit(-1);	
    }

    do_ls(in_dir); //doesn't skip "." and ".."
    //just a worker
    //read all subfolders in input directory
    //save all records chronologically in hash table:
        //read each line
        //if ENTRY, make a string "ID Fname Lname Disease Country(from directory) entryDate(from directory) -"
        //if same ID, EXIT --> update exit date in the hash table saved record
    //prepare structurs for answers to the commands




    std::ifstream dataset(in_dir); //dataset: recordID FName LName diseaseID Country entryDate exitDate	
    std::string line;	
    unsigned int records_num = 0;	
    while (std::getline(dataset, line))	
    {	
        records_num++;	
    }
    ht my_ht(records_num * 3 / 4); //ht size = 0.75 * (plithos records from file) => not the worst seek time (project algorithmika)	
    aht diseaseHT(records_num * 3 / 4, 1024);
    aht countryHT(records_num * 3 / 4, 1024);	
    //reread file and actually keep info	
    dataset.clear();	
    dataset.seekg(0);	
    while (std::getline(dataset, line)) {	
        record temp_r(line); //temp r	
        record * elegxos = my_ht.insert(&temp_r); //edw ginetai kai elegxos gia unique IDs	
        if (elegxos == NULL)	
        {	
            break; //sto piazza eipw8ike oti an brethei kapoio ID duplicate, na proxwrame stis entoles & na mhn sunexizoun ta insertions.	
        }	
        else	
        {	
            diseaseHT.ainsert(elegxos, false);	
            countryHT.ainsert(elegxos, true);	
        }	
    }
    std::string com; //command

    //std::cout << "Enter desired function:\n";
    //while (1) { //an den dinei apo file
    while (std::getline(std::cin, com)) { //aposxoliase otan bgaloun output format
        //std::cout << "Enter desired function:\n";                                     //an den dinei apo file
        //std::getline(std::cin, com); //std::cin >> com; doesn't work due to spaces    //an den dinei apo file
        if (com.length() == 0)
        {	
            continue; //ama m dwseis enter, sunexizw na zhtaw entoles	
        }	
        char * cstr = new char[com.length() + 1]; //auto 8a kanw tokenize	
        strcpy(cstr, com.c_str()); //copy as string to line sto cstr	
        char * pch;	
        const char delim[2] = " ";	
        pch = strtok(cstr, delim);	
        std::string comms[8]; //i entoli me ta perissotera orismata einai h insertPatientRecord me d2	
        int counter = 0;	
        comms[0] = pch;	
        //check first word to match with command, check entire command if correct	
        if (comms[0] == "/listCountries") // /listCountries --> for each country print PID of corresponding worker
        {
            std::cerr << "I am List Countries!\n";
            //lala
        }
        else if (comms[0] == "/exit")	
        {	
            //workers -> log files
            //free memory again as needed
            delete[] cstr;
            std::cout << "exiting\n";
            return 0;	
        }	
        else if (comms[0] == "/diseaseFrequency") //8. /diseaseFrequency virusName date1 date2 [country]	
        {	
            //same but make it for each worker in the future
            while (pch != NULL)
            {	
                comms[counter] = pch;	
                counter++;	
                pch = strtok(NULL, delim);	
            }
            std::string virusName = comms[1];
            std::string wannabedate1 = comms[2];
            std::string wannabedate2 = comms[3];
            if ((date_format(wannabedate1)==false)||(date_format(wannabedate2)==false))
            {
                //std::cerr << "Type properly.(6)\n";
                std::cerr << "error\n";
                break;
            }
            //else
            date d1(wannabedate1);
            date d2(wannabedate2);
            if (isLater(d1,d2)==-1) //an d1>d2, epistrefei -1
            {
                //std::cerr << "Type properly.(7)\n";
                std::cerr << "error\n";
                break;
            }
            //else ok
            block * apantisi = diseaseHT.search(virusName);
            if (apantisi==NULL) //mou zitas na brw kati pou den exw sti vasi m
            {
                //std::cout << "Disease named " << virusName << " has 0 records.\n";"0\n"; //if not exists, return 0
                //else
                std::cerr << "error\n";
            }
            else //to brika
            {//An oxi country orisma, gia kathe country, print posa Virus metaksu twn 2 dates
                if (counter==4) //posa virusName krousmata anamesa sto date1 date2
                {//den exw country
                    //std::cout << /*"Disease named " << virusName << " has "" " << apantisi->stats(d1,d2) << /*" records.\n";"\n";
                }
                else if(counter==5) //exei country
                {//An nai, mono gi auto to country print posa Virus metaksu twn 2 dates
                    std::string cntrName = comms[4];
                    std::cout << /*"Disease named " <<*/ virusName <</* " has " " "<< */apantisi->statsC(d1, d2, cntrName) /*<< " records for country named " << cntrName << ".\n";*/ << "\n";
                }
                else //mou edwses alla m nt alla
                {
                    //std::cerr << "Type properly.(8)\n";
                    std::cerr << "error\n";
                }
                
            }
        }	
        else if (comms[0] == "/topk-Diseases") //10. /topk-Diseases k country [date1 date2]	
        {//Gia to country, which k viruses are top (most krousmata) [between dates if given]
            //same but make it for each worker in the future
            while (pch != NULL)
            {	
                comms[counter] = pch;	
                counter++;	
                pch = strtok(NULL, delim);	
            }
            int k = atoi(comms[1].c_str());
            //std::cerr << k <<"\n";
            std::string countryName = comms[2];
            //std::cerr << countryName <<"\n";
            //pame stin entoli:
            block * b = countryHT.search(countryName);
            //b->print_blk(true);
            if (b==NULL)
            {
                //std::cerr << countryName << " has no records.\n";
                std::cerr << "error\n"; //else print
                //std::cerr << countryName << " 0\n";
            }
            else
            {
                if (counter==3)
                {
                    b->top_k_diseases(k);
                }
                else if (counter==5)
                {
                    //std::cerr << "exw dates!\n";
                    std::string wannabedate1 = comms[3];
                    std::string wannabedate2 = comms[4];
                    if ((date_format(wannabedate1)==false)||(date_format(wannabedate2)==false))
                    {
                        //std::cerr << "Type properly.(6)";
                        std::cerr << "error\n";
                        break;
                    }
                    //else dates are ok
                    date d1(wannabedate1);
                    date d2(wannabedate2);
                    if (isLater(d1,d2)==-1) //an d1>d2, epistrefei -1
                    {
                        //std::cerr << "Type properly.(7)";
                        std::cerr << "error\n";
                        break;
                    }//else dates are ok ok ara kaloume
                    b->top_k_diseases(k, d1, d2);
                }
                else
                {
                    std::cerr << "error\n";
                }
                
            }
        }
        else if (comms[0] == "/topk-AgeRanges")
        {
            std::cerr << "I am topk age ranges!\n";
            //  /topk-AgeRanges k country disease d1 d2 --> age range & pososta
        }
        else if (comms[0] == "/searchPatientRecord")
        {
            std::cerr << "I am search patient record!\n";
            //  /searchPatientRecord recordID
            //o Aggr stelnei se olous tous workers kai perimenei apantisi
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
    } //end while(1)
    return 0;	
}