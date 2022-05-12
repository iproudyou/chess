CXX = g++
CXXFLAGS = -std=c++14 -g -Wall -MMD
EXEC = chess
OBJECTS = main.o controller.o chessSubject.o chessBoard.o textObserver.o piece.o king.o queen.o bishop.o rook.o knight.o pawn.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	rm ${DEPENDS} ${OBJECTS} ${EXEC}
