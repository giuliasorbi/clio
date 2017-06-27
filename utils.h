size_t write_callback( char* buf, size_t size, size_t nmemb, void* userp );
size_t write_data( char *ptr, size_t size, size_t nmemb, void *userdata );
std::string retrieve_data( const char *url);
std::vector<std::string>  img_find( std::string data, const char *url );
void img_save( char *url, std::string name);