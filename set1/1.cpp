#include "..\Common\Base64Lib.cpp"

//==============================================================================
int main( int argc, char *argv[] )
//==============================================================================
{
    char hexString[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

    ByteStream input = hexToBytes( hexString );
    std::cout << "Input: ";
    printHex( input );
    std::cout << "Output: ";
    ByteStream output = encodeBase64( input );
    printBase64( output );
}
