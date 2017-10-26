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

#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include "pareto.h"

using namespace std;

auto nanoseconds() {
	return std::chrono::nanoseconds(
		std::chrono::system_clock::now().time_since_epoch()
	).count();
}

int main(int argc, char *argv[]) {
	cout << "<< Pareto Test >>" << endl;
	
	size_t n = 1024;
	if (argc > 1) {
		n = atoi(argv[1]);
	}
	
	// Random engine initialization
	std::default_random_engine r(nanoseconds());
	auto rd = [&]()->double {
		return std::generate_canonical<double, 64>(r);
	};
	
	// Randomize input data
	cout << "Generating " << n << " random pairs... " << flush;
	std::vector< std::pair<double, double> > data(n);
	for (auto &p : data) {
		do {
			p.first  = 4*rd();
			p.second = 4*rd();
		} while (p.first < 1./p.second);
	}
	cout << "Done." << endl;
	
	// This is where the magic happens:
	cout << "Building Pareto... " << flush;
	auto start = nanoseconds();
	auto p = pareto_frontier(data, { 
		[](const std::pair<double,double> &p)->double {
			return p.first;
		},
		[](const std::pair<double,double> &p)->double {
			return p.second;
		}
	});
	auto stop = nanoseconds();
	cout << "Completed after " << (stop - start)/1e6 << "ms." << endl;
	
	// Save to files, so you can plot with octave
	cout << "Selected " << p.size() << " out of " << data.size()
	<< " data points for the Pareto, saving... " << flush;
	
	ofstream outdata("pareto_data.txt", ios::out | ios::trunc);
	for (auto &i : data)
		outdata << i.first << " " << i.second << endl;
	
	ofstream outpareto("pareto.txt", ios::out | ios::trunc);
	for (auto i : p)
		outpareto << i.first << " " << i.second << endl;
	
	cout << "Done." << endl;
	
	return EXIT_SUCCESS;
}
