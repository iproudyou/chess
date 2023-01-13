#ifndef __CHESSWINDOW_H__
#define __CHESSWINDOW_H__
#include <X11/Xlib.h>
#include <map>
#include <memory>
#include <iostream>
#include <string>

using namespace std;

class Pos;

class ChessWindow {
    Display *display; // display
    Window w; // window
    int s;
	int width, height;
    const int winBorderWidth = 2;
    GC gc; // graphics controller
	unsigned long colours[10];

	string xbmExtension = ".xbm";
	string folder = "";
	string fileName;

	char pieceMap[12] = {'K','Q','R','B','N','P','k','q','r','b','n','p'};
	char boardDecor[3] = {' ', '.', '/'};

	map<char, char> conversionMap = {
		{'K', '1'}, {'Q', '2'}, {'R', '3'},
		{'B', '4'}, {'N', '5'}, {'P', '6'},
		{'k', '7'}, {'q', '8'}, {'r', '9'},
		{'b', 'D'}, {'n', 'E'}, {'p', 'F'}	
	};
	map<char, unique_ptr<Pixmap>> pieceToPixmap;

	unsigned int bitmapSize = 64;
	const int margin = 40;
	const int offset = 7;

	void createNewChessWindow(int width, int height, int x, int y);
  	void setUpGC();
	void readXBM();
	void xbmIOReader(const unique_ptr<Pixmap>& bitmap, unsigned int x, unsigned int y);
	void setUpColours();

    public:
		ChessWindow(int width=642, int height=642);  
		~ChessWindow();                            

		void draw(char piece, const Pos& pos);
		void clear(const Pos& pos);
};

#endif
