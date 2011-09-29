#ifndef _LIB_POWER_BUTTON_H
#define _LIB_POWER_BUTTON_H

#define BOOST_DATE_TIME_NO_LIB
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

int power_button_get_value();
void power_button_reset();

#endif

