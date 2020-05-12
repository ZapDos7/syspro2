#!/bin/bash

#use: ./create_inflies.sh diseasesFile countriesFile inputdir numFilesPerDirectory numRecordsPerFile

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
#while IFS= read -r line
#do
  #echo "$line"
#done < "$1"
#while IFS= read -r line
#do
  #echo "$line"
#done < "$2"

#make subdirs for each country
#make in these subdirs $4 named DD-MM-YYYY (days 1 ws 30, month 1-12 kai years ok)



#diseasesFile="$1"
#countriesFile = $2
#inputdir = $3
#numFilesPerDirectory = $4
#numRecordsPerFile = $5