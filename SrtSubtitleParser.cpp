#include "stdafx.h"
#include "SrtSubtitleParser.h"
#include "FileUtility.h"
#include "ConsoleUtility.h"
#include "WriteConsoleHelper.h"


namespace Utility
{

    SrtSubtitleParser::SrtSubtitleParser( const std::wstring& file_name )
        : m_file_name( file_name )
    {
        parse();
    }


    void SrtSubtitleParser::parse()
    {
        m_subtitles.reserve( 5000 );
        std::wstring s = Utility::wstring_from_file( m_file_name.c_str() );
        boost::wsmatch m;
        boost::wregex e
        (
            L"(?x)"
            L"^ ([0-9]+) [\\r\\n]+"         // $1: number
            L"^ ([^\\r\\n]+) [\\r\\n]+"     // $2 start-stop time
            L"^ ([^\\r\\n]+) [\\r\\n]+"     // $3 text
            L"^ ([^\\r\\n]+)?"              // $4 ?text2
        );

        boost::wregex time_e
        (
            L"([0-9]+):([0-9]+):([0-9]+),([0-9]+) --> ([0-9]+):([0-9]+):([0-9]+),([0-9]+)"
        );

        boost::wsregex_iterator it( s.begin(), s.end(), e );
        boost::wsregex_iterator end;

        for ( ; it != end; ++it )
        {
            SrtSubtitle sub;
            sub.number = boost::lexical_cast<size_t>( it->str(1) );

            std::wstring time_string = it->str(2);
            boost::wsmatch m;
            if ( boost::regex_search( time_string, m, time_e ) )
            {
                sub.start_hour           = boost::lexical_cast<size_t>( m.str(1) );
                sub.start_minute         = boost::lexical_cast<size_t>( m.str(2) );
                sub.start_second         = boost::lexical_cast<size_t>( m.str(3) );
                sub.start_millisecond    = boost::lexical_cast<size_t>( m.str(4) );
                sub.start_time = ( sub.start_hour*3600 + sub.start_minute*60 + sub.start_second ) * 1000 + sub.start_millisecond;

                sub.stop_hour            = boost::lexical_cast<size_t>( m.str(5) );
                sub.stop_minute          = boost::lexical_cast<size_t>( m.str(6) );
                sub.stop_second          = boost::lexical_cast<size_t>( m.str(7) );
                sub.stop_millisecond     = boost::lexical_cast<size_t>( m.str(8) );
                sub.stop_time = ( sub.stop_hour*3600 + sub.stop_minute*60 + sub.stop_second ) * 1000 + sub.stop_millisecond;
            }

            sub.text = it->str(3);
            
            if ( (*it)[4].matched )
            {
                sub.text2 = it->str(4);
            }

            m_subtitles.push_back( sub );
        }
    }

}
