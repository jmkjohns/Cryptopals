#include <iostream>

//constants
const char base64[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
const char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

//types
//==============================================================================
struct ByteStream
//==============================================================================
{
    unsigned char *bytes;
    int size;
};


//prototypes
void printHex( ByteStream data );
void printBase64( ByteStream data );
ByteStream hexToBytes( char *hexString );
void hexToBytes( ByteStream &data );
ByteStream base64ToBytes( char *base64String );
ByteStream encodeBase64( ByteStream data );
ByteStream decodeBase64( ByteStream data );
char hexCharToByte( char hexChar );
ByteStream fixedXOR( ByteStream one, ByteStream two );
void fixedXOR( ByteStream &output, ByteStream inputOne, ByteStream inputTwo );
int fixedXORInPlace( ByteStream &decoder, ByteStream input );
void printBytesAsString( ByteStream input );
ByteStream stringToBytes( char *string );
void repKeyXOR( ByteStream &input, ByteStream key );
int hammingDistance( ByteStream one, ByteStream two );


//==============================================================================
void printHex( ByteStream data )
//==============================================================================
{
    for (int i = 0; i < data.size; ++i)
    {
        std::cout << hex[data.bytes[i] >> 4] << hex[data.bytes[i] & 0x0f];
    }
    std::cout << std::endl;
}


//==============================================================================
void printBase64( ByteStream data )
//==============================================================================
{
    for (int i = 0; i < data.size; ++i)
    {
        std::cout << base64[data.bytes[i]];
    }
    std::cout << std::endl;
}


//==============================================================================
//takes a c string representing a string of hex values and converts it into
//a byte stream. Assumes the c string has a terminating null
//if there is an odd number of chars, assumes the final 4 bits are zero

ByteStream hexToBytes( char *hexString )
//==============================================================================
{
    //declare
    ByteStream data;
    int strLength = strlen( hexString );

    //initialise
    data.size = 
        (strLength % 2) 
            ? strlen( hexString) / 2 + 1 
            : strLength / 2;
    data.bytes = new unsigned char[data.size];
    memset( data.bytes, 0, data.size * sizeof( char ) );

    //convert string
    for (int i = 0; i < data.size * 2; i += 2)
    {
        data.bytes[i / 2] = hexCharToByte( hexString[i] ) << 4;
        if ((i + 1) < strLength)
        {
            data.bytes[i / 2] = data.bytes[i / 2] | hexCharToByte( hexString[i + 1] );
        }
    }

    //return
    return data;
}


//==============================================================================
//takes a bytestream which has a hex string extracted from a file and 
//translates it into raw bytes in place
//assumes that the string is a multiple of 2 in length

void hexToBytes( ByteStream &data )
//==============================================================================
{
    //declare
    int newSize = data.size / 2;

    for (int i = 0; i < data.size; i += 2)
    {
        data.bytes[i / 2] = hexCharToByte( data.bytes[i] ) << 4;
        if ((i + 1) < data.size)
        {
            data.bytes[i / 2] = data.bytes[i / 2] | hexCharToByte( data.bytes[i + 1] );
        }
    }

    //TODO - clear remaining memory?
    
    data.size = newSize;
}


//==============================================================================
ByteStream base64ToBytes( char *base64String )
//==============================================================================
{
    ByteStream data;
    return data;
}


//==============================================================================
ByteStream encodeBase64( ByteStream data )
//==============================================================================
{
    //declare
    ByteStream output;
    int size = data.size + (2 * (data.size % 3)) % 3;
    unsigned char *buf = new unsigned char[size];
    unsigned char *inBuf = 0;
    unsigned char *outBuf = 0;

    //set up output
    output.size = (size / 3) * 4;
    output.bytes = new unsigned char[output.size];

    //set up buffers
    memset( buf, 0, size * sizeof( unsigned char ) );
    memset( output.bytes, 0, output.size * sizeof( unsigned char ) );
    memcpy( buf, data.bytes, data.size * sizeof( unsigned char ) );

    //iterate over the buffer, encoding data
    for (int i = 0; i < size; i += 3)
    {
        inBuf = buf + i;
        outBuf = output.bytes + ((i / 3) * 4);

        outBuf[0] = inBuf[0] >> 2;
        outBuf[1] = ((inBuf[0] << 4) & 0x3f) | inBuf[1] >> 4;
        outBuf[2] = ((inBuf[1] << 2) & 0x3f) | inBuf[2] >> 6;
        outBuf[3] = inBuf[2] & 0x3f;
    }

    //clean up and return
    delete [] buf;
    return output;
}


//==============================================================================
ByteStream decodeBase64( ByteStream data )
//==============================================================================
{
    ByteStream output;

    return output;
}


//==============================================================================
char hexCharToByte( char hexChar )
//==============================================================================
{
    char byte = 0;

    if (hexChar >= '0' && hexChar <= '9')
    {
        byte = hexChar - '0';
    }

    if (hexChar >= 'a' && hexChar <= 'f')
    {
        byte = hexChar - 'a' + 10;
    }

    return byte;
}


//==============================================================================
ByteStream fixedXOR( ByteStream one, ByteStream two )
//==============================================================================
{
    ByteStream result = {0, 0};

    if (one.size == two.size)
    {
        result.size = one.size;
        result.bytes = new unsigned char[result.size];
        for (int i = 0; i < result.size; ++i)
        {
            result.bytes[i] = one.bytes[i] ^ two.bytes[i];
        }
    }

    return result;
}

//==============================================================================
void fixedXOR( ByteStream &output, ByteStream inputOne, ByteStream inputTwo )
//==============================================================================
{
    //free any existing memory
    if (output.bytes)
    {
        delete [] output.bytes;
    }

    //assign to output
    output = fixedXOR( inputOne, inputTwo );
}


//==============================================================================
void printBytesAsString( ByteStream input )
//==============================================================================
{
    for (int i = 0; i < input.size; ++i)
    {
        std::cout << input.bytes[i];
    }
    
    std::cout << std::endl;
}


//==============================================================================
int fixedXORInPlace( ByteStream &decoder, ByteStream input )
//==============================================================================
{
    int retCode = 0;

    if (decoder.size == input.size)
    {
        for (int i = 0; i < input.size; ++i)
        {
            decoder.bytes[i] ^= input.bytes[i];
        }
    }
    else
    {
        retCode = 1;
    }

    return retCode;
}


//==============================================================================
ByteStream stringToBytes( char *string )
//==============================================================================
{
    ByteStream output;
    output.size = strlen( string );
    output.bytes = new unsigned char[output.size];
    memcpy( output.bytes, string, output.size );
    return output;
}


//==============================================================================
void repKeyXOR( ByteStream &input, ByteStream key )
//==============================================================================
{
    for (int i = 0; i < input.size; ++i)
    {
        input.bytes[i] = input.bytes[i] ^ key.bytes[i % key.size];
    }
}


//==============================================================================
int hammingDistance( ByteStream one, ByteStream two )
//==============================================================================
{
    int total = -1;
    ByteStream output = {0, 0};
    unsigned char current = 0;

    if (one.size == two.size)
    {
        total = 0;
        output.size = one.size;
        output.bytes = new unsigned char[output.size];
        memcpy( output.bytes, one.bytes, one.size );

        for (int i = 0; i < output.size; ++i)
        {
            *(output.bytes + i) ^= *(two.bytes + i);
            current = *(output.bytes + i);
            while (current != 0)
            {
                total++;
                current &= current - 1;
            }
        }

        delete [] output.bytes;
    }

    return total;
}
