#pragma once

#include <map>
#include <list>

#include "reportManager.h"

namespace util {
	template <class IdType, class ValueType>
	struct _cache_list_item {
		_cache_list_item(const IdType & _id) : id( _id ) {
		}

		IdType id;
		ValueType value;
		bool dirty;

		_cache_list_item<IdType,ValueType> * next;
		_cache_list_item<IdType,ValueType> * prev;
	};

	template <class IdType, class ValueType>
	class cache {
		typedef _cache_list_item<IdType, ValueType> _c_l;

		std::map<IdType, _c_l *> contents;

		_c_l * front;
		_c_l * back;

		int count;
		int size;

	protected:
		bool autoDirty;

	public:
		cache() {
			size = 10000;
			autoDirty = false;
			front = back = NULL;
			count = 0;
		}

		cache( int size ) {
			this->size = size;
			autoDirty = false;
			front = back = NULL;
			count = 0;
		}

		bool contains( const IdType & id ) {
			return contents.count( id ) == 1;
		}

		const ValueType & get( const IdType & id ) {
			if ( contents.count( id ) == 0 ) {
				// no entry available

				_c_l * newVal = new _c_l(id);
				newVal->value = populate( id );
				newVal->dirty = autoDirty;

				contents[id] = newVal;

				this->append( newVal );
				count++;

				if ( count == size ) {
					shrink();
				}

			} else {
				this->moveToBack( contents[ id ] );
			}

			return contents[id]->value;
		}

		ValueType & getForModification( const IdType & id ) {
			if ( contents.count( id ) == 0 ) {
				// no entry available

				_c_l * newVal = new _c_l(id);
				newVal->value = populate( id );
				newVal->dirty = true;

				contents[id] = newVal;

				this->append( newVal );
				count++;

				if ( count == size ) {
					shrink();
				}

			} else {
				contents[id]->dirty = true;
				this->moveToBack( contents[ id ] );
			}

			return contents[id]->value;
		}

		const void set( const IdType & id, const ValueType & value ) {
			if ( contents.count( id ) == 0 ) {
				// no entry available

				_c_l * newVal = new _c_l(id);
				newVal->value = value;
				newVal->dirty = true;

				contents[id] = newVal;
				this->append( newVal );
				count++;

				if ( count == size ) {
					shrink();
				}

			} else {
				contents[id]->value = value;
				contents[id]->dirty = true;
				this->moveToBack( contents[ id ] );
			}
		}

		void flush() {
			reportManager report(5);

			int i = 0;
			_c_l * it;
			for ( it = front; it != NULL; it = it->next ) {
				if ( it->dirty ) {
					persist( it->id, it->value );
					it->dirty = false;
				}

				i++;
				if ( report.reportRequired() ) {
					printf("Flushed %d elements out of %d.\n", i, count);
					report.reported();
				}
			}
		}

		void clear() {
			flush();

			contents.clear();

			// now clear out the list
			_c_l * it = front;
			_c_l * next;
			while ( it != NULL ) {
				next = it->next;

				remove( it->id, it->value );
				delete it;

				it = next;
			}

			front = back = NULL;
		}

	protected:
		virtual void remove( const IdType & id, ValueType value ) {
		}

		virtual void persist( const IdType & id, ValueType value ) {
		}

		virtual ValueType populate( const IdType & id ) {
			ValueType v;
			return v;
		}

	private:
		inline void append( _c_l * newEnd ) {
			if ( this->front == NULL ) {
				// empty list, now has 1 element
				newEnd->next = newEnd->prev = NULL;
				this->front = this->back = newEnd;
			} else {
				newEnd->next = NULL;
				newEnd->prev = this->back;
				this->back->next = newEnd;
				this->back = newEnd;
			}
		}

		inline void moveToBack( _c_l * elem ) {
			// test if the element is already at the back, if so, do nothing
			if ( elem->next != NULL ) {				
				elem->next->prev = elem->prev;

				if ( elem->prev == NULL ) {
					// the first element
					this->front = elem->next;
				} else {
					elem->prev->next = elem->next;
				}

				append( elem );
			}
		}

		void shrink() {
			// if necessary, persist the first element
			if ( front->dirty ) {
				persist( front->id, front->value );
			}
			remove( front->id, front->value );

			// remove it from the map
			contents.erase( front->id );

			// remove it from the list
			_c_l * next = front->next;
			delete front;

			front = next;
			if ( front == NULL )
				back = NULL;
			else
				front->prev = NULL;

			// mark the new count
			count--;
		}
	};

};
