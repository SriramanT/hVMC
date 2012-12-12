/*
 * Copyright (c) 2012, Robert Rueger <rueger@itp.uni-frankfurt.de>
 *
 * This file is part of hVMC.
 *
 * hVMC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * hVMC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hVMC.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "lattice_2dsquare.hpp"
using namespace std;



Lattice2DSquare::Lattice2DSquare( cl_uint L_init )
  : Lattice( LATTICE_2DSQUARE, L_init ), S( uintsqrt( L_init ) )
{
  assert( is_perfect_square( L_init ) );
}



void Lattice2DSquare::get_Xnn(
  cl_uint l, cl_uint X,
  vector<cl_uint>* outbuf ) const
{
  assert( l < 2 * L );
  assert( X == 1 || X == 2 || X == 3 );

  if ( X == 1 ) {
    get_1nn( l, outbuf );
  } else if ( X == 2 ) {
    get_2nn( l, outbuf );
  } else { /* X == 3 */
    get_3nn( l, outbuf );
  }
}

void Lattice2DSquare::get_1nn( cl_uint l, vector<cl_uint>* outbuf ) const
{
  outbuf->resize( 4 );

  // add left neighbor
  if ( l % S == 0 ) {
    ( *outbuf )[0] = l + S - 1;
  } else {
    ( *outbuf )[0] = l - 1;
  }

  // add right neighbor
  if ( ( l + 1 ) % S == 0 ) {
    ( *outbuf )[1] = l + 1 - S;
  } else {
    ( *outbuf )[1] = l + 1;
  }

  // add bottom neighbor
  if ( l < S || ( l >= L && l < L + S ) ) {
    ( *outbuf )[2] = l + L - S;
  } else {
    ( *outbuf )[2] = l - S;
  }

  // add top neighbor
  if ( l >= 2 * L - S || ( l < L && l >= L - S ) ) {
    ( *outbuf )[3] = l + S - L;
  } else {
    ( *outbuf )[3] = l + S;
  }
}

void Lattice2DSquare::get_2nn( cl_uint l, vector<cl_uint>* outbuf ) const
{
  outbuf->resize( 4 );

  // add bottom left neighbor
  if ( l % S == 0 ) {
    // in left column
    if ( l == 0 || l == L ) {
      // bottom left corner
      ( *outbuf )[0] = l + L - 1;
    } else {
      ( *outbuf )[0] = l - 1;
    }
  } else if ( l < S || ( l >= L && l < L + S ) ) {
    // in bottom row
    // (but NOT in bottom left corner!)
    ( *outbuf )[0] = l + L - S - 1;
  } else {
    // in the center
    ( *outbuf )[0] = l - S - 1;
  }

  // add bottom right neighbor
  if ( ( l + 1 ) % S == 0 ) {
    // in right column
    if ( l == S - 1 || l == L + S - 1 ) {
      // bottom right corner
      ( *outbuf )[1] = l + L + 1 - 2 * S;
    } else {
      ( *outbuf )[1] = l + 1 - 2 * S;
    }
  } else if ( l < S || ( l >= L && l < L + S ) ) {
    // in bottom row
    // (but NOT in bottom right corner!)
    ( *outbuf )[1] = l + L + 1 - S;
  } else {
    // in the center
    ( *outbuf )[1] = l + 1 - S;
  }

  // add top right neighbor
  if ( ( l + 1 ) % S == 0 ) {
    // in right column
    if ( l == L - 1 || l == 2 * L - 1 ) {
      // top right corner
      ( *outbuf )[2] = l + 1 - L;
    } else {
      ( *outbuf )[2] = l + 1;
    }
  } else if ( l >= 2 * L - S || ( l < L && l >= L - S ) ) {
    // in top row
    // (but NOT in top right corner!)
    ( *outbuf )[2] = l + S + 1 - L;
  } else {
    // in the center
    ( *outbuf )[2] = l + S + 1;
  }

  // add top left neighbor
  if ( l % S == 0 ) {
    // in left column
    if ( l == L - S || l == 2 * L - S ) {
      // top left corner
      ( *outbuf )[3] = l + 2 * S - 1 - L ;
    } else {
      ( *outbuf )[3] = l + 2 * S - 1;
    }
  } else if ( l >= 2 * L - S || ( l < L && l >= L - S ) ) {
    // in top row
    // (but NOT in top left corner!)
    ( *outbuf )[3] = l + S - 1 - L;
  } else {
    // in the center
    ( *outbuf )[3] = l + S - 1;
  }
}

void Lattice2DSquare::get_3nn( cl_uint l, vector<cl_uint>* outbuf ) const
{
  outbuf->resize( 4 );

  // add left neighbor
  if ( l % S <= 1  ) {
    ( *outbuf )[0] = l + S - 2;
  } else {
    ( *outbuf )[0] = l - 2;
  }

  // add right neighbor
  if ( l % S >= S - 2 ) {
    ( *outbuf )[1] = l + 2 - S;
  } else {
    ( *outbuf )[1] = l + 2;
  }

  // add bottom neighbor
  if ( l < 2 * S || ( l >= L && l < L + 2 * S ) ) {
    ( *outbuf )[2] = l + L - 2 * S;
  } else {
    ( *outbuf )[2] = l - 2 * S;
  }

  // add top neighbor
  if ( l >= 2 * ( L - S ) || ( l < L && l >= L - 2 * S ) ) {
    ( *outbuf )[3] = l + 2 * S - L;
  } else {
    ( *outbuf )[3] = l + 2 * S;
  }
}



IrreducibleIdxRel Lattice2DSquare::reduce_idxrel(
  cl_uint i, cl_uint j ) const
{
  assert( i < 2 * L );
  assert( j < 2 * L );
  assert( ( i < L && j < L ) || ( i >= L && j >= L ) );

  // calculate the positions of i and j
  const cl_uint x_i = i % S;
  const cl_uint y_i = i / S;
  const cl_uint x_j = j % S;
  const cl_uint y_j = j / S;

  // calculate the position difference
  cl_uint dx = x_i > x_j ? x_i - x_j : x_j - x_i;
  cl_uint dy = y_i > y_j ? y_i - y_j : y_j - y_i;

  // wrap large differences around the boundaries
  if ( dx > S / 2 ) {
    dx = S - dx;
  }
  if ( dy > S / 2 ) {
    dy = S - dy;
  }

  // dx should be larger than dy
  if ( dy > dx ) {
    swap( dx, dy );
  }

  return IrreducibleIdxRel( 0, dx + S * dy );
}



set<IrreducibleIdxRel> Lattice2DSquare::irreducible_idxrel_list() const
{
  set<IrreducibleIdxRel> irr_idxrels;
  for ( cl_uint i = 0; i < L; ++i ) {
    irr_idxrels.insert( reduce_idxrel( 0, i ) );
  }

#if VERBOSE >= 1
  cout << "Lattice2DSquare::irreducible_idxrel_list() : "
       << "list of irreducible index relations =" << endl;
  for ( auto it = irr_idxrels.begin(); it != irr_idxrels.end(); ++it ) {
    cout << "(" << it->first << "," << it->second << ")" << endl;
  }
#endif

  return irr_idxrels;
}