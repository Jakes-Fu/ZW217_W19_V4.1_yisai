#ifndef _POINTS_H_
#define _POINTS_H_

#pragma warning( push, 3 )
#include <vector>
#pragma warning( pop )
#pragma warning( disable : 4018)
using namespace std;

typedef struct SSinglePoint
{
	SSinglePoint(){x = 0; y = 0; idx=0;};
	SSinglePoint(double _x, double _y){x = _x; y = _y; idx=0;};
	SSinglePoint(double _x, double _y ,ULONG _idx){x = _x; y = _y; idx = _idx;};
	double x;
	double y;
    ULONG  idx;

    double reserve;
}SSinglePoint;

class CPointsCollection
{
	public:
		CPointsCollection(BOOL b_sort_x = FALSE, BOOL b_keep_same_x = TRUE);
		CPointsCollection(CPointsCollection* pcol);
		virtual ~CPointsCollection();

		int AddPoint(SSinglePoint* csp, BOOL bReScan, int* res);
		int AddPoint(double _x, double _y, BOOL bReScan, int* res, int _idx =0);

		void RemovePoint(int index, BOOL bReScan);
		void RemovePointX(double x, BOOL bReScan);
		void RemovePointY(double y, BOOL bReScan);
		void RemoveAll();

		int EditPoint(int index, double x, double y, BOOL bRescan);
		int InsertPoint(int index, double x, double y, BOOL bRescan);

		int GetPoint(int index, double* x, double *y);
		int GetPoint(int index, SSinglePoint* result)
		{
		    *result = points.at(index);
		    return index;
		};

		int GetNearestPoint(double _x, double _y, SSinglePoint* result);

		int GetSize(){return points.size();};
		void RescanPoints();

		vector<SSinglePoint> points;
		DWORD Flags;
		double max_x, max_y, min_x, min_y;
		BOOL bSortX, bKeepSameX;

};

#endif