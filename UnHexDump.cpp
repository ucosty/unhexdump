//
// UnHexDump
// Copyright (c) 2009 Matthew Costa
// http://www.rackbits.com/
//
// ------------------------------------------------------------------------------------
//
// This program takes the ascii output of HexDump on a Cisco 797x or 79x1
// phone and converts it into a binary file.
//
// Possibly also works with other HexDump implementations that output
// in a format like this example:
//
// 000a100 6972 676e 6428 3e2d 616c 7473 754e 626d
// 000a110 7265 202c 2d63 643e 6169 656c 4e64 6d75
// 000a120 6562 2c72 7320 7a69 6f65 2866 2d64 6c3e
//
// Where each line contains 8 groups of two bytes prefixed with 
// the relative address of the line.
//
// IMPORTANT: This program is set to fix endian issues with respect to
//            translating from Big Endian on the MIPS processor to little
//            endian on my Intel processor. Comment out the FIX_ENDIAN define
//            below to disable this behaviour.
//
// Also note: This program is quite hackish and does little in the way of sanity
//            checking. It will probably fail on output that differs too far from
//            the example above.
//
// That said it has successfully converted hundreds of kilobytes of hex output
// streamed from a PuTTY session to a Cisco IP phone.
//
// Use it at your own peril >D
//
// ------------------------------------------------------------------------------------
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#define FIX_ENDIAN 1

using namespace std;

template <class T>
bool from_string(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

#ifdef FIX_ENDIAN
inline void endian_swap(unsigned short& x)
{
	// Swap the byte order
    x = (x>>8) | (x<<8);
}
#endif

int main(int argc, char* argv[])
{
	// Temporary hex value holder
	unsigned short hex_value;

	// Temporary string chunk holder
	string chunk;

	// Counter of bytes written
	unsigned int counter = 0;

	cout << "UnHexDump Version 0.3 Written by Matthew Costa" << endl;

	// Only run if the number of arguments is correct
	if(argc != 3 )
	{
		cout << "Usage: UnHexDump.exe input output" << endl;
		return 1;
	}
	
	// Open the second argument as the input file for reading
	ifstream fin(argv[1]);

	// Open the third argument as the output file in binary mode for writing
	ofstream fout(argv[2], ios_base::out | ios_base::binary);

	// Loop until there is no more file left
	while(!fin.eof())
	{
		for(int i=0; i<9; i++)
		{
			// Read the next space delimited chunk of hex-ascii
			fin >> chunk;

			// Skip this if it is the first chunk in the line
			if(i == 0) continue;

			// Convert the string to an integer
			if(from_string<unsigned short>(hex_value, chunk, std::hex))
			{
#ifdef FIX_ENDIAN
				// Perform an endian-ness swap
				endian_swap(hex_value);
#endif
				// Write the two-byte value to the output file
				fout.write((char*)&hex_value, sizeof(unsigned short));

				// Increment the bytes counter by 2
				counter+=2;
			}

		}
		
	}

	cout << "Wrote " << counter << " bytes." << endl;
	
	// Close the input and output files
	fin.close();
	fout.close();
	return 0;
}
