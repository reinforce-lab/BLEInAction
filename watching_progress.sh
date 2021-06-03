#!/bin/sh
cd /Users/u_akihiro/Desktop/Books/BLEInAction
date '+%F' | tr -d "\n"                             >> progress_data.csv
echo ','   | tr -d "\n"                             >> progress_data.csv
wc *re | tail -n 1 | tr -s ' ' | cut -d ' ' -f4     >> progress_data.csv

osascript progress_data.scpt 
