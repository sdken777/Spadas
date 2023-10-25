// ///////////////////////////////////////////////////////////////// //
// *C++ 11 BigInteger Library                                 
// *Copyright(c) 2018  Mbadiwe Nnaemeka Ronald                 
// *Github Repository <https://github.com/ron4fun>             

// *Distributed under the MIT software license, see the accompanying file LICENSE 
// *or visit http ://www.opensource.org/licenses/mit-license.php.           

// *Acknowledgements:                                  
// ** //
// *Thanks to Ugochukwu Mmaduekwe (https://github.com/Xor-el) for his creative        
// *development of this library in Pascal/Delphi                         

// ////////////////////////////////////////////////////// ///////////////

#include "RandomNumberGenerator.h"
#include "PCGRandomNumberGenerator.h"
// Spadas modification: Function declarations
#include <stdexcept>
// Spadas modification end

const char * RandomNumberGenerator::UnknownAlgorithm = "Unknown Random Generation Algorithm Requested";

IRandomNumberGenerator RandomNumberGenerator::CreateRNG()
{
	// Spadas modification: For compatibility
	// return make_shared<PCGRandomNumberGenerator>();
	return IRandomNumberGenerator(new PCGRandomNumberGenerator());
	// Spadas modification end
}

IRandomNumberGenerator RandomNumberGenerator::CreateRNG(const string rngName)
{
	// Spadas modification: For compatibility
	// if (rngName == "PCGRandomNumberGenerator") return make_shared<PCGRandomNumberGenerator>();
	if (rngName == "PCGRandomNumberGenerator") return IRandomNumberGenerator(new PCGRandomNumberGenerator());
	// Spadas modification end

	throw invalid_argument(UnknownAlgorithm);
}

