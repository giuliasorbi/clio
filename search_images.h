std::string get_filename( const std::string& url );
std::string get_sname( const std::string& url );

class search_images {
public:
    search_images( const bool& save, const bool& write_file );
    void search( const std::string& url );

private:
    bool m_save;
    //std::string m_directory;
    bool m_write_file;
    //std::string m_filename;

    std::vector<std::string> img_find( const std::string& data , const std::string& url );
    void img_save( const std::string& directory, const std::vector<std::string>& url );
    void img_write( const std::string& filename, const std::vector<std::string>& url );
};