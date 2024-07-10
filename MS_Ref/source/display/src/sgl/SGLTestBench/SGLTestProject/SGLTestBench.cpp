// SGLTestBench.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "test_bench.h"


int main(int argc, char* argv[])
{
	TEST_DrawLine("..\\output\\line.bmp");
	TEST_DrawPolygon("..\\output\\polygon.bmp");
	TEST_DrawTexturePlane("..\\output\\texture_plane.bmp");
	return 0;
}

