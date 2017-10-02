#include "..\Common\CryptoLib.cpp"

//constants
int letterCounts[] = { 9, 2, 4, 4, 13, 3, 2, 6, 8, 1, 1, 5, 3, 8, 8, 3, 1, 7, 7, 10, 3, 2, 2, 1, 2, 1, 0, 0, 0, 0, 0, 0, 9, 2, 4, 4, 13, 3, 2, 6, 8, 1, 1, 5, 3, 8, 8, 3, 1, 7, 7, 10, 3, 2, 2, 1, 2, 1 }; 


int scoreString( ByteStream input );

//==============================================================================
int main( int argc, char *argv[] )
//==============================================================================
{
    //declare
    ByteStream encoded = hexToBytes( "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736" );
    ByteStream byteEncoder = {0, 0};
    ByteStream output = {0, 0};
    int index = 0;
    int max = 0;
    int current = 0;

    //initialise
    byteEncoder.size = encoded.size;
    byteEncoder.bytes = new unsigned char[byteEncoder.size];

    //loop and decode against a string of single characters
    for (unsigned int i = 0; i < 256; ++i)
    {
        memset( byteEncoder.bytes, i, byteEncoder.size * sizeof( unsigned char ));
        fixedXOR( output, encoded, byteEncoder );
        current = scoreString( output );

        if (current > max)
        {
            max = current;
            index = i;
        }
    }

    memset( byteEncoder.bytes, index, byteEncoder.size * sizeof( unsigned char ));
    fixedXOR( output, encoded, byteEncoder );

    std::cout << "Input: ";
    printHex( encoded );
    std::cout << "Code: " << (unsigned char)index << " (" << index << ")" << std::endl;
    std::cout << "Output: ";
    printBytesAsString( output );
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

        //arbitrary value assigned to space
        if (input.bytes[i] == 0x20)
        {
            total += 10;
        }
    }

    return total;
}
