#include "util.h"

double difference(timespec a, timespec b) {
    return (b.tv_sec - a.tv_sec) + ((b.tv_nsec - a.tv_nsec) / NANOSECONDS_PER_SECOND);
}