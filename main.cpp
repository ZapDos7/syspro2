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
        fprintf ( stderr , " cannot open % s \ n " , dirname ) ;
    else {
        while ( ( direntp = readdir ( dir_ptr ) ) != NULL )
        printf ( " % s\ n " , direntp -> d_name ) ;
        closedir ( dir_ptr ) ;
    }
}

bool date_format(std::string); //elegxei oti ena string einai stin morfi XX-YY-ZZZZ

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
    //just a worker
    //read all subfolders in input directory
    //save all records chronologically in hash table:
        //read each line
        //if ENTRY, make a string "ID Fname Lname Disease Country(from directory) entryDate(from directory) -"
        //if same ID, EXIT --> update exit date in the hash table saved record
    //prepare structurs for answers to the commands



    /*
    std::ifstream dataset(in_dir); //dataset: recordID FName LName diseaseID Country entryDate exitDate	
    std::string line;	
    unsigned int records_num = 0;	
    while (std::getline(dataset, line))	
    {	
        records_num++;	
    }	
    ht my_ht(records_num * 3 / 4); //ht size = 0.75 * (plithos records from file) => not the worst seek time (project algorithmika)	
    aht diseaseHT(h1, b);	
    aht countryHT(h2, b);	
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
        if (comms[0] == "/insertPatientRecord") //1. /insertPatientRecord recordID patientFirstName patientLastName diseaseID entryDate [exitDate]	
        {//Eisagoume nea eggrafi (exit date may be missiing)	
            while (pch != NULL) {	
                //std::cerr << pch;	
                comms[counter] = pch;	
                counter++;	
                pch = strtok(NULL, delim);	
            }	
            record r1;	
            r1.set_id(comms[1]);	
            r1.set_fname(comms[2]);	
            r1.set_lname(comms[3]);	
            r1.set_disease(comms[4]);	
            r1.set_country(comms[5]);	
            r1.set_entryD(comms[6]);	
            if (counter == 8) //uparxei date 2	
            {	
                if (comms[7]=="-")//edwses date2 mia paula
                {
                    r1.set_exitD("-");
                }
                else//edwses kanoniko date2
                {
                    r1.set_exitD(comms[counter - 1]);
                }
            }	
            else//date2 omited --> ennoeitai paula
            {	
                r1.set_exitD("-");
            }	
            r1.print_record();	
            record * elegxos2 = my_ht.insert(&r1); //edw ginetai kai elegxos gia unique IDs	
            if (elegxos2 == NULL)	
            {	
                //std::cerr << "Record error. Exiting.\n";
                std::cerr << "error\n";
            }	
            else	
            {	
                diseaseHT.ainsert(elegxos2, false);	
                countryHT.ainsert(elegxos2, true);
                std::cout << "Record added\n";
                //std::cerr << "Inserted: ";	
                //r1.print_record();	
                //std::cerr << std::endl;	
            }
        }	
        else if (comms[0] == "/recordPatientExit") //3. /recordPatientExit recordID exitDate //Add exit Date to this record	
        {	
            //e.g.: recordPatientExit 47 06-04-2021 //epistrefei already has denexit date	
            //e.g.: recordPatientExit 89 06-04-2021 //epistrefei ok	
            //e.g.: recordPatientExit 89 06-04-1500 //epistrefei oti den eisai o Doctor	
            while (pch != NULL)	
            {	
                comms[counter] = pch;	
                counter++;	
                pch = strtok(NULL, delim);	
            }	
            //std::cerr << "Psaxnw to " << comms[1] << "\n";
            ht_item * h = my_ht.search(comms[1]);	
            if (h == NULL)	
            {	
                //std::cerr << "Can't update this record as it doesn't exist.\n";
                std::cout << "Not found\n";
            }	
            else	
            {	
                h->rec->print_record();
                date d2(comms[2]); //d2 = ti paw na valw
                date *d1 = h->rec->get_exitDatePtr(); //to uparxon exit date
                bool magkas = d1->set; //ama eixa prin set=true ara eixa idi exit date, true, else false
                //std::cerr << "magkas " << magkas << "\n";
                date *din = h->rec->get_entryDatePtr(); //to entry date
                {
                    if (isLater(d2, *din) == 1) //pas na mou baleis kati pou den einai later tou entry date m
                    {	
                        //std::cerr << "Entry date later than desired exit date. You're not The Doctor.\n";
                        std::cerr << "error\n";
                    }	
                    else	
                    {	
                        h->rec->set_exitD(d2.get_date_as_string()); //twra exei exit date
                        if (magkas==false) //prin den imoun set ara update counters:
                        {
                            block * blokaki1 = diseaseHT.search(comms[1]);
                            if (blokaki1!=NULL)
                            {
                                blokaki1->update_c_in(false);
                            }
                            block * blokaki2 = countryHT.search(comms[1]);
                            if (blokaki2!=NULL)
                            {
                                blokaki2->update_c_in(false);
                            }
                        }
                        //eidallws oi metrites den allazoun!
                        //std::cerr << "Updated record: " << comms[1] << " with exit date: " << h->rec->get_exitDate().get_date_as_string() << "\n";
                        std::cout << "Record updated\n";
                    }	
                //}
                else //den eixe paulitsa ara itan set	
                {	
                    std::cerr << "This record alread has an exit date.\n";
                //}//auto kai to apopanw sxoliasmena (an einai == "-")	aposxoliazontai an den theloume na allazoume uparxon exit date, alla edw akolouthoume tis odigies sto piazza
            }	
        }	
        else if (comms[0] == "/exit")	
        {	
            delete[] cstr;
            std::cout << "exiting\n";
            return 0;	
        }	
        else if (comms[0] == "/numCurrentPatients") //4. /numCurrentPatients [disease]	
        {	
            //eg: numCurrentPatients SARS-1	
            //an dw8ei to [disease] print posoi patients exoun auto to disease AKOMA (exitDate based)	
            while (pch != NULL)	
            {	
                comms[counter] = pch;	
                counter++;	
                pch = strtok(NULL, delim);	
            }	
            if (counter > 1) //exoume 2o orisma	
            {	
                std::string dis(comms[1]);	
                block * blockPtr = diseaseHT.search(dis);	
                if (blockPtr == NULL)	
                {
                    std::cout << dis <<" 0\n";
                    //std::cout << "0\n"; //den eixame kanena krousma
                }	
                else	
                {	
                    blockPtr->print_blk(false);	
                }	
            }	
            else //den exoume 2o orisma	
            { //if not, print posoi patient exoun kathe disease AKOMA - gia KATHE disease ara peridiavenw to diseaseHT	
                for (int i = 0; i < h1; i++) //gia kathe bucket* sto hash table	
                {	
                    diseaseHT.get_table()[i].print_bkt(false);	
                }	
            }	
        }	
        else if (comms[0] == "/globalDiseaseStats") //6. /globalDiseaseStats [date1 date2]	
        {	
            while (pch != NULL)
            {	
                comms[counter] = pch;	
                counter++;	
                pch = strtok(NULL, delim);	
            }	
            if (counter==1)
            {	
                for (int i = 0; i < h1; i++) //gia kathe bucket* sto hash table	
                {	
                    diseaseHT.get_table()[i].print_bkt_all(false);
                }	
            }  //an uparxei date1 prepei na uparxei date2, alla mporei na leipoun kai ta 2
            else if(counter==3) //0 = globalDiseaseStats, 1 = d1, 2 = d2	
            {
                if ((date_format(comms[1])==false)||(date_format(comms[2])==false))
                {
                    //std::cerr << "Format dates as: XX-YY-ZZZZ or X-Y-Z.\n";
                    std::cerr << "error\n";
                    break;
                }

                date date1(comms[1]);
                date date2(comms[2]);
                
                if ((isLater(date1, date2)==-1)||(isLater(date1, date2)==0))	//an mou dwseis date1 > date 2 akuro, an tautizontai episis giati den exw diastima honey
                {
                    //std::cerr << "Type properly(3).\n";
                    std::cerr << "error\n";
                }	
                else	
                {	
                    diseaseHT.global_stats(date1, date2);
                }
            }	
            else	
            {	
                //std::cerr << "Type properly(4).\n";	
                std::cerr << "error\n";
            }
        }	
        else if (comms[0] == "/diseaseFrequency") //8. /diseaseFrequency virusName date1 date2 [country]	
        {	
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
                //std::cerr << "error\n";
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
                    std::cout << /*"Disease named " << virusName <</* " has " " "<< apantisi->statsC(d1, d2, cntrName) << " records for country named " << cntrName << ".\n"; "\n";
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
        else if (comms[0] == "/topk-Countries") //11. /topk-Countries k disease [date1 date2]
        {	
            //Gia to virus, which k countries are top (most krousmata) [between dates if given] //an uparxei date1 prepei na uparxei date2, alla mporei na leipoun kai ta 2	
            while (pch != NULL)
            {	
                comms[counter] = pch;	
                counter++;	
                pch = strtok(NULL, delim);	
            }
            int k = atoi(comms[1].c_str());
            //std::cerr << k <<"\n";
            std::string diseaseName = comms[2];
            //std::cerr << diseaseName <<"\n";
            //pame stin entoli:
            block * b = diseaseHT.search(diseaseName);
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
                    b->top_k_countries(k);
                }
                else if (counter==5)
                {
                    //std::cerr << "exw dates!\n";
                    std::string wannabedate1 = comms[3];
                    std::string wannabedate2 = comms[4];
                    if ((date_format(wannabedate1)==false)||(date_format(wannabedate2)==false))
                    {
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
                    b->top_k_countries(k, d1, d2);
                }
                else
                {
                    std::cerr << "error\n";
                }
                
            }
        }	
        else	
        {	
            //std::cerr << "Unknown Command!\n"; //doesn't exit the program, gives the user another chance to type properly this time.	
            std::cerr << "error\n";
        }	
        delete[] cstr; //just in case	
    } //end while(1)
    */	
    return 0;	
}

bool date_format(std::string str)
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