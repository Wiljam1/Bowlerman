make clean_windows
make UDPserver
make windows
start UDPserver.exe        
:: ^START GÖR ATT DEN GÅR VIDARE TILL NÄSTA "RAD" MEDAN .EXE-FILEN KÖRS
main.exe                   
:: ^INGEN START BETYDER ATT DEN VÄNTAR MED NÄSTA RAD TILLS MAN STÄNGT NER .EXE FILEN
taskkill /im UDPserver.exe 
:: ^STÄNGER NER SERVERN EFTER MAN STÄNGT SIN CLIENT