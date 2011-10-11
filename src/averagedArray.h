#include <time.h>

#define NANOSECONDS_PER_SECOND 1E9
#define BUFFLEN 256

template <class T>
class AveragedArray {
    private:
        double averageInterval; // seconds
        
        T buffer [BUFFLEN];
        timespec bufferAge [BUFFLEN];
        T value;
        int bufferLo, bufferHi;
        
        void ensureLatest() {
          
            timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);

            int hi = bufferHi;
            if (bufferLo == -1) {
                bufferLo = 0;
            } else {
                int lo = bufferLo;
                while (lo != hi) {
                    timespec age = bufferAge[lo];
                    double diff = (now.tv_sec - age.tv_sec) + ((now.tv_nsec - age.tv_nsec) / NANOSECONDS_PER_SECOND);
                    
                    if (diff > averageInterval) {
                        lo = (lo+1) % BUFFLEN;
                    } else {
                        break;
                    }
                }
                bufferLo = lo;
            }
            
            int lo = bufferLo;
            T avg = 0;
            int cnt = 0;
            while (lo != (hi + 1) % BUFFLEN) {
                avg += buffer[lo];
                lo = (lo+1) % BUFFLEN;
                cnt++;
            }
        
            value = avg / cnt;
                

        }
        
    public:
        void add(T val) {
            int nextBuff = (bufferHi + 1) % BUFFLEN;
            buffer[nextBuff] = val;
            timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);
            bufferAge[nextBuff] = now;
            bufferHi = nextBuff;
        }
        
        T getLatest() {
            ensureLatest();
            return value;
        }
               
        AveragedArray(double averageInterval) {
            this->averageInterval = averageInterval;
            bufferHi = -1;
            bufferLo = -1;
        }
        
        ~AveragedArray() {
            delete [] buffer;
            delete [] bufferAge;
        };
};