// Subtitle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "InputUtility.h"
#include "SrtSubtitleParser.h"
#include "WriteConsoleHelper.h"


int _tmain(int argc, _TCHAR* argv[])
{
    const std::wstring class_name = L"MediaPlayerClassicW";
    const std::wstring player = L"C:\\Program Files\\MPC-HC\\mpc-hc64.exe";
    std::wstring movie = L"D:\\Download\\The.Divergent.Series.Allegiant.Part.1.2016.1080p.WEB-DL.x264.AC3-JYK\\The.Divergent.Series.Allegiant.Part.1.2016.1080p.WEB-DL.x264.AC3-JYK.mkv";
    std::wstring subtitle = L"D:\\Download\\The.Divergent.Series.Allegiant.Part.1.2016.1080p.WEB-DL.x264.AC3-JYK\\The.Divergent.Series.Allegiant.Part.1.2016.1080p.WEB-DL.x264.AC3-JYK.ºÚ”¢.srt";

    if ( argc == 3 )
    {
        movie = argv[1];
        subtitle = argv[2];
    }

    std::wstringstream movie_strm;
    movie_strm << "\"" << movie << "\" /open";

    HWND shell_handle = NULL;

    ShellExecute(
        NULL,
        L"open",
        player.c_str(),
        movie_strm.str().c_str(),
        0,
        SW_SHOWNOACTIVATE );

    Sleep( 1000 );

    std::wstring window_name = boost::filesystem::path( movie ).filename().wstring();
    HWND hwnd = FindWindow( class_name.c_str(), window_name.c_str() );

    if ( hwnd == NULL )
    {
        std::cout << "can not find window " << std::endl;
        CloseHandle( shell_handle );
        return 0;
    }

    Utility::SrtSubtitleParser parser( subtitle );

    Utility::SrtSubtitleList& subtitles = parser.m_subtitles;

    for ( size_t i = 0; i < subtitles.size(); ++i )
    {
        SetForegroundWindow( hwnd );
        Utility::send_input_ctrl( 'G' );
        Utility::SrtSubtitle& sub = subtitles[i];
        stdcout << sub.text2 << "\n" << sub.text << "\n";

        std::stringstream strm;
        strm << std::setw(2) << std::setfill('0') << sub.start_hour;
        strm << std::setw(2) << std::setfill('0') << sub.start_minute;
        strm << std::setw(2) << std::setfill('0') << sub.start_second;
        strm << std::setw(3) << std::setfill('0') << sub.start_millisecond;
        Utility::send_input( strm.str() );
        Utility::send_input_enter();
        Utility::send_input( VK_SPACE );
        Sleep( sub.stop_time - sub.start_time + 200 );
        Utility::send_input( VK_SPACE );
        SetForegroundWindow( GetConsoleWindow() );
        //std::cout << "press any key to continue" << std::endl;
        int ch = _getch();

        if ( ch == (unsigned char)'r' )
        {
            --i;
        }
        else if ( ch >= (unsigned char)'0' && ch <= (unsigned char)'9')
        {
            i += (ch - (unsigned char)'0' );
        }
        else if ( ch == (unsigned char)'q' )
        {
            break;
        }
        else if ( ch == (unsigned char)'g' )
        {
            i += 100;
        }
    }

    //TerminateProcess( hwnd, 0 );

    system( "taskkill /im mpc-hc64.exe" );


    //GetConsoleWindow();








    //send_input_ctrl( 'G' );
    //send_input( "002353");
    //send_input_enter();


	return 0;
}

