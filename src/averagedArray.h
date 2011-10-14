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
        int count, bufferHi;
        
        void ensureLatest() {
          
            if (count == 0)
                return;
            
            timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);

            int hi = bufferHi;
            
            int lo = hi;
            for (int i = hi; i > hi - count; i--) {
                timespec age = bufferAge[i % BUFFLEN];
                double diff = (now.tv_sec - age.tv_sec) + ((now.tv_nsec - age.tv_nsec) / NANOSECONDS_PER_SECOND);
                
                if (diff > averageInterval) {
                    lo = i;
                } else {
                    break;
                }                
            }
            
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
            if (count < BUFFLEN) {
                count++;
            }
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
        
        int getLatest(double delta, T* values, timespec* times) {
            ensureLatest();
            
            timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);    
            
            int hi = bufferHi;
            int length = 0;
            for (int i = hi; i > hi - count; i--) {
                timespec age = bufferAge[i % BUFFLEN];
                double diff = (now.tv_sec - age.tv_sec) + ((now.tv_nsec - age.tv_nsec) / NANOSECONDS_PER_SECOND);
                                
                if (diff > delta) {
                    break;
                } else {
                    length++;
                }
            }
            
            values = new T [length];
            times = new timespec [length];
            for (int i = 0; i < length; i++) {
                values[i] = buffer[(hi - i) % BUFFLEN];
                times[i] = bufferAge[(hi - i) % BUFFLEN];
            }
                
            return values;
        }
               
        AveragedArray(double averageInterval) {
            this->averageInterval = averageInterval;
            bufferHi = -1;
            count = 0;
            value = 0;
        }
        
        ~AveragedArray() {
            delete [] buffer;
            delete [] bufferAge;
        };
};