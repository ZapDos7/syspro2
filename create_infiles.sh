#!/bin/bash

#use: ./create_inflies.sh diseasesFile countriesFile inputdir numFilesPerDirectory numRecordsPerFile

random-string()
{
    cat /dev/urandom | tr -dc 'a-z0-9' | fold -w ${1:-32} | head -n 1
}

random-name()
{
    cat /dev/urandom | tr -dc 'a-z' | fold -w ${1:-32} | head -n 1
}

if [ $# -ne 5 ];
then
    echo "Please provide with 5 proper arguements."
    exit -1
else
    echo "Total arguments : $#"
fi

diseasNum=`wc -l "$1"`
countrNum=`wc -l "$2"`

echo $diseasNum #21 diseaseFile.txt
echo $countrNum #184 countriesFile.txt

mkdir -p "$3"

#IDs
ARRAY=()

#read files
FLOOR=1900
counter=$4
counter2=$5
while IFS= read -r line
do
    #echo "$line" #gia kathe grammi ftiakse ena folder
    mkdir -p "$3/$line" #make subdirs for each country
    #den xreiazetai na elegxoume an uparxei to directory giati to country file den exei diplotupa onomata xwrwn
    #an thelame, elegxoume me to if [[ -d $3/$line ]]; then ...
    #ftiakse $4 files edw mesa
    while [ $counter -gt 0 ] #5 4 3 2 1
    do
        #echo "$counter"
        day=$(( ( RANDOM % 30 )  + 1 )) #1 ws 30
        month=$(( ( RANDOM % 12 )  + 1 )) #1 ws 12
        year=0 #init
        while [ "$year" -le $FLOOR ]
        do
            year=$(( ( RANDOM % 2020 + 1 ) )) #1900-2020
        done #end while gia na exoume xronies > 1900 just so stuff seem to make sense & have 4 digits
        FILE=$3/$line/$day-$month-$year
        if [[ -f "$FILE" ]]; then
            echo "$FILE exists"
            #in order to skip the while, we init counter2 as 0 so it never runs
            counter2=0
        else
            touch "$3/$line/$day-$month-$year" #legontai DD-MM-YYYY
        fi
        while [ $counter2 -gt 0 ] #10 9 8 7 ...
        do
            rand=$[ $RANDOM % 2 ]
            id="$(random-string 5)"
            if [ $rand -eq 0 ] #exoume entry
                #tuxaio string me arithmous kai noumera - 5 xaraktires
                while [ [ ${arr[*]} =~ id ] ] #while ID is in array
                do
                    id="$(random-string 5)" #generate new id
                done #while gia elegxo unique ID
            else #exoume exit ara prepei na uparxei idi to ID
                while [ ! [ ${arr[*]} =~ id ] ] #while ID is not in array
                do
                    id="$(random-string 5)" #generate new id
                done #while gia elegxo existing ID
            fi
            
            ARRAY+=('id') #used IDs get here to we only get unique ones throughout the iterations for all countries
            #enter i exit:
            arr[0]="ENTER"
            arr[1]="EXIT" #gia na exei exit prepei na uparxei allou enter!!!
            second=${arr[$rand]}
            #tuxaio string mono xaraktirwn megethous 1 ews 10
            fname="$(random-name "$(( ( RANDOM % 10 )  + 1 ))")"
            lname="$(random-name "$(( ( RANDOM % 10 )  + 1 ))")"
            #random astheneia apo to $1
            disease=$( shuf -n 1 $1 )
            #age 0 ews 120
            age=$(( ( RANDOM % 120 )  + 1 ))
            echo -n "$id" >> $3/$line/$day-$month-$year
            echo -n " " >> $3/$line/$day-$month-$year
            echo -n "$second" >> $3/$line/$day-$month-$year
            echo -n " " >> $3/$line/$day-$month-$year
            echo -n "$fname" >> $3/$line/$day-$month-$year
            echo -n " " >> $3/$line/$day-$month-$year
            echo -n "$lname" >> $3/$line/$day-$month-$year
            echo -n " " >> $3/$line/$day-$month-$year
            echo -n "$disease" >> $3/$line/$day-$month-$year
            echo -n " " >> $3/$line/$day-$month-$year
            echo -n "$age" >> $3/$line/$day-$month-$year
            echo " " >> $3/$line/$day-$month-$year
            #sto teleutaio bazw to new line
            counter2=$(( $counter2 - 1 ))
        done #while gia record
        #reset counter2!!
        counter2=$5
        counter=$(( $counter - 1 ))
    done #while file
    #reset counter!!
    counter=$4
done < "$2" #while xwres


#diseasesFile="$1"
#countriesFile = $2
#inputdir = $3
#numFilesPerDirectory = $4
#numRecordsPerFile = $5