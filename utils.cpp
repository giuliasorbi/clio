#include <iostream>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include "utils.h"

size_t write_callback(char* buf, size_t size, size_t nmemb, void* userp) {   //size*nmemb is the size of the buffer	 
    std::string * data  = static_cast <std::string *>(userp);
	for (int c = 0; c < size * nmemb; c++) {
        *data += buf[c];
    }
    return size * nmemb; //tell curl how many bytes we handled
}

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
    std::ofstream *out = static_cast<std::ofstream *>(userdata);
    size_t nbytes = size * nmemb;
    out->write(ptr, nbytes);
    return nbytes;
}

std::string retrieve_data( const char *url ) {
	CURL* curl; //our curl object
	std::string data;
    curl_global_init(CURL_GLOBAL_ALL); // Initialize everything possible
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url ); // URL to work on
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_callback); // Callback for writing data.
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &data );
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // ask libcurl to show us the verbose output

    curl_easy_perform(curl); // perform a blocking file transfer 

	// write web page content /////
	std::ofstream file;
	file.open ("web_content.txt");
	file << data;
	file.close();
	///////////////////////////////

	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return data;

}

std::vector<std::string> img_find( std::string data , const char *url ) {
    int count = 0;
	boost::regex pattern (".*img.*src.*");
	//boost::regex pattern (".*image.*href.*");
	//boost::regex pattern (".*image.*content.*");
    std::vector<std::string> strs, img, result;

    boost::split(strs, data ,boost::is_any_of("<>"));

    for(auto str : strs ) {
		if ( boost::regex_match (str, pattern) ) {
			boost::split(img, str, boost::is_any_of(" "));
			for( auto s : img ) {
				if( boost::contains( s, "src=" )) {
					boost::erase_all(s, "src=" );
					boost::erase_all(s, "\"" );
					if( boost::contains(s, "//") ) {
						if( !boost::contains(s, "http") ) {
							s = "https:" + s;
						}
					}
					else { 
						s = url + s;
					}
					std::vector<std::string>::iterator i = find(result.begin(), result.end(), s);
					if( i == result.end() ) {
						result.push_back(s);
						//std::cout << s << std::endl;
						count++;
						
					}
				}
			}
		}
    }
    std::cout << "\n#images = " << count << std::endl;
	return result;
} 

void img_save( char *url, std::string name ) {
	CURL *image; 
	CURLcode imgresult; 
	std::ofstream img_file;
	img_file.open(name, std::ios::out);

	if(!img_file.is_open())
    {
		std::cout << "Error file" << std::endl;
	}

	image = curl_easy_init(); 
	if( image ){ 
		curl_easy_setopt(image, CURLOPT_URL, url); 
		curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, write_data); 
		curl_easy_setopt(image, CURLOPT_WRITEDATA, &img_file); 

		imgresult = curl_easy_perform(image); 

		if( imgresult != 0 ){ 
			std::cout << "Cannot save the image!\n"; 
		} 
	}
	else {
		std::cout << "Error " << std::endl;
	} 
	curl_easy_cleanup(image); // Clean up the resources 
	img_file.close();
}
