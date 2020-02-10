#!/bin/sh
wc *re | tail -n 1 | tr -s ' ' | cut -d ' ' -f4 
