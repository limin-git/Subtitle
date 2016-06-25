#pragma once


namespace Utility
{

    struct SrtSubtitle
    {
        size_t number;
        size_t start_hour;
        size_t start_minute;
        size_t start_second;
        size_t start_millisecond;
        size_t start_time;

        size_t stop_hour;
        size_t stop_minute;
        size_t stop_second;
        size_t stop_millisecond;
        size_t stop_time;
        std::wstring text;
        std::wstring text2;
    };

    typedef std::vector<SrtSubtitle> SrtSubtitleList;


    class SrtSubtitleParser
    {
    public:
        
        SrtSubtitleParser( const std::wstring& file_name );
        void parse();

    public:

        std::wstring m_file_name;
        SrtSubtitleList m_subtitles;
    };

}
