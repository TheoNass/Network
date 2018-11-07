#include "network.h"
#include "random.h"

#include <iostream>
#include <algorithm>

using namespace std;

void Network::resize(const size_t &n) {
	vector<double> tmp (n);
	RandomNumbers rd;
	rd.normal(tmp);
	values = tmp;
}

bool Network::add_link(const size_t& a, const size_t& b) {
	/// tout d'abord je dois regarder si les nodes a et b existes
	/// en gros je dois regarder si j'ai un lien  de a à b et/ou de b à a
	/// ce qui veut dire que je dois itérer sur la multimap
	if ((values.size() <= a) or (values.size() <= b) or (a == b)) return false;  /// on verifie que les nodes existent, puis que l'on essaye pas de lier le meme node
	for(pair<size_t, size_t> elem : links) {
		if (((elem.first == a) and (elem.second ==b)) 
			or ((elem.first == b) and (elem.second ==a))) {
				return false;									/// si le lien existe deja on retourne false 
		}
	}
			
	links.insert ( std::pair<size_t,size_t>(a,b) );  /// si le lien n'existe pas je le rajoute
	links.insert ( std::pair<size_t,size_t>(b,a) );  /// dans les 2 sens
	return true;
}

size_t Network::random_connect(const double& mean) { //FAUX
	links.clear();
	size_t compteur(0);
	RandomNumbers rd;
	for (size_t i(0); i<values.size(); i++)  {   ///on parcoure les nodes
		size_t nb_links(rd.poisson(mean));
		for (size_t j(0); j<nb_links; j++) {
			size_t node(rd.uniform_double(0, values.size()));  // pb de lien avec lui meme, et pb de lien double
			if (add_link (i,node))  compteur ++;
		}
	}
	return compteur;
}

size_t Network::set_values(const vector<double>& vec) {
	size_t compteur(0);
	vector<double> tmp;
	if (vec.size()>values.size()){
		tmp = vec;
		tmp.resize(values.size());
		values=tmp;
		compteur =values.size();
	}
	else {
		for (size_t i=0; i<vec.size(); i++) {
			values[i] = vec[i];
			compteur ++;
		}
	}
		
	return compteur;
}

			
size_t Network::size() const { ///WILL WORK
	return values.size();/// return # of nodes
}

size_t Network::degree(const size_t &_n) const { /// WILL WORK
	size_t compteur(0);
	for(pair<size_t, size_t> elem : links) {
		if (elem.first == _n)  compteur++;   ///on considere 1 lien comme un lien bidirectionnel
	}
	return compteur;
}

double Network::value(const size_t &_n) const { ///WILL WORK
	return values[_n];
}
	
vector<double> Network::sorted_values() const { ///SHOULD WORK... I GUESS
	vector<double> vec = values;
	sort(vec.rbegin(), vec.rend());  /// rbegin et rend permettent d'inverser l'ordre
	return vec;
}

vector<size_t> Network::neighbors(const size_t& _n) const {
	vector<size_t> neighbors;
	for(pair<size_t, size_t> elem : links) {
		if (elem.first == _n)  {
			neighbors.push_back(elem.second);
		}
	}
	return neighbors;
}
			
