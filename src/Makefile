default:
	g++ program.cpp motorControl.cpp control.cpp interfaceKit.cpp sensing.cpp phidget.cpp servoControl.cpp strategy.cpp dances.cpp util.cpp -o program -lphidget21 -lrt -lpowerbutton -D SILENT -D SILENT_STRATEGY -Wall -pedantic -std=c++0x
	
test:
	g++ test.cpp motorControl.cpp control.cpp interfaceKit.cpp sensing.cpp phidget.cpp strategy.cpp dances.cpp util.cpp -o test -lphidget21 -lrt -lpowerbutton -D SILENT -D SILENT_STRATEGY -std=c++0x
	
clean:
	rm -f *.o
