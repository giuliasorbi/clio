#include <iostream>
#include <curl/curl.h>
#include <string>
#include <fstream>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>

#include "utils.h"

int main( int argc, const char* argv[] )
{
  
    if( argc == 1 ) {
        std::cout << "Error. Insert valid URL" << std::endl;
    }

    /*boost::program_options::options_description desc("Allowed options");

    desc.add_options()
        ("save,s", "save images in local folder")
    ;
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm); 
*/
    std::string data = retrieve_data( argv[1] );
    //std::string url = std::string(argv[1]);//+std::string("/");
	std::vector<std::string> images = img_find( data, argv[1] );

   // if( vm.count("save")) {

    for (auto i : images) {
        char *s = new char[i.length() + 1];
        std::strcpy(s, i.c_str());
        std::vector<std::string> url_comp;
        boost::split( url_comp, i ,boost::is_any_of("/") );
        std::cout << "Saving " << i  << std::endl;
        img_save( s, "img/" + url_comp.back());
    }

   //}
    //else{
        return 0;
   // }

}
