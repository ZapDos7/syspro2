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
                        if (help[1]=="ENTRY") //exoume eisodo
                        {
                            wannabe+=entry->d_name; //wannabe="ID Fname Lname Disease Country Age EntryDate"
                            wannabe+=" ";
                            record temp_r(wannabe); //temp r
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
            //i=0;
        } //prepei na mpikan ola sta hash table, we gonna check now!
        //epanamhdenizw ton arithmo
        posa_arxeia=0;
    }
    return 0;
}