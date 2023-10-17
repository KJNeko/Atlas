//
// Created by kj16609 on 10/6/23.
//

#include <chrono>
#include <string>

int main( int argc, char** argv )
{
	if ( argc != 2 )
	{
		//TODO: Add error reporting here
		// We should expect TWO arguments. The 0th arg is the path to the OUR executable and the 1st arg is the PID of the process to watch.
	}

	//int pid = std::stoi( argv[ 1 ] );

	//Watch the pid until it dies. If it takes longer then 1 minute then abort the update process.
	const auto now { std::chrono::steady_clock::now() };
	const auto timeout { now + std::chrono::minutes( 1 ) };
	while ( std::chrono::steady_clock::now() < timeout )
	{
		//TODO: Add watching code
	}

	//One last check to ensure that we left the loop because the process died.
	if ( /* Check process aliveness here. This just gets the compiler to STFU ->*/ true )
	{
		//If we have entered here then we are safe to begin modifying the files.
	}
	else
	{
		//Error report shit here
	}
}
