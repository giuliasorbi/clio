#include <iostream>
#include <boost/network/protocol/http/client.hpp>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#include "search_images.h"


int main( int argc, const char* argv[] )
{
    using namespace boost::program_options;

    bool save = false;
    bool write_file = false;
    std::vector<std::string> url;

    try {
        options_description desc( "Allowed options" );
        desc.add_options()
            ( "help", "produce help message" )
            ( "url", value<std::vector<std::string>>(), "input url" )
            ( "save,s", "save images in local folder" )
            ( "output,o", "write images url in local file" )
        ;
        positional_options_description p;
        p.add( "url", -1 );
        command_line_parser parser{argc, argv};
        parser.options(desc).positional(p).allow_unregistered();
        parsed_options parsed_options = parser.run();
        variables_map vm;
        store( parsed_options, vm );
        notify( vm ); 

        if ( vm.count( "help" )  ){
            std::cout << desc << "\n";
            return 1;
        }
        else {
            if( vm.count( "save" )) {
                save = true;
            }
            if( vm.count( "output" )) {
                write_file = true;
            }
            if ( vm.count( "url" ) ) {
                url = vm[ "url" ].as<std::vector<std::string>>();
            }
            else {
                std::cout << "Error: the option --url is required" << std::endl;
            }
        }

    } catch ( const boost::program_options::error &ex ) {
        std::cerr << ex.what() << '\n';
        return 1;
    }

    search_images s( save, write_file ); // constructor

    for ( auto u : url ) {
        std::cout << "\n\n----- Retrieving data from " << u << " -----" << std::endl;
        s.search( u );
    }

    return 0; 
}