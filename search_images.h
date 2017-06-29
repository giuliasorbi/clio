std::string get_filename( const boost::network::uri::uri &url );

class search_images {
public:
    search_images( const bool& save, const std::string& directory, const bool& write_file, const std::string& filename );
    void search( const std::string& url );

private:
    bool m_save;
    std::string m_directory;
    bool m_write_file;
    std::string m_filename;

    std::vector<std::string> img_find( const std::string& data , const std::string& url );
    void img_save( const std::string& directory, const std::vector<std::string>& url );
    void img_write( const std::string& filename, const std::vector<std::string>& url );
};