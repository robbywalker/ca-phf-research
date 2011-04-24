#pragma once

#include "../tabu/util/cache.h"
#include "../tabu/arrays/tuple.h"

#include "../coverage/fhCache.h"

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <utility>

namespace coverage {
	typedef std::pair< arrays::tuple, int > byteId;
	typedef util::cache< byteId, char > byteCacheBase;

	class byteCache : public byteCacheBase {
		fhCache * fhC;
	public: 
		byteCache( ) : byteCacheBase(5000000) {			
			this->fhC = NULL;
		}

		void setFHCache( fhCache * fhC ) {
			this->fhC = fhC;
		}

		virtual char populate( const byteId & id ) {
			FILE * f = fhC->get( id.first );

			fseek( f, id.second, SEEK_SET );

			char buffer;
			fread( &buffer, 1, 1, f );

			return buffer;
		}

		virtual void persist( const byteId & id, char value ) {
			FILE * f = fhC->get( id.first );

			fseek( f, id.second, SEEK_SET );
			fwrite( &value, 1, 1, f );
		}
	};
};