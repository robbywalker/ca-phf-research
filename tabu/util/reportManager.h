#pragma once

#include <time.h>

namespace util {
	class reportManager {
		int frequency;
		time_t start, last, now;
	
	public:
		reportManager() {
			frequency = 5;
			time(&last);
			time(&start);
		}

		reportManager(int frequency) {
			this->frequency = frequency;
			time(&last);
			time(&start);
		}

		bool reportRequired() {
			time(&now);
			return ( now - last > frequency);
		}

		void reported() {
			time(&last);
		}

		long getTotalTime() {
			time(&now);
			return (long) (now - start);
		}
	};
};