#pragma once

#include <stdio.h>

namespace tabu {
	struct intScore {
		int score;
	public:
		intScore() {
			score = 0;
		}

		intScore( int _score ) : score( _score ) {
		}

		const int asInt() const {
			return score;
		}

		void print() {
			printf("%d", score);
		}

		void fprint(FILE * out) {
			fprintf(out, "%d", score);
		}
	};

	bool operator== (const intScore & a, const intScore & b) {
		return a.score == b.score;
	}

	bool operator== (const intScore & a, const int b) {
		return a.score == b;
	}

	bool operator> (const intScore & a, const int b) {
		return a.score > b;
	}

	bool operator< (const intScore & a, const intScore & b) {
		return a.score < b.score;
	}
};