/***************************************************************************
 *   Copyright (C) 2017 by Lucas V. Hartmann <lucas.hartmann@gmail.com>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <functional>
#include <vector>

/// Returns true if x1 is pareto-dominant over x2
template <class T>
bool pareto_dominant(
	const T &x1, const T &x2,
	const std::vector< std::function<double(const T &)> > objectives
) {
	bool dominant = false;
	for (auto f : objectives) {
		double f1 = f(x1);
		double f2 = f(x2);
		
		if (f1 > f2)
			return false;
		
		if (f1 < f2)
			dominant = true;
	}
	return dominant;
}

/// Returns true if x1 is pareto-dominanted by any of the others.
template <class T>
size_t pareto_dominated(
	const T &x1,
	const std::vector<T> &others,
	const std::vector< std::function<double(const T &)> > objectives
) {
	size_t r = 0;
	for (auto &x2 : others)
		if (pareto_dominant(x2,x1,objectives))
			r++;
	
	return r;
}

/// Returns a vector containing pareto frontier elements.
#include <boost/range/adaptor/reversed.hpp>
template <class T>
std::vector<T> pareto_frontier(
	const std::vector<T> &src,
	std::vector< std::function<double(const T &)> > objectives
) {
	std::vector<T> tmp;
	for (auto &x1 : src) {
		if (!pareto_dominated(x1, tmp, objectives))
			tmp.push_back(x1);
	}
	
	std::vector<T> ret;
	for (auto &x1 : boost::adaptors::reverse(tmp)) {
		if (!pareto_dominated(x1, ret, objectives))
			ret.push_back(x1);
	}
	
	return ret;
}
