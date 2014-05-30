#include <windows.h>

#include "tree.h"

#include <boost/foreach.hpp>
#include <boost/unordered_set.hpp>

void draw_point( HDC hDC, const int x, const int y )
{
	const int point_radius = 4;
	Ellipse( hDC, x - point_radius, y - point_radius, x + point_radius, y + point_radius );
}

void draw_line( HDC hDC, const int x1, const int y1, const int x2, const int y2 )
{
	MoveToEx( hDC, x1, y1, NULL );
	LineTo( hDC, x2, y2 );
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	using namespace storage;
	
	typedef segment< int > segment_t;
	static tree< segment_t > finder;
	typedef std::vector< segment_t > data_array;
	static data_array segments;
	static boost::unordered_set< int > found_segments;
	static int rect_x1 = -1, rect_y1 = -1, rect_x2 = -1, rect_y2 = -1;
	static int last_x = -1, last_y = -1;
	static int id = 0;

	switch(message)
	{
	case WM_LBUTTONDOWN:
		if( rect_x1 >=0 && rect_y1 >=0 )
			break;
		if( last_x >= 0 && last_y >= 0 )
		{
			int current_x = LOWORD( lParam );
			int current_y = HIWORD( lParam );
			const int current_id = ++id;
			segment_t segment( point( current_x, current_y ), point( last_x, last_y ), current_id );
			segments.push_back( segment );
			finder.add_segment( segment );
			last_x = last_y = -1;
		}
		else
		{
			last_x = LOWORD( lParam );
			last_y = HIWORD( lParam );
		}
		break;
	case WM_RBUTTONDOWN:

		if( rect_x1 < 0 && rect_y1 < 0 )
		{
			rect_x1 = LOWORD( lParam );
			rect_y1 = HIWORD( lParam );
		}
		else
		{
			HDC hDC;
			hDC = GetDC( hwnd );

			HPEN hPen=CreatePen( 2, 2, RGB( 255, 255, 255 ) );
			SelectObject( hDC, hPen );

			Rectangle( hDC, rect_x1, rect_y1, rect_x2, rect_y2 );

			ReleaseDC( hwnd, hDC ); 
	
			box query = detail::get_edges( point( rect_x1, rect_y1 ), point( rect_x2, rect_y2 ) );
			found_segments.clear();
			data_array detected_segments;
			finder.find_segments( query, std::back_inserter( detected_segments ) );
			for( data_array::const_iterator cit = detected_segments.begin(); cit != detected_segments.end(); ++cit )
				found_segments.insert( cit->get_info() );
			rect_x1 = rect_y1 = -1;
		}
		break;
	case WM_MOUSEMOVE:
		if( rect_x1 >= 0 && rect_y1 >= 0 && rect_x2 >= 0 && rect_y2 >= 0 )
		{
			HDC hDC;
			hDC = GetDC( hwnd );

			HPEN hPen=CreatePen( 2, 2, RGB( 255, 255, 255 ) );
			SelectObject( hDC, hPen );

			Rectangle( hDC, rect_x1, rect_y1, rect_x2, rect_y2 );

			ReleaseDC( hwnd, hDC ); 
		}

		rect_x2 = LOWORD( lParam );
		rect_y2 = HIWORD( lParam );

		break;
	case WM_PAINT:
		HDC hDC;
		hDC = GetDC( hwnd );

		SelectObject( hDC, GetStockObject( NULL_BRUSH ) );
		if( rect_x1 >=0 && rect_y1 >= 0 )
			Rectangle( hDC, rect_x1, rect_y1, rect_x2, rect_y2 );
		{
			HBRUSH hbrush = CreateSolidBrush( WHITE_BRUSH );
			SelectObject( hDC, hbrush );
			for( size_t i = 0; i < segments.size(); ++i )
			{
				const int x1 = segments[i].get_start().x;
				const int y1 = segments[i].get_start().y;
				const int x2 = segments[i].get_end().x;
				const int y2 = segments[i].get_end().y;

				draw_line( hDC, x1, y1, x2, y2 );
				if( found_segments.find( segments[i].get_info() ) == found_segments.end() )
				{
					draw_point( hDC, x1, y1 );
					draw_point( hDC, x2, y2 );
				}
			}
				
			if( last_x >= 0 && last_y >= 0 )
			{
				draw_point( hDC, last_x, last_y );
			}

			DeleteObject( hbrush );
		}

		{
			HBRUSH hbrush = CreateSolidBrush( RGB( 0, 255, 0 ) );
			SelectObject( hDC, hbrush );
			for( size_t i = 0; i < segments.size(); ++i )
			{
				const int x1 = segments[i].get_start().x;
				const int y1 = segments[i].get_start().y;
				const int x2 = segments[i].get_end().x;
				const int y2 = segments[i].get_end().y;

					
				draw_line( hDC, x1, y1, x2, y2 );
				if( found_segments.find( segments[i].get_info() ) != found_segments.end() )
				{
					draw_point( hDC, x1, y1 );
					draw_point( hDC, x2, y2 );
				}
			}
			DeleteObject( hbrush );
		}

		ReleaseDC( hwnd, hDC );
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	default:
		return DefWindowProc( hwnd, message, wParam, lParam );
	}
	return 0;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	MSG msg;
	
	WNDCLASS  wc;
	LPCSTR szClassName = "classname";
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon( NULL, IDI_ASTERISK );
	wc.hCursor       = LoadCursor( NULL, IDC_CROSS );
	wc.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = szClassName;
	RegisterClass( &wc );

	HWND hWnd;
	hWnd = CreateWindow(
		szClassName,
		"Segment Detector GUI",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,  
		CW_USEDEFAULT,    
		CW_USEDEFAULT,     
		CW_USEDEFAULT,       
		NULL,                
		NULL,              
		hInstance,         
		NULL );        

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	while( GetMessage( &msg, NULL, 0, 0 ) ) 
	{   	
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return (int)msg.wParam;
}
