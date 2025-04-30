#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary
{
	bool ImportMesh(PolygonalMesh& mesh);
	bool ImportCell0Ds(PolygonalMesh& mesh);
	bool ImportCell1Ds(PolygonalMesh& mesh);
	bool ImportCell2Ds(PolygonalMesh& mesh);

	bool Test_markers_ordinati(PolygonalMesh& mesh);
	bool Test_lunghezza_spigoli(PolygonalMesh& mesh);
	bool Test_aree_non_nulle(PolygonalMesh& mesh);
}
