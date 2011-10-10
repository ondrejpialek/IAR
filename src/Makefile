default:
	g++ program.cpp motorControl.cpp control.cpp interfaceKit.cpp sensing.cpp phidget.cpp strategy.cpp -o program -lphidget21 -lrt -lpowerbutton -D SILENT -g
	
test:
	g++ test.cpp motorControl.cpp control.cpp interfaceKit.cpp sensing.cpp phidget.cpp strategy.cpp -o test -lphidget21 -lrt -lpowerbutton
	
clean:
	rm -f *.o
