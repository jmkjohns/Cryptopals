#include "..\Common\Cryptolib.cpp"
#include <fstream>

//==============================================================================
int main( int argc, char *argv[] )
//==============================================================================
{
    //declare
    ByteStream key = stringToBytes( "ICE" );
    ByteStream input = {0, 0};

    //get input text from file
    std::ifstream inputFile;
    inputFile.open( "5.txt", std::ios_base::binary );

    if (inputFile.good())
    {
        //get input text size
        inputFile.seekg( 0, std::ios_base::end );
        input.size = inputFile.tellg();
        inputFile.seekg( 0, std::ios_base::beg );

        //create input bytestream
        input.bytes = new unsigned char[input.size];
        inputFile.read( (char *)input.bytes, input.size );

        repKeyXOR( input, key );

        printHex( input );
        inputFile.close();
    }
}


