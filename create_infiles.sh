#!/bin/bash

#use: ./create_inflies.sh diseasesFile countriesFile inputdir numFilesPerDirectory numRecordsPerFile

random-string()
{
    cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w ${1:-32} | head -n 1
}

random-name()
{
    cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w ${1:-32} | head -n 1
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

#read files
FLOOR=1900
counter=$4
counter2=$5
while IFS= read -r line
do
    #echo "$line" #gia kathe grammi ftiakse ena folder
    mkdir -p "$3/$line" #make subdirs for each country
    #ftiakse $4 files edw mesa
    while [ $counter -gt 0 ] #5 4 3 2 1
    do
        #echo "$counter"
        day=$(( ( RANDOM % 30 )  + 1 )) #1 ws 30
        month=$(( ( RANDOM % 12 )  + 1 )) #1 ws 12
        year=0 #init
        while [ "$year" -le $FLOOR ]
        do
            year=$(( ( RANDOM % 2050 + 1 ) ))
        done #end while gia na exoume xronies > 1900 just so stuff seem to make sense
        touch "$3/$line/$day-$month-$year" #legontai DD-MM-YYYY
        while [ $counter2 -gt 0 ] #10 9 8 7 ...
        do
            #tuxaio string me arithmous kai noumera - 5 xaraktires
            id="$(random-string 5)"
            #enter i exit:
            arr[0]="ENTER"
            arr[1]="EXIT" #gia na exei exit prepei na uparxei allou enter!!!
            rand=$[ $RANDOM % 2 ]
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