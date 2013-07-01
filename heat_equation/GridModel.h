#pragma once
class GridModel
{
	class DataArray
	{
		double *dArray;
		int size;
	public:
		DataArray(int size = 100)
			: size(size)
		{
			dArray = new double[size];
		}
		double &operator[](int i)
		{
			return dArray[i];
		}
		~DataArray(void)
		{
			delete dArray;
		}
	};

	double xStart;			
	double xFin;			
	double xLength;			
	double xStep;	//dx, xStep
	int xNodes;

	double tStep;
	double startTime;						
	double maxTime;						
	int timeIterations;

	double thermalDiffusivity;

	DataArray heatGrid, dPart;

	void construct();
	void iterate();

public:
	GridModel(double xStart, double xFin, int xNodes, double thermalDiffusivity);
	~GridModel(void);

	void partZero();
	void partAddTrapezoids(double heat, double warmWidth, double coldWidth, double transitionWidth);
	void solve(const double startTime, const double maxTime, const int timeIterations);
	
	void generateTimedCsv(const std::string outputName, const double startTime, const double maxTime, const int timeIterations, const int frameLength);
};

