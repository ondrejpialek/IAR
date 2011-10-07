default:
	g++ program.cpp motorControl.cpp control.cpp interfaceKit.cpp sensing.cpp phidget.cpp -o program -lphidget21 -lrt -lpowerbutton -D NSILENT -g
	
test:
	g++ test.cpp motorControl.cpp control.cpp interfaceKit.cpp sensing.cpp phidget.cpp -o test -lphidget21 -lrt -lpowerbutton
	
clean:
	rm -f *.o
