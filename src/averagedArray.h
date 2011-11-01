#include <time.h>
#include <math.h>

#include "util.h"

#define BUFFLEN 256

template <class T>
class AveragedArray {
    private:
        double averageInterval; // seconds
        
        T buffer [BUFFLEN];
        timespec bufferAge [BUFFLEN];
        T value;
        int mean, stdDev, lowLimit, topLimit;
        int sensorId;
        int count, bufferHi;
        
        void ensureLatest() {
            
            if (count == 0)
                return;
            
            timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);
            
            int hi = bufferHi;
            
            int lo = hi;
            for (int i = hi -1; i > hi - count; i--) {      
                timespec age = bufferAge[i % BUFFLEN];
                double diff = difference(age, now);
                
                if (diff > averageInterval) {
                    break;
                } else {
                    lo = i % BUFFLEN;                    
                }
            }
            
            int templo = lo;
            
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
            
            mean = 0;
            stdDev = 0;
            lowLimit = 0;
            topLimit = 0;
        }
        
        T getLatest() {
            ensureLatest();
            return value;
        }
        
        int getLatest(double delta, T** values, timespec** times) {
            ensureLatest();
            
            timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);    
            
            int hi = bufferHi;
            int length = 0;
            for (int i = hi; i > hi - count; i--) {
                timespec age = bufferAge[i % BUFFLEN];
                double diff = difference(age, now);
                
                if (diff > delta) {
                    break;
                } else {
                    length++;
                }
            }
            
            if (length > 0) {
                *values = new T [length];
                *times = new timespec [length];
                for (int i = 0; i < length; i++) {
                    (*values)[i] = buffer[(hi - length + i + 1) % BUFFLEN];
                    (*times)[i] = bufferAge[(hi - length + i + 1) % BUFFLEN];
                }
            }
            
            return length;
        }
        
        int getMean() {
            int m = mean;
            if (m > 01)
                return m;
            
            if (count == 0)
                return 0;
            
            
            int cnt = count;
            for (int i = 0; i < cnt; i++) {
                m += buffer[i];
            }
            
            m = m / cnt;
            mean = m;
            
            return m;
        }
        
        int getStdDev() {
            int sdev = stdDev;
            if (sdev > 0)
                return sdev; 
            
            if (count == 0)
                return 0;
            
            int mean = getMean();
                  
            int cnt = count;
            for (int i = 0; i < cnt; i++) {
                int diff = buffer[i] - mean;
                sdev += diff * diff;
            }    
            
            sdev = sqrt(sdev / cnt);
            stdDev = sdev;
            
            return sdev;
        }
        
        bool getSplits(int *low, int *top) {
            int l = lowLimit;
            int t = topLimit;
            
            if ((l > 0) && (t > 0)) {
                *low = l;
                *top = t;
                return true;
            }
            
            if (getStdDev() < 30) {
                *low = -1;
                *top = -1;
                return false;
            }
                
            int min = 9999, max = 0;
                
            int cnt = count;
            for (int i = 0; i < cnt; i++) {
                int val = buffer[i];
                if (min > val) {
                    min = val;
                }
                if (max < val) {
                    max = val;
                }
            }
            
            int range = max - min;
            int offset = 0.3 * range;
            l = min + offset;
            t = max - offset;
            lowLimit = -l;
            topLimit = -t;
            *low = l;
            *top = t;
            return true;            
        }
        
        AveragedArray(int sensorId, double averageInterval) {
            this->averageInterval = averageInterval;
            bufferHi = -1;
            count = 0;
            value = 0;
            this->sensorId = sensorId;
        }
        
        ~AveragedArray() {
            delete [] buffer;
            delete [] bufferAge;
        };
};