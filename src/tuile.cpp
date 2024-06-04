#include "tuile.h"
#include <QString>

void Tuile::tourner() {
	//maj du tableau de minituiles
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 3; j > 0; j--) {
			swap(minituiles[j * 3 + i], minituiles[(j - 1) * 3 + i]);
		}
	}

	//maj des liens
	for (std::vector<std::vector<unsigned int>>::iterator v = liens.begin(); v != liens.end(); ++v) {
		for (std::vector<unsigned int>::iterator it = v->begin(); it != v->end(); ++it) {
			if (*it != 12)
				*it = (*it + 3) % 12;
		}
	}

    ++rotation;
    rotation %= 4;
}

void Tuile::reset() {
    while(rotation != 0) {
        tourner();
    }
}

Tuile::Tuile(const std::array<std::string, 13>& minituiles_,const std::vector<std::vector<unsigned int>>& liens_, bool blason_,const string& c) : liens(liens_), blason(blason_),chemin(c) {
	FabriqueMinituile f;
	for (unsigned int i = 0; i < 13; i++) {
		std::unique_ptr<Minituile> temp(f.methodeFabrication(minituiles_[i]));
		swap(minituiles[i], temp);
	}
}

bool Tuile::possedeMinituile(const Minituile& m) {
    for (std::array<std::unique_ptr<Minituile>, 13>::const_iterator it = minituiles.cbegin(); it != minituiles.cend(); it++) {
		//on trouve la minituile
		if (&m == it->get()) {
			return true;
		}
	}
	return false;
}

int Tuile::getIndiceMinituile(const Minituile& m) {
	//renvoie -1 si la tuile ne contient pas la minituile
	if (possedeMinituile(m)) {
		for (unsigned int i = 0; i < 13; i++) {
			//on trouve la minituile
			if (&m == minituiles[i].get()) {
				return i;
			}
		}
	}
	return -1;
}

void Tuile::affiche() const {
	std::array<std::string, 13> tab;
	Pre pr;
	Ville vi;
	Route ro;
	Riviere ri;
	Abbaye ab;
	Jardin ja;
	for (unsigned int i = 0; i < 13; i++) {
		if (*(minituiles[i].get()) == pr) {
			tab[i] = "pr";
		}
		else if (*(minituiles[i].get()) == vi) {
			tab[i] = "vi";
		}
		else if (*(minituiles[i].get()) == ro) {
			tab[i] = "ro";
		}
		else if (*(minituiles[i].get()) == ri) {
			tab[i] = "ri";
		}
		else if (*(minituiles[i].get()) == ab) {
			tab[i] = "ab";
		}
		else if (*(minituiles[i].get()) == ja) {
			tab[i] = "ja";
		}
		else {
			tab[i] = "er";
		}
	}
	cout << endl << endl << "Affichage :" << endl << endl;
	cout << "  |" << tab[0] << "|" << tab[1] << "|" << tab[2] << "|" << endl;
	cout << "--|--|--|--|--" << endl;
	cout << tab[11] << "|  |  |  |" << tab[3] << endl;
	cout << "--|--|--|--|--" << endl;
	cout << tab[10] << "|  |" << tab[12] <<  "|  |" << tab[4] << endl;
	cout << "--|--|--|--|--" << endl;
	cout << tab[9] << "|  |  |  |" << tab[5] << endl;
	cout << "--|--|--|--|--" << endl;
	cout << "  |" << tab[8] << "|" << tab[7] << "|" << tab[6] << "|" << endl;
}


std::vector<unsigned int> Tuile::get_liens(unsigned int pos) const {
	bool trouve = false;
	std::vector<unsigned int> res;
	for (auto vecteur : liens) {
		for (unsigned int i : vecteur) {
			if (i == pos) {
				res = vecteur;
				trouve = false;
				break;
			}
		}
		if (trouve) break;
	}
	return res;
}

void affiche_liens(std::vector<unsigned int> v) {
	for (unsigned int i : v) {
		std::cout << i << "  ";
	}
	cout << endl;
}

std::list<Minituile*> Tuile::getMinituilesVoisines(const Minituile& minituile) const{
    std::list<Minituile*> res;
    for(size_t i = 0; i< 13; i++){
        if(minituiles[i].get() == &minituile){
            if(i == 12){
                res.push_back(minituiles[1].get());
                res.push_back(minituiles[4].get());
                res.push_back(minituiles[7].get());
                res.push_back(minituiles[10].get());
            }
            else if(i == 0){
                res.push_back(minituiles[1].get());
                res.push_back(minituiles[11].get());
            }
            else if(i == 11){
                res.push_back(minituiles[0].get());
                res.push_back(minituiles[10].get());
            }
            else{
                if(i%3 == 1){
                    res.push_back(minituiles[12].get());
                }
                res.push_back(minituiles[i+1].get());
                res.push_back(minituiles[i-1].get());
            }
        }
    }
    return res;
}
