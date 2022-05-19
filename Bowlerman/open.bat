start udpServer.exe        
:: ^START GÖR ATT DEN GÅR VIDARE TILL NÄSTA "RAD" MEDAN .EXE-FILEN KÖRS
start tcpServer.exe                 
:: ^INGEN START BETYDER ATT DEN VÄNTAR MED NÄSTA RAD TILLS MAN STÄNGT NER .EXE FILEN
start main.exe    
main.exe                
taskkill /im udpServer.exe 
taskkill /im tcpServer.exe
taskkill /im main.exe 
:: ^STÄNGER NER SERVERN EFTER MAN STÄNGT SIN CLIENT