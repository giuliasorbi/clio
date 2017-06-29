#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/network/protocol/http/client.hpp>
#include <boost/network/uri.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>

#include "utils.h"


std::string get_filename( const boost::network::uri::uri &url )
{
	std::string path = boost::network::uri::path(url);
	std::size_t index = path.find_last_of('/');
	std::string filename = path.substr(index + 1);
	return filename.empty()? "index.html" : filename;
}

std::string retrieve_data( const std::string& url )
{
	using namespace boost::network;

	http::client::options options;
	options.always_verify_peer( false );
	http::client client( options );

	http::client::request request( url );
	request << header( "Connection", "close" );
	http::client::response response = client.get( request );
	
	///////////////////////////////
	std::ofstream file;
    file.open ( "web_content.txt" );
    file << body( response );
    file.close();
	///////////////////////////////
	return body( response );
}

std::vector<std::string> img_find( const std::string& data , const std::string& url )
{
	int count = 0;
	boost::regex pattern ( ".*img.*src.*" );
	//boost::regex pattern (".*image.*href.*");
	//boost::regex pattern (".*image.*content.*");
	std::vector<std::string> strs, img, result;

	boost::split( strs, data ,boost::is_any_of( "<>" ) );

	for(auto str : strs ) {
		if ( boost::regex_match (str, pattern) ) {
			boost::split( img, str, boost::is_any_of( " " ) );
			for( auto s : img ) {
				if( boost::contains( s, "src=" )) {
					boost::erase_all( s, "src=" );
					boost::erase_all( s, "\"" );
					if( boost::contains( s, "//" ) ) {
						if( !boost::contains( s, "http" ) ) {
							s = "https:" + s;
						}
					}
					else { 
						s = url + s;
					}
					std::vector<std::string>::iterator i = find( result.begin(), result.end(), s );
					if( i == result.end() ) {
						result.push_back( s );
						std::cout  << s << std::endl;
						count++;
					}
				}
			}
		}
	}

	std::cout << "\n#images = " << count << std::endl;

	return result;
} 

void img_save( std::string directory, const std::vector<std::string>& url )
{
	using namespace boost::network;
	boost::filesystem::create_directory( directory );
	boost::filesystem::path dir( directory );

	http::client::options options;
	options.always_verify_peer( false );
	http::client client( options );
	for( auto u : url ) {
		try {
			http::client::request request( u );
			http::client::response response = client.get( request );
			std::string filename = get_filename( request.uri() );
			std::cout << "Saving to: " << directory + "/" + filename << std::endl;
			
			std::ofstream ofs( directory + "/" + filename.c_str() );
			ofs << static_cast<std::string>( body( response ) ) << std::endl;
		} catch ( std::exception& e ) {
			std::cout << "Error: " << u << " is not a valid img url " << std::endl;
		}
	}

}

void img_write( std::string filename, const std::vector<std::string>& url )
{
	std::ofstream file;
	file.open ( filename, std::ios::app );
	for( auto u : url ) {
		file << u << "\n";
	}
	file.close();
}

