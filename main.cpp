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
#include <unistd.h>

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
//uparxei i string swap-> str1.swap(str2); arkei gia na antallaksoun periexomena 2 strings
int partition (std::string arr[], int low, int high) 
{ 
    std::string pivot = arr[high];    // pivot 
    int i = (low - 1);  // Index of smaller element 
  
    for (int j = low; j <= high- 1; j++) 
    { 
        date d1(arr[j]);
        date d2(pivot);
        // If current element is smaller than or equal to pivot 
        if ( isLater(d1,d2)>-1 )
        { 
            i++;    // increment index of smaller element
            arr[i].swap(arr[j]);
        } 
    } 
    arr[i+1].swap(arr[high]);
    return (i + 1); 
}

void quickSort(std::string arr[], int low, int high) 
{ 
    if (low < high) 
    {
        int pi = partition(arr, low, high); //partitioning index, to pi einai sti swsti thesi tou tr
        //kanw quisort gia ta prin kai gia ta meta xwria
        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high); 
    } 
} 

int main(int argc, char const *argv[])
{
    //Anagnwsi params
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
        std::cerr << "error\n";
        return -1;	
    }    

    //arxikopoiisi hash tables
    ht my_ht(w*10); //yolo size
    aht diseaseHT(w*10, 1024);
    aht countryHT(w*10, 1024);
    
    //directory metablites
    DIR *dir; struct dirent *entry; //xwra
    DIR *dir2; struct dirent *entry2; //imerominia
    
    int posa_arxeia = 0; //posa date arxeia exei mesa to folder gia mia xwra
    
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
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            snprintf(path, sizeof(path), "%s/%s", in_dir, entry->d_name); //path = "../folder2/Xwra"
            //pame ena level mesa tr ara open to path
            dir2 = opendir(path);
            if (dir2 == NULL)
            {
                std::cerr << "error opening input subdirectory\n";
                return -2;
            }
            while ((entry2 = readdir(dir2)) != NULL)
            {
                if (entry2->d_type == DT_DIR) //if it is a directory, access
                {
                    if (strcmp(entry2->d_name, ".") == 0 || strcmp(entry2->d_name, "..") == 0) continue;
                    std::cerr <<"error\n"; //den prepei na tupwsei tpt gt exoume 1 level subdir mono oxi parapanw!
                }
                else
                {
                    //std::cerr << entry2->d_name << " ";
                    posa_arxeia++;
                }
            }
            closedir(dir2);
            //twra ftiaxnw pinaka
            std::string date_file_names[posa_arxeia];

            //twra ksananoigw gia na valw ta file names edw mesa
            dir2 = opendir(path);
            if (dir2 == NULL)
            {
                std::cerr << "error opening input subdirectory\n";
                return -2;
            }
            //else
            unsigned int i = 0;
            while ((entry2 = readdir(dir2)) != NULL)
            {
                if (entry2->d_type == DT_DIR) //if it is a directory, access
                {
                    if (strcmp(entry2->d_name, ".") == 0 || strcmp(entry2->d_name, "..") == 0) continue;
                }
                else
                {
                    date_file_names[i] = entry2->d_name; //vazw to entry2->d_name sto array
                    i++;
                }
            }
            closedir(dir2);


            //sort ta pramata mes to array
            
            int megethos = sizeof(date_file_names)/sizeof(date_file_names[0]); //posa exei mesa to array
            quickSort(date_file_names,0,megethos-1);
            /*for (int i1 = 0; i1 < megethos; i1++)
            {
                std::cerr << date_file_names[i1] << '\t';
            }
            std::cerr << "\n";
            */

            //std::cerr << path << "\n"; // /dir/Xwra
            dir2 = opendir(path);
            if (dir2 == NULL)
            {
                std::cerr << "error opening input subdirectory\n";
                return -2;
            }
            //else
            for (int i1 = 0; i1 < megethos; i1++)
            {
                //std::cerr << date_file_names[i1] << '\t';
                std::string path2 = path;
                path2+='/';
                path2+=date_file_names[i1];
                //std::cerr << path2 << "\n\n";  // /dir/Xwra/XX-YY-ZZZZ
                dataset.open(path2);
                if (dataset.is_open()==0) std::cerr << "error\n";
                else //we gonna do the thingggg
                {
                    std::string line;
                    while (std::getline(dataset, line))	 //diavazei olo to this date --> we gotta do it for all files in this folder
                    {	
                        //line read: ID ENTER fn ln disease age
                        //constructor gia record: ID FName Lname DIsease Country age entryD exitD/-
                        //std::cerr << "paw na valw " << line <<"\n";
                        std::string wannabe="";
                        std::string help[6]; //boithitiki domi me ta tokens tou line pou 8a ftiaksw swsta
                        int counter = 0;
                        const char* c_string = line.c_str();
                        char* token = strtok((char*)c_string, " ");
                        while (token) {
                            //std::cerr << token << '\n';
                            help[counter] = token;
                            token = strtok(NULL, " ");
                            counter++;
                        }
                        wannabe+=help[0]; //wannabe="ID"
                        wannabe+=" ";
                        wannabe+=help[2]; //wannabe="ID Fname"
                        wannabe+=" ";
                        wannabe+=help[3]; //wannabe="ID Fname Lname"
                        wannabe+=" ";
                        wannabe+=help[4]; //wannabe="ID Fname Lname Disease"
                        wannabe+=" ";
                        wannabe+=entry->d_name; //wannabe="ID Fname Lname Disease Country"
                        wannabe+=" ";
                        wannabe+=help[5]; //wannabe="ID Fname Lname Disease Country Age"
                        wannabe+=" ";
                        if (help[1]=="ENTER") //exoume eisodo
                        {
                            wannabe+=date_file_names[i1];

                            //record lala("ntber xwctbce jyjzwsg Salmonella Sudan 74 18-6-1945");
                            //lala.print_record();
                            //if (wannabe!="ntber xwctbce jyjzwsg Salmonella Sudan 74 18-6-1945") std::cerr << "ma pws?\n";
                            //else std::cerr << "ok\n";
                            record temp_r(wannabe); //temp record gia insert

                            record * elegxos = my_ht.insert(&temp_r); //edw ginetai kai elegxos gia unique IDs
                            if (elegxos == NULL)
                            {
                                std::cerr << "den evala to " << elegxos->get_id() << "\n";
                                break; //sto piazza eipw8ike oti an brethei kapoio ID duplicate, na proxwrame stis entoles & na mhn sunexizoun ta insertions.
                            }
                            else
                            {
                                std::cerr << "evala to " << elegxos->get_id() << "\n";
                                diseaseHT.ainsert(elegxos, false);
                                countryHT.ainsert(elegxos, true);
                            }
                        }
                        else if (help[1]=="EXIT")
                        {
                            ht_item * h = my_ht.search(help[0]); //bres an uparxei to record me auto to ID
                            if (h==NULL)
                            {
                                continue; //oops this record doesn't exist, akuro, bye
                            }
                            else
                            {
                                date d2(entry->d_name); //d2 = ti paw na valw
                                date *d1 = h->rec->get_exitDatePtr(); //to uparxon exit date
                                bool magkas = d1->set; //ama eixa prin set=true ara eixa idi exit date, true, else false
                                date *din = h->rec->get_entryDatePtr(); //to entry date
                                if (isLater(d2, *din) == 1) //pas na mou baleis kati pou den einai later tou entry date m
                                {
                                    std::cerr << "error\n";
                                }
                                else //valid basei entry date
                                {	
                                    h->rec->set_exitD(d2.get_date_as_string()); //twra exei exit date
                                    if (magkas==false) //prin den imoun set ara update counters:
                                    {
                                        block * blokaki1 = diseaseHT.search(entry->d_name);
                                        if (blokaki1!=NULL)
                                        {
                                            blokaki1->update_c_in(false);
                                        }
                                        block * blokaki2 = countryHT.search(entry->d_name);
                                        if (blokaki2!=NULL)
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
                }//diavase to epomeno arxeio tr
            } //end for gia chronological
            closedir(dir2);
            //i=0; //de xreiazetai gt midenizetai mes tin loopa tou
        } //prepei na mpikan ola sta hash table, we gonna check now!
        //epanamhdenizw ton arithmo arxeiwn giati paw se alli xwra
        posa_arxeia=0;
    }

    
    //commands
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
            //
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
                    std::cout << virusName << " " << apantisi->stats(d1,d2) << "\n";
                }
                else if(counter==5) //exei country
                {//An nai, mono gi auto to country print posa Virus metaksu twn 2 dates
                    std::string cntrName = comms[4];
                    std::cout << virusName << " " << apantisi->statsC(d1, d2, cntrName) << "\n";
                }
                else //mou edwses alla m nt alla
                {
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
            //age ranges: 0-20, 21-40, 41-60, 60+
            //  /topk-AgeRanges k country disease d1 d2 --> age range & pososta
        }
        else if (comms[0] == "/searchPatientRecord")
        {
            //  /searchPatientRecord recordID
            //o Aggr stelnei se olous tous workers kai perimenei apantisi

            while (pch != NULL) //kovw tin entoli sta parts tis
            {	
                comms[counter] = pch;	
                counter++;	
                pch = strtok(NULL, delim);	
            }
            if (counter!=2) std::cerr << "error1\n";
            else
            {
                ht_item * anazitisis = my_ht.search(comms[counter-1]);
                if (anazitisis==NULL) std::cerr << "error2\n";
                else
                {
                    anazitisis->print_ht_item();
                }
                
            }
            
            //search vasei ID
            //return to record olo printed
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