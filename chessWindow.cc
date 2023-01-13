#include "chessWindow.h"
#include "position.h"
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <unistd.h>

void ChessWindow::createNewChessWindow(int width, int height, int x, int y ) {
    // Creates a simple window as a child of the root window.
    // Uses Black and White pixel for foreground and bakcground colors respectively
    // Places the top-left to the specified (x,y) coordinate
	w = XCreateSimpleWindow(display, 
            RootWindow(display, s), x, y,
			width, height, winBorderWidth, 
            BlackPixel(display, s), 
            WhitePixel(display, s));

    // Draws window on screen
    XMapRaised(display, w);

    // Flushes the buffer created to the server
	XFlush(display);
}

// Sets up the graphics controller
void ChessWindow::setUpGC() {
	unsigned long valuemask = 0;
	XGCValues values; 

	gc = XCreateGC(display, w, valuemask, &values);
	if (!gc) throw runtime_error("GC setup failed!");

	XSetForeground(display, gc, BlackPixel(display, s));
	XSetBackground(display, gc, WhitePixel(display, s));
	
	// Defines the style for the lines
	XSetLineAttributes(display, gc, 2, LineSolid, CapButt, JoinBevel);

    // Defines fill style to be solid for the graphics controller
	XSetFillStyle(display, gc, FillSolid);
}

void ChessWindow::xbmIOReader(const unique_ptr<Pixmap>& bitmap, unsigned int x, unsigned int y) {
    int rc = XReadBitmapFile(
        display, w, fileName.c_str(),
        &x, &y, 
        bitmap.get(), 0, 0);
    
    if (rc == BitmapOpenFailed) throw runtime_error("Read failed: " + fileName + " - No file found.");
    else if (rc == BitmapFileInvalid) throw runtime_error("Read failed: " + fileName + " - No valid bitmap.");
    else if (rc == BitmapNoMemory) throw runtime_error("Read failed: " + fileName + " - Not enough memory.");
}

void ChessWindow::readXBM() {

    unique_ptr<Pixmap> bitmap { nullptr };

    // Reads in the xbm for standard chess pieces
	for (int i = 0; i < 12; ++i) {
        if (pieceMap[i] != toupper(pieceMap[i])) {
            fileName = folder + "Black"; 
		} else {
            fileName = folder + "White"; 
		}

        // Produces file names
        fileName += toupper(pieceMap[i]); // filename of chess piece on white square
        string bsquareCopy = fileName; // filename of chess piece on black square
        char tmpInd = i + '1';
        if (i == 9) tmpInd = 'D';
        else if(i == 10) tmpInd = 'E';
        else if(i == 11) tmpInd = 'F';
        fileName += xbmExtension;
        bsquareCopy += "1" + xbmExtension;

        // Reads xbm file and stores in map
		bitmap = make_unique<Pixmap>();
        xbmIOReader(bitmap, bitmapSize, bitmapSize);
        pieceToPixmap[pieceMap[i]] = move(bitmap);
        fileName = bsquareCopy;
        bitmap = make_unique<Pixmap>();
        xbmIOReader(bitmap, bitmapSize, bitmapSize);
        pieceToPixmap[tmpInd] = move(bitmap);

        // Flushes output buffer to server and checks if all in sync before proceding
        XSync(display,False);        
	} 

    // Reads in the xbm for other board elements
    for (int i = 0; i < 3; ++i) {
        bitmap = make_unique<Pixmap>();

        switch (boardDecor[i]) {
            case ' ':
                fileName = folder + "square" + xbmExtension;
                xbmIOReader(bitmap,bitmapSize,bitmapSize);
                break;
            case '.':
                fileName = folder + "bsquare" + xbmExtension;
                xbmIOReader(bitmap,bitmapSize,bitmapSize);
                break;
            case '/':
                fileName = folder + "background" + xbmExtension;
                xbmIOReader(bitmap, width, height);
                break;
        }
        
        pieceToPixmap[boardDecor[i]] = move(bitmap);
    }

    XCopyPlane(display, *(pieceToPixmap.at('/')), w, gc, 0, 0, width, height, 0, 0, 1);
    XSync(display, False);

    Pos tmp {};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            tmp.row = i;
            tmp.col = j;
            clear(tmp);
        }
    }
}

// Draws a piece on a square
void ChessWindow::draw(char piece, const Pos& pos) {
    int row = pos.row * (bitmapSize + offset) + margin;
	int col = pos.col * (bitmapSize + offset) + margin;

    if (pos.row % 2 == pos.col % 2) {
	    XCopyPlane(display, *(pieceToPixmap.at(piece)), w, gc, 0, 0, bitmapSize,bitmapSize, col, row, 1);
        XSync(display, False); 
    } else {
        XCopyPlane(display, *(pieceToPixmap.at(conversionMap.at(piece))), w, gc, 0, 0, bitmapSize,bitmapSize, col, row, 1);
        XSync(display, False);
    }
}

// Clears a square of pieces
void ChessWindow::clear(const Pos& pos) {
    int row = pos.row * (bitmapSize + offset) + margin;
	int col = pos.col * (bitmapSize + offset) + margin;

    if (pos.row % 2 == pos.col % 2) {
	    XCopyPlane(display, *(pieceToPixmap.at(boardDecor[0])), w, gc, 0, 0, bitmapSize,bitmapSize, col, row, 1);
        XSync(display, False);
    } else {
        XCopyPlane(display, *(pieceToPixmap.at(boardDecor[1])), w, gc, 0, 0, bitmapSize,bitmapSize, col, row, 1);
        XSync(display, False);
    }
}

// ChessWindow's ctor
ChessWindow::ChessWindow(int width, int height) : width{width}, height{height} {
    // Attempts to open a connection to the X server to control a display
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        cerr << "Cannot open display" << endl;
        exit(1);
    }
    
    // Obtains the display count (number of windows)
	s = DefaultScreen(display);

    createNewChessWindow(width, height, 0, 0); // Window setup
    setUpGC(); // Graphics controller setup
    readXBM(); // Read from XBM files

    // Flushes output buffer to server and checks if all in sync before proceding
	XSync(display, False);

    usleep(500);
}

// ChessWindow's dtor
ChessWindow::~ChessWindow() {
    XFlush(display);
    for (auto & bitmap : pieceToPixmap) {
        XFreePixmap(display, *(pieceToPixmap.at(bitmap.first)));
    }
    XFreeGC(display, gc);
    XCloseDisplay(display);
}


