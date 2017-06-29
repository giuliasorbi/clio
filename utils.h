std::string get_filename( const boost::network::uri::uri &url );
std::string retrieve_data( const std::string& url );
std::vector<std::string> img_find( const std::string& data , const std::string& url );
void img_save( std::string directory, const std::vector<std::string>& url );
void img_write( std::string filename, const std::vector<std::string>& url );