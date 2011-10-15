default:
	g++ program.cpp motorControl.cpp control.cpp interfaceKit.cpp sensing.cpp phidget.cpp strategy.cpp util.cpp -o program -lphidget21 -lrt -lpowerbutton -D SILENT -D SILENT_STRATEGY -g -Wall -pedantic -std=c++0x
	
test:
	g++ test.cpp motorControl.cpp control.cpp interfaceKit.cpp sensing.cpp phidget.cpp strategy.cpp -o test -lphidget21 -lrt -lpowerbutton -std=c++0x
	
clean:
	rm -f *.o
