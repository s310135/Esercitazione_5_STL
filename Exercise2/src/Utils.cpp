#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolygonalLibrary
{
bool ImportMesh(PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;

}
// ***************************************************************************
bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell0Ds.csv");

    if(file.fail())
        return false;

    list<string> listlines;

    string line;
    while (getline(file, line))
        listlines.push_back(line);

    file.close();

    // remove header
    listlines.pop_front();

    mesh.NumCell0Ds = listlines.size();

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "Non vi è la cella 0D" << endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

    for (const string& line : listlines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;
		string field_value;
		vector<string> field_values;
		
		while (getline(converter, field_value, ';'))	//legge line per line
			field_values.push_back(field_value);
		
		//stoi converte la stringa in intero
		id = stoi(field_values[0]);
		marker = stoi(field_values[1]);		//marker associato al vertice
		mesh.Cell0DsCoordinates(0, id) = stod(field_values[2]);
		mesh.Cell0DsCoordinates(1, id) = stod(field_values[3]);
		
        mesh.Cell0DsId.push_back(id);
       /// Memorizza i marker
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell0Ds.find(marker);
            if(it == mesh.MarkerCell0Ds.end())
            {
                mesh.MarkerCell0Ds.insert({marker, {id}});
            }
            else
            {
                it->second.push_back(id);
            }
        }

    }
    return true;
}
// ***************************************************************************
bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell1Ds.csv");

    if(file.fail())
        return false;

    list<string> listlines;
    string line;
    while (getline(file, line))
        listlines.push_back(line);

    file.close();

    // remove header
    listlines.pop_front();

    mesh.NumCell1Ds = listlines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "Non vi è la cella 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (const string& line : listlines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertici;
		string field_value;
		vector<string> field_values;
		
		while (getline(converter, field_value, ';'))
			field_values.push_back(field_value);
		
		id = stoi(field_values[0]);
		marker = stoi(field_values[1]);
		mesh.Cell1DsExtrema(0, id) = stoi(field_values[2]);
		mesh.Cell1DsExtrema(1, id) = stoi(field_values[3]);
       
	   
        mesh.Cell1DsId.push_back(id);

        /// Memorizza i marker
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell1Ds.find(marker);
            if(it == mesh.MarkerCell1Ds.end())
            {
                mesh.MarkerCell1Ds.insert({marker, {id}});
            }
            else
            {
                // mesh.MarkerCell1Ds[marker].push_back(id);
                it->second.push_back(id);
            }
        }
    }

    return true;
}
// ***************************************************************************
bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell2Ds.csv");

    if(file.fail())
        return false;

    list<string> listlines;
    string line;
    while (getline(file, line))
        listlines.push_back(line);

    // remove header
    listlines.pop_front();

    mesh.NumCell2Ds = listlines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "Non vi è la cella 2D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2Dsvertici.reserve(mesh.NumCell2Ds);
    mesh.Cell2Dsspigolo.reserve(mesh.NumCell2Ds);

    for (const string& line : listlines)
    {
        istringstream converter(line);
        string field_value;
        vector<string> field_values;

        unsigned int id, marker, num_vertici, num_spigoli;
        vector<unsigned int> vertici;
        vector<unsigned int> spigolo;

		while (getline(converter, field_value, ';'))
		{
			field_values.push_back(field_value);	
		}
		
        id = stoi(field_values[0]);
        marker = stoi(field_values[1]);
        num_vertici = stoi(field_values[2]);
        
        for(unsigned int i = 3; i < 3 + num_vertici; i++)
            vertici.push_back(stoi(field_values[i]));
        
        num_spigoli = stoi(field_values[3 + num_vertici]);
        
        for(unsigned int i = 3 + num_vertici; i <  3 + num_vertici + num_spigoli; i++)
            spigolo.push_back(stoi(field_values[i]));

        mesh.Cell2DsId.push_back(id);
        
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell2Ds.find(marker);
            if(it == mesh.MarkerCell2Ds.end())
            {
                mesh.MarkerCell2Ds.insert({marker, {id}});
            }
            else
            {
                it->second.push_back(id);
            }
        }
        mesh.Cell2Dsvertici.push_back(vertici);
        mesh.Cell2Dsspigolo.push_back(spigolo);
        }
	file.close();
	return true;
}

//verifica che i markers siano correttamente ordinati
bool Test_markers_ordinati(PolygonalMesh& mesh)
{
    cout << "Verifica ordine dei marker: " << endl;
    bool tuttiordinati = true;

    cout << "Cell0D Markers: " << endl;
    for (const auto& pair : mesh.MarkerCell0Ds) 
	{
        cout << "  Marker " << pair.first << " : ";
        for (unsigned int id : pair.second) 
		{
            cout << id << " ";
        }
        cout << endl;
    }

    cout << "Cell1D Markers: " << endl;
    for (const auto& pair : mesh.MarkerCell1Ds) 
	{
        cout << "  Marker " << pair.first << " : ";
        for (unsigned int id : pair.second) 
		{
            cout << id << " ";
        }
        cout << endl;
    }

    cout << "Cell2D Markers: " << endl;
    for (const auto& pair : mesh.MarkerCell2Ds) 
	{
        cout << "  Marker " << pair.first << " : ";
        for (unsigned int id : pair.second) 
		{
            cout << id << " ";
        }
        cout << endl;
    }

    cout << "Tutti i marker sono ordinati" << endl;
    return true;
}

//verifica che gli spigoli non abbiano lunghezza nulla
bool Test_lunghezza_spigoli(PolygonalMesh& mesh)
{
	for(unsigned int i = 0; i<mesh.NumCell2Ds; i++)
	{
		for(unsigned int j = 0; j<mesh.Cell2Dsspigolo[i].size();j++)
		{	
			
			vector<unsigned int>& spigolo = mesh.Cell2Dsspigolo[i];
			
            //prendo le coordinate dei punti estremi dell' j-esimo edge
			int& indice_iniziale = mesh.Cell1DsExtrema(0,spigolo[j]);
			int& indice_finale = mesh.Cell1DsExtrema(1,spigolo[j]);
			
			//mi ricavo le coordinate dei punti estremi del segmento, e faccio il teorema di pitagora 
            //per ricavarne la lunghezza
			double& x_origine = mesh.Cell0DsCoordinates(0,indice_iniziale);
			double& y_origine = mesh.Cell0DsCoordinates(1,indice_iniziale);
			double& x_fine = mesh.Cell0DsCoordinates(0,indice_finale);
			double& y_fine = mesh.Cell0DsCoordinates(1,indice_finale);	
			
			double distanza = sqrt(pow(x_origine-x_fine,2)+pow(y_origine-y_fine,2));
			
			if(distanza < 1e-16)
			{
				cout<< "Il poligono " << i << " ha uno spigolo di lunghezza nulla" <<endl;
				return false;
			}
		}
	}
	cout<<"Non ci sono spigoli di lunghezza nulla"<<endl;
	return true;
}

//verifica che ogni poligono non abbia area nulla
bool Test_aree_non_nulle(PolygonalMesh& mesh)
{
	for(unsigned int i = 0; i<mesh.NumCell2Ds; i++)
	{
		double area = 0.0;
		unsigned int n = mesh.Cell2Dsvertici[i].size();
		for(unsigned int j = 0; j < n; j++)
		{
			unsigned int& id_p1 = mesh.Cell2Dsvertici[i][j];
			unsigned int& id_p2 = mesh.Cell2Dsvertici[i][(j+1)%n];
			
			double& X_p1 = mesh.Cell0DsCoordinates(0,id_p1);
			double& Y_p1 = mesh.Cell0DsCoordinates(1,id_p1);
			double& X_p2 = mesh.Cell0DsCoordinates(0,id_p2);
			double& Y_p2 = mesh.Cell0DsCoordinates(1,id_p2);
			
			area = area + X_p1 * Y_p2 - X_p2 * Y_p1;
		}
		
		area = 0.5*abs(area);
		
		if(area < 1e-12)
		{
			cout<< "Il poligono " << i << " ha area nulla"<<endl;
			return false;
		}
	}
	cout<<"Non ci sono poligoni di area nulla"<<endl;
	return true;
}
}