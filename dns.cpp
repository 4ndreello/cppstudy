#include "../../includes.h"

void reverseStr( std::string &str )
{
    int n = str.length( );
    for ( int i = 0; i < n / 2; i++ )
        std::swap( str[ i ], str[ n - i - 1 ] );
}

template <typename TP> std::string Num2Str( TP const &value )
{
    std::stringstream sin;
    sin << value;
    return sin.str( );
}

bool isNumber( const char &str )
{
    if ( std::isdigit( str ) == 0 ) return false;
    return true;
}

void PingAtHome( const std::string &address, const int &max_attempts, int reason )
{

#ifdef _WIN32
    std::string command = "ping -n " + Num2Str( max_attempts ) + " " + address + " | FIND \"TTL\" ";
#else
    std::string command = "ping -c " + Num2Str( max_attempts ) + " " + address + " | FIND \"TTL\" ";
#endif

    std::stringstream sout;

    FILE *in;
    char buff[ 512 ];

    if ( reason == 1 )
        command = "ping google.com -n 1 -f -l " + Num2Str( max_attempts );

#ifdef _WIN32
    if ( !( in = _popen( command.c_str( ), "r" ) ) )
    {
#else
    if ( !( in = popen( command.c_str( ), "r" ) ) )
    {
#endif
        ;
    }

    while ( fgets( buff, sizeof( buff ), in ) != nullptr )
    {
        sout << buff;
    }
    std::string ttl = "TTL=";
    std::size_t found = sout.str( ).find( ttl );
    std::string t = "";
    std::size_t exist_or_not = sout.str( ).find( ttl );

    switch ( reason )
    {
        case 0:
            for ( int x = found - 4; x > found - 7; x-- )
            {
                if ( !( isNumber( sout.str( )[ x ] ) ) )
                    break;
                t += sout.str( )[ x ];
            }
            reverseStr( t );
            var->dns_array.push_back( stoi( t ) );
            _pclose( in );
            break;
    
        case 1:
            if ( exist_or_not != std::string::npos )
                var->check = true;

            _pclose( in );
            break;

        default:
            break;
    }
}

void getMtu( )
{
    std::cout << "Getting your MTU..." << std::endl;
    for ( int i = 1500; i >= 1400; i-- )
    {
        PingAtHome( "1.1.1.1", i, 1 );
        if ( var->check )
        {
            var->vMtu = i + 28;
            break;
        }
    }
    std::cout << "Your MTU: " << var->vMtu << std::endl;
}

void net::getBetter( )
{
    std::string dns_list[ ] = { "4.2.2.1", "1.0.0.1", "144.91.80.191" };
    int arrSize = sizeof( dns_list ) / sizeof( dns_list[ 0 ] );
    int i = 0;

    std::cout << "Testing the best DNS for your profile...." << std::endl;

    for ( int x = 0; x < arrSize; x++ )
        PingAtHome( dns_list[ x ], 1, 0 );

    int min = *std::min_element( var->dns_array.begin( ), var->dns_array.end( ) );

    while ( i < arrSize )
    {
        if ( var->dns_array[ i ] == min )
            break;
        i++;
    }

    std::cout << "The best DNS ping is: " << min << "ms\nFrom: "  << dns_list[i] << std::endl;
    getMtu( );
}
