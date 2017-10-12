#include "..\Common\CryptoLib.cpp"
#include <fstream>

//constants
int letterCounts[] = { 9, 2, 4, 4, 13, 3, 2, 6, 8, 1, 1, 5, 3, 8, 8, 3, 1, 7, 7, 10, 3, 2, 2, 1, 2, 1, 0, 0, 0, 0, 0, 0, 9, 2, 4, 4, 13, 3, 2, 6, 8, 1, 1, 5, 3, 8, 8, 3, 1, 7, 7, 10, 3, 2, 2, 1, 2, 1 }; 


int scoreString( ByteStream input );


//==============================================================================
int main( int argc, char *argv[] )
//==============================================================================
{
    //declare
    std::ifstream input;
    ByteStream current = {0, 0};
    ByteStream maxStream = {0, 0};
    ByteStream decoder = {0, 0};
    int max = 0;
    int currentScore = 0;
    int currentString = 0;

    //initialise
    input.open( "4.txt" );
    current.bytes = new unsigned char[256]; 
    decoder.bytes = new unsigned char[256];
    maxStream.bytes = new unsigned char[256];

    //test
    if (input.good())
    {
        while(!input.eof())
        {
            memset( current.bytes, 0, 256 * sizeof( unsigned char ) );
            input.getline( (char *)current.bytes, 256 );
            current.size = input.gcount();
            hexToBytes( current );
            decoder.size = current.size;

            //test each line for XOR
            for (unsigned int i = 0; i < 256; ++i)
            {
                memset( decoder.bytes, i, 256 * sizeof( unsigned char ) );
                fixedXORInPlace( decoder, current );
                currentScore = scoreString( decoder );
                if (currentScore > 0)
                {
                //printBytesAsString( decoder );
                }

                if (currentScore > max)
                {
                    memcpy( maxStream.bytes, decoder.bytes, 256 );
                    maxStream.size = decoder.size;
                    max = currentScore;
                }
            }

            if (current.size == 256)
            {
                std::cout << "Buffer too small" << std::endl;
                exit( 1 );
            }
        }

        printBytesAsString( maxStream );

        //clean up
        input.close();
    }
}


//==============================================================================
int scoreString( ByteStream input )
//==============================================================================
{
    int total = 0;
    for (int i = 0; i < input.size; ++i)
    {
        if (input.bytes[i] >= 'A' && input.bytes[i] <= 'z')
        {
            total += letterCounts[input.bytes[i] - 'A'];
        }
        else
        {
            //arbitrary value assigned to space
            if (input.bytes[i] == 0x20)
            {
                total += 10;
            }
        }

    }

    return total;
}
