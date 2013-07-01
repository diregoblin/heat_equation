#include "stdafx.h"
#include "GridModel.h"


GridModel::GridModel(double xStart, double xFin, int xNodes, double thermalDiffusivity)
	: xStart(xStart), xFin(xFin), xNodes(xNodes), heatGrid(xNodes), dPart(xNodes), thermalDiffusivity(thermalDiffusivity)
{
	construct();
}

void GridModel::construct()
{
	xLength = xFin - xStart;			
	xStep = xLength / (xNodes - 1);
	partZero();
}

GridModel::~GridModel(void)
{
}

void GridModel::partZero()
{
	for(int i = 0; i < xNodes; i++)
	{
		heatGrid[i] = 0;
	}
}

void GridModel::partAddTrapezoids(double heat, double warmWidth, double coldWidth, double transitionWidth)
{
	for(int i = 0; i < xNodes; i++)
	{
		double periodicCoord = fmod(i * xStep + coldWidth / 2, warmWidth + coldWidth + 2 * transitionWidth);
		if(periodicCoord < coldWidth)
		{
			//Do nothing
		}
		else
		{
			if(periodicCoord < coldWidth + transitionWidth)
				heatGrid[i] += heat * (periodicCoord - coldWidth) / transitionWidth;
			if((periodicCoord >= coldWidth + transitionWidth) && (periodicCoord < coldWidth + warmWidth + transitionWidth))
				heatGrid[i] += heat;
			if(periodicCoord >= coldWidth + warmWidth + transitionWidth)
				heatGrid[i] += heat * (coldWidth + warmWidth + 2 * transitionWidth - periodicCoord) / transitionWidth;
		}
	}
}

//Решение с заданными временами старта, финиша и числом итераций методом iterate()
void GridModel::solve(const double startTime, const double maxTime, const int timeIterations)
{
	this->startTime = startTime;
	this->maxTime = maxTime;
	this->timeIterations = timeIterations;
	tStep = (maxTime - startTime) / timeIterations;
	for(int i = 0; i < timeIterations; i++)
	{
		iterate();
	}
}

//Явная итерация
void GridModel::iterate()
{
	for(int i = 0; i < xNodes; i++)
	{
		dPart[i] = 0;
	}
	double partChange;
	for(int i = 1; i < xNodes; i++)
	{
		partChange = tStep * (thermalDiffusivity * (heatGrid[i-1] - heatGrid[i]) / (xStep * xStep));
		dPart[i-1] -= partChange;
		dPart[i] += partChange;
	}
	for(int i = 0; i < xNodes; i++)
	{
		heatGrid[i] = heatGrid[i] + dPart[i];
		heatGrid[0] = 0;
		heatGrid[xNodes - 1] = 0;
	}
}

void GridModel::generateTimedCsv(const std::string outputName, const double startTime, const double maxTime, const int timeIterations, const int frameLength)
{
	std::stringstream s;
	s << "./Results/" << outputName << ".csv";
	std::ofstream out(s.str());
	out << "x coord, t coord, heat" << std::endl;

	double currentTime = startTime;
	double frameTime = (maxTime - startTime) * frameLength / timeIterations;

	while (currentTime < maxTime)
	{
		for(int i = 0; i < xNodes; i++)
		{
				out << xStart + i * xStep << ", " << currentTime << ", "  << heatGrid[i] << std::endl;
		}
		solve(currentTime, currentTime + frameTime, frameLength);
		currentTime += frameTime;
		std::cout << "Current time: " << currentTime << " out of " << maxTime << std::endl;
	}
}