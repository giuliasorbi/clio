#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <boost/network/protocol/http/client.hpp>
#include <boost/network/uri.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <boost/exception/diagnostic_information.hpp> 

#include "search_images.h"

// constructor
search_images::search_images( const bool& save, const bool& write_file )
{
	m_save = save;
	m_write_file = write_file;
}

std::string get_filename( const std::string& url ) // get downloaded img name
{
	std::size_t index = url.find_last_of( '/' );
	std::string filename = url.substr( index + 1 );
	return filename.empty()? "index" : filename;
}

std::string get_sname( const std::string& url ) // get direcory name to download img ( or file name to write img url )
{
	std::size_t index = url.find_first_of( '.' );
	std::string sname = url.substr( index + 1 );
	std::replace( sname.begin(), sname.end(), '.', '_');
	std::replace( sname.begin(), sname.end(), '/', '_');
	return sname.empty()? "index" : sname;
}

void search_images::search( const std::string& url ) 
{
	using namespace boost::network;

	boost::network::uri::uri u( url );
	if ( u.is_valid() ) {
		http::client::options options;
		options.always_verify_peer( false );
		http::client client( options );
	
		try {
			http::client::request request( url );
			request << header( "Connection", "close" );
			http::client::response response = client.get( request );
			
			///////////////////////////////
			std::ofstream file;
			file.open ( "web_content.txt" );
			file << body( response );
			file.close();
			///////////////////////////////

			std::vector<std::string> img = img_find( body( response ), url );
			
			if( m_save ) {
				img_save( get_sname( url ), img );
			}
			if( m_write_file ) {
				img_write( get_sname( url ) + ".txt", img );
			}

		} catch ( const std::exception& ex ) {
				std::cerr << "Error: " << ex.what() << '\n';
		}

	} else {
		std::cerr << "Error: " << url << " is not a valid url " << std::endl;
	}

}

std::vector<std::string> search_images::img_find( const std::string& data , const std::string& url )
{
	int count = 0;
	boost::regex pattern ( ".*img.*src.*" );
	//boost::regex pattern (".*image.*href.*");
	//boost::regex pattern (".*image.*content.*");
	std::vector<std::string> strs, img, result;

	boost::split( strs, data ,boost::is_any_of( "<>" ) );

	for( auto str : strs ) {
		if ( boost::regex_match (str, pattern) ) {
			boost::split( img, str, boost::is_any_of( " " ) );
			for( auto s : img ) {

				if( boost::contains( s, "src=" )) {
					boost::erase_all( s, "src=" );
					boost::erase_all( s, "\"" );
					if ( !boost::contains( s, "http" ) ) {
						auto finder = boost::first_finder( "//" );
						boost::iterator_range<std::string::iterator> it = finder( s.begin(), s.end() );
						int index = std::distance( s.begin(), it.begin() );
						if( index == 0 ) {
							if( boost::contains ( url, "https" ) ) {
								s = "https:" + s;
							}
							else {
								s = "http:" + s;
							}
						}
						else { 
							s = url + s;
						}
					}
					boost::network::uri::uri img_url( s );
					if ( img_url.is_valid() ) { 
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
	}

	std::cout << "\n#images = " << count << "\n\n";

	return result;
} 

void search_images::img_save( const std::string& directory, const std::vector<std::string>& url )
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

			std::string filename = get_filename( u );
			std::cout << "Saving to: " << directory + "/" + filename << std::endl;
			std::ofstream ofs( directory + "/" + filename.c_str() );
			ofs << static_cast<std::string>( body( response ) ) << std::endl;
		} catch ( std::exception& ex ) {
			std::cerr << "Error: " << ex.what() << '\n';
		}
	}
}

void search_images::img_write( const std::string& filename, const std::vector<std::string>& url )
{
	std::ofstream file;
	file.open( filename, std::ios::app );
	for( auto u : url ) {
		file << u << "\n";
	}
	file.close();
}