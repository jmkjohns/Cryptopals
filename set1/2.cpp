#include "..\Common\CryptoLib.cpp"

//==============================================================================
int main( int argc, char *argv[] )
//==============================================================================
{
    ByteStream one = hexToBytes( "1c0111001f010100061a024b53535009181c" );
    ByteStream two = hexToBytes( "686974207468652062756c6c277320657965" );

    printHex( fixedXOR( one, two ) );
}
