/*
 * Copyright (c) 2013, Robert Rueger <rueger@itp.uni-frankfurt.de>
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

#ifndef LATTICE_H_INCLUDED
#define LATTICE_H_INCLUDED

#include <vector>
#include <set>

#define EIGEN_NO_AUTOMATIC_RESIZING
#include <eigen3/Eigen/Core>

#include "macros.h"


enum lattice_t {
  LATTICE_1DCHAIN,
  LATTICE_2DSQUARE
};

class Lattice {

  public:

    const lattice_t type;
    const unsigned int L;

    Lattice( lattice_t type_init, unsigned int L_init )
      : type( type_init ), L( L_init ) { }
    virtual ~Lattice() { }

    unsigned int get_spinup_site( unsigned int l ) const;
    unsigned int get_spinlinked_site( unsigned int l ) const;

    virtual void get_Xnn(
      unsigned int l, unsigned int X, std::vector<unsigned int>* outbuf
    ) const = 0;

    virtual unsigned int reduce_idxrel( unsigned int i, unsigned int j ) const = 0;
    virtual std::set<unsigned int> irreducible_idxrel_list() const = 0;
    virtual unsigned int irreducible_idxrel_maxdist() const = 0;

    virtual Eigen::VectorXd r( unsigned int i, unsigned int j ) const = 0;
    virtual std::vector<Eigen::VectorXd> get_qvectors() const = 0;
};

#endif // LATTICE_H_INCLUDED
