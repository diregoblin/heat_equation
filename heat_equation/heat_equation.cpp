// heat_equation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GridModel.h"


int _tmain(int argc, _TCHAR* argv[])
{
	double xStart = 0;
	double xFin = 1;
	int xNodes;
	double thermalDiffusivity;
	std::cout << "Rod length is " << xFin - xStart << " m" << std::endl;	
	std::cout << "Enter thermal diffusivity: ";	
	std::cin >> thermalDiffusivity;
	std::cout << "Enter number of nodes: ";
	std::cin >> xNodes;
	
	GridModel gridModel(xStart, xFin, xNodes, thermalDiffusivity);
	gridModel.partAddTrapezoids(1, 0.2, 0.2, 0.05);

	std::string s;
	double startTime = 0;
	double maxTime = 60; 
	int timeIterations;
	std::cout << "Time interval is " << maxTime - startTime << " s" << std::endl;	
	std::cout << "Enter number of time iterations: ";
	std::cin >> timeIterations;
	std::cout << "Enter output file name: ";
	std::cin >> s;
	
	gridModel.generateTimedCsv(s, startTime, maxTime, timeIterations, 500);
	
	return 0;
}

