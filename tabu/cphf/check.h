#pragma once

#include <stdlib.h>

#include <vector>
#include <set>
#include <list>

#include "utuple.h"
#include "deep_array.h"

#include "../math/galois_field.h"

namespace cphf {

	template <class Symbol>
	class cphf_Check {
		typedef std::vector<Symbol> sliceType;
		static deep_array nonCovering;
		static bool initialized;

	public:
		cphf_Check(int strength, int base) {
			if ( !initialized )
				init(strength, base);
		}

		static void init(int strength, int base) {
			if ( strength != 3 && strength != 4 ) {
				printf("Strength must be 3 or 4!\n");
				exit(1);
			}

			initialized = true;

			nonCovering.init( util::ipow(base,strength-1), strength );

			if ( strength == 3 ) {
				// first, we assume alpha_2 == 0
				math::GFNum gamma0(base);
				math::GFNum i(base), j(base), k(base);

				for ( gamma0 = 0; gamma0 < base; gamma0++ ) {
					utuple u(3);
					for ( i = 0; i < base; i++ ) {
						u[0] = (int)gamma0 + base * (int) i;
						for ( j = 0; j < base; j++ ) {
							if ( j == i ) 
								continue;
							u[1] = (int) gamma0 + base * (int) j;
							for ( k = 0; k < base; k++ ) {
								if ( k == i || k == j )
									continue;

								u[2] = (int) gamma0 + base * (int) k;
								nonCovering.put( u, true );
							}
						}
					}
				}

				// now, we assume alpha_2 != 0
				math::GFNum gamma1(base);

				for ( gamma0 = 0; gamma0 < base; gamma0++ ) {
					for ( gamma1 = 0; gamma1 < base; gamma1++ ) {

						utuple u(3);
						for ( i = 0; i < base; i++ ) {
							// i = h1_1
							math::GFNum h1_2 = gamma1 * i + gamma0;
							u[0] = ((const int) h1_2) * base + ((const int) i);

							for ( j = 0; j < base; j++ ) {
								if ( j == i ) 
									continue;

								// j = h2_1
								math::GFNum h2_2 = gamma1 * j + gamma0;
								u[1] = (const int) h2_2 * base + (const int) j;

								for ( k = 0; k < base; k++ ) {
									if ( k == i || k == j )
										continue;								

									// k = h3_1
									math::GFNum h3_2 = gamma1 * k + gamma0;
									u[2] = (const int) h3_2 * base + (const int) k;

									nonCovering.put( u, true );								
								}
							}
						}
					}
				}
			}

			else if ( strength == 4 ) {
				// STRENGTH = 4
				utuple u(4);

				// build list of 4 distinct ordered pairs
				typedef std::pair<int,int> opair;
				typedef std::vector<opair> pairset;
				typedef std::list<pairset> plist;
				
				plist pairsets;
				pairset cur(4);
	
				int h1_2, h1_3, h2_2, h2_3, h3_2, h3_3, h4_2, h4_3;

				for ( h1_2 = 0; h1_2 < base; h1_2++ ) {
					for ( h1_3 = 0; h1_3 < base; h1_3++ ) {

						cur[0] = opair( h1_2, h1_3 );

						for ( h2_2 = 0; h2_2 < base; h2_2++ ) {
							for ( h2_3 = 0; h2_3 < base; h2_3++ ) {

								if ( h2_2 == h1_2 && h2_3 == h1_3 )
									continue;

								cur[1] = opair( h2_2, h2_3 );

								for ( h3_2 = 0; h3_2 < base; h3_2++ ) {
									for ( h3_3 = 0; h3_3 < base; h3_3++ ) {

										if (( h3_2 == h2_2 && h3_3 == h2_3 ) ||
											( h3_2 == h1_2 && h3_3 == h1_3 ))
											continue;

										cur[2] = opair( h3_2, h3_3 );

										for ( h4_2 = 0; h4_2 < base; h4_2++ ) {
											for ( h4_3 = 0; h4_3 < base; h4_3++ ) {

												if (( h4_2 == h3_2 && h4_3 == h3_3 ) ||
													( h4_2 == h2_2 && h4_3 == h2_3 ) ||
													( h4_2 == h1_2 && h4_3 == h1_3 ))
													continue;

												cur[3] = opair( h4_2, h4_3 );
												pairsets.push_back( cur );
											}
										}
									}
								}

							}
						}

					}
				}

				// case 1.a. alpha3 = 0, alpha2 = 0
				math::GFNum gamma0(base);

				for ( gamma0 = 0; gamma0 < base; gamma0++ ) {
					utuple u(4);

					std::list<pairset>::const_iterator it = pairsets.begin();
					for( ; it != pairsets.end(); it++ ) {
						pairset p = *it;

						u[0] = ( p[0].second * base + p[0].first ) * base + (int)gamma0;
						u[1] = ( p[1].second * base + p[1].first ) * base + (int)gamma0;
						u[2] = ( p[2].second * base + p[2].first ) * base + (int)gamma0;
						u[3] = ( p[3].second * base + p[3].first ) * base + (int)gamma0;

						nonCovering.put( u, true );
					}
				}
				printf("done 1.a.\n");

				// case 1.b. alpha3 = 0, alpha2 != 0
				math::GFNum gamma1(base), htemp(base);
				for ( gamma0 = 0; gamma0 < base; gamma0++ ) {
					for ( gamma1 = 0; gamma1 < base; gamma1++ ) {
						std::list<pairset>::const_iterator it = pairsets.begin();
						for( ; it != pairsets.end(); it++ ) {
							pairset p = *it;

							for ( int i = 0; i < 4; i++ ) {
								htemp = gamma1 * math::GFNum(p[i].first,base) + gamma0;
								u[i] = ( p[i].second * base + (int) htemp ) * base + p[i].first;
							}

							nonCovering.put( u, true );
						}
					}
				}
				printf("done 1.b.\n");

				// case 2. alpha3 != 0
				math::GFNum gamma2(base);
				for ( gamma0 = 0; gamma0 < base; gamma0++ ) {
					for ( gamma1 = 0; gamma1 < base; gamma1++ ) {
						for ( gamma2 = 0; gamma2 < base; gamma2++ ) {

							std::list<pairset>::const_iterator it = pairsets.begin();
							for( ; it != pairsets.end(); it++ ) {
								pairset p = *it;

								for ( int i = 0; i < 4; i++ ) {
									htemp = gamma2 * math::GFNum(p[i].second,base) + gamma1 * math::GFNum(p[i].first,base) + gamma0;
									u[i] = ( (int) htemp * base + p[i].second ) * base + p[i].first;
								}

								nonCovering.put( u, true );
							}

						}
					}
				}
				printf("done 2.\n");

				//printf("enumerated %ld bad tuples", nonCovering.size() );
			}
		}

		inline bool good( const sliceType & slice ) {
			int size = (int) slice.size();
			for ( int i = 0; i < (size - 1); i++ ) {
				for ( int j = i+1; j < size; j++ ) {
					if ( slice[i] == slice[j] ) {
						return false;
					}
				}
			}

			// return true as long as we do not find it in the non-covering list
			return ( ! nonCovering.get( slice ) );
		}
		
	};

	template <class Symbol>
	deep_array cphf_Check<Symbol> :: nonCovering;

	template <class Symbol>
	bool cphf_Check<Symbol> :: initialized = false;

};