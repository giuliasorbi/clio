#include <iostream>
#include <boost/network/protocol/http/client.hpp>
#include <boost/network/uri.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include "utils.h"


int main( int argc, const char* argv[] )
{
    using namespace boost::program_options;

    try {
        options_description desc( "Allowed options" );
        desc.add_options()
            ( "help", "produce help message" )
            ( "url", value<std::vector<std::string>>()->required(), "input url" )
            ( "save,s", value<std::string>(), "save images in local folder")
            ( "output,o", value<std::string>(), "write images url in local file")
        ;
        positional_options_description p;
        p.add( "url", -1 );
        command_line_parser parser{argc, argv};
        parser.options(desc).positional(p).allow_unregistered();
        parsed_options parsed_options = parser.run();
        variables_map vm;
        store( parsed_options, vm );
        notify( vm ); 

        if ( vm.count( "help" ) ) {
            std::cout << desc << "\n";
            return 1;
        }
        else {
            if ( vm.count( "url" ) ) {
                for ( auto url :  vm[ "url" ].as<std::vector<std::string>>() ) {
                    std::cout << "\n\n----- Retrieving data from " << url << " -----" << std::endl;
                    std::string data = retrieve_data( url );
                    std::vector<std::string> img = img_find ( data, url );
                    if( vm.count( "save" )) {
                        img_save( vm[ "save" ].as<std::string>(), img );
                    }
                     if( vm.count( "output" )) {
                        img_write( vm[ "output" ].as<std::string>(), img );
                    }
                }
            }
        }
       
    } catch (const boost::program_options::error &ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }

    return 0; 
}
