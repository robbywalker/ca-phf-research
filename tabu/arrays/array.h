#pragma once

#include "tuple.h"
#include "symbol.h"

namespace arrays {

	template <class SYM>
	class Array {
		int symbolCount;
		int rows, cols;
		SYM ** contents;

		int * refs;

		void initFromFile( FILE * f, int & syms, int & strength ) {
            fscanf( f, "%d %d %d %d\n", &rows, &cols, &syms, &strength );

			contents = new SYM*[rows];

			for ( int i = 0; i < rows; i++ ) {
				contents[i] = new SYM[cols];

				for ( int j = 0; j < cols; j++ ) {
					contents[i][j] = SYM(f, syms, strength);
				}
			}

			refs = new int;
			*refs = 1;

			gen = NULL;
		}

		void destroy() {
			*refs = *refs - 1;
			if ( contents != NULL && ( *refs == 0 ) ) {
				delete refs;

				for ( int i = 0; i < rows; i++ ) {
					delete[] contents[i];
				}

				delete[] contents;
				contents = NULL;
			}
		}


	public:
		SymbolGenerator<SYM> * gen;

		Array() {
			gen = NULL;
			rows = 0;
			cols = 0;
			contents = NULL;
			refs = new int;
			*refs = 1;
		}

		Array(SymbolGenerator<SYM> * sg, int rows, int cols, bool random = false) {
			this->rows = rows;
			this->cols = cols;
			contents = new SYM*[rows];
			gen = sg;

			for ( int i = 0; i < rows; i++ ) {
				contents[i] = new SYM[cols];

				if ( random ) {
					for ( int j = 0; j < cols; j++ ) {
						contents[i][j] = sg->randomValue();
					}
				} else {
					for ( int j = 0; j < cols; j++ ) {
						contents[i][j] = sg->defaultValue();
					}
				}
			}
			refs = new int;
			*refs = 1;
		}

		Array( const Array<SYM> & copy ) {
			gen = copy.gen;
			rows = copy.rows;
			cols = copy.cols;
			contents = copy.contents;
			refs = copy.refs;
			*refs = *refs + 1;
		}

		Array( Array<SYM> & copy, int makeSureItCopies ) {
			gen = copy.gen;
			rows = copy.rows;
			cols = copy.cols;
			contents = copy.contents;
			refs = copy.refs;
			*refs = *refs + 1;
		}

		Array( char * filename, int & syms, int & strength ) {
			FILE * f = fopen(filename, "rt");
			if ( f == NULL ) {
				printf("cannot open file: %s\n", filename);
				exit(1);
			}

			initFromFile( f, syms, strength );

			fclose( f );
		}

		Array( FILE * inFile, int & syms, int & strength ) { 
			initFromFile( inFile, syms, strength );
		}

		~Array() {
			this->destroy();
		}

		void copyInto( Array<SYM> & arr ) const {
			arr.destroy();

			arr.symbolCount = symbolCount;
			arr.rows = rows;
			arr.cols = cols;
			arr.gen = gen;
			arr.refs = new int;
			*arr.refs = 1;

            arr.contents = new SYM*[rows];
			for ( int i = 0; i < rows; i++ ) {
				arr.contents[i] = new SYM[cols];

				for ( int j = 0; j < cols; j++ ) {
					arr.contents[i][j] = contents[i][j];
				}
			}
		}

		inline int getWidth() const {
			return cols;
		}

		inline int getHeight() const {
			return rows;
		}

		inline int getSymbolCount() const {
			return gen->getCount();
		}

		inline SYM get(int i, int j) const {
			return contents[i][j];
		}

		inline void set(int i, int j, SYM val) {
			contents[i][j] = val;
		}

		template <class Tuple>
		inline void sliceInto( int row, const Tuple & indices, std::vector<SYM> & slice ) const {
			for ( int i = 0; i < indices.strength(); i++ ) {
				slice[i] = contents[row][ indices[i] ];
			}
		}

		template <class Tuple>
		inline void sliceInto( int row, const Tuple & indices, SYM * slice ) const {
			for ( int i = 0; i < indices.strength(); i++ ) {
				slice[i] = contents[row][ indices[i] ];
			}
		}

		const void print() const {
			for (int i = 0; i < rows; i++ ) {
				for ( int j = 0; j < cols; j++ ) {
					contents[i][j].print();
					if ( j != (cols - 1) ) 
						printf(" ");
				}
				printf("\n");
			}
		}

		const void printRow(int i) const {
			for ( int j = 0; j < cols; j++ ) {
				contents[i][j].print();
				if ( j != (cols - 1) ) 
					printf(" ");
			}
			printf("\n");
		}

		const void fprint(FILE * out, int syms, int strength) const {
			fprintf(out, "%d %d %d %d\n", rows, cols, syms, strength);
			for (int i = 0; i < rows; i++ ) {
				for ( int j = 0; j < cols; j++ ) {
					contents[i][j].fprint(out);
					if ( j != (cols - 1) ) 
						fprintf(out, " ");
				}
				fprintf(out, "\n");
			}
		}

		const void fprintRow(FILE * out, int i) const {
			for ( int j = 0; j < cols; j++ ) {
				contents[i][j].fprint(out);
				if ( j != (cols - 1) ) 
					fprintf(out, " ");
			}
			fprintf(out, "\n");
		}
	};

};