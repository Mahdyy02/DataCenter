#include <bits/stdc++.h>
using namespace std;

class EspacePS;

namespace IntervalFloat{
    class zoneDeStckage{
    public:
        zoneDeStckage(float val) {
            if (val < 0.0f || val > 10.0f){
                cerr<<"Valeur doit etre dans l\'intervalle [0, 10]"<<endl;
                exit(EXIT_FAILURE);
            }
            value = val;
        }

        float getValue() const{
            return value;
        }

        operator float() const{
            return value;
        }

    private:
        float value;
    };
}

namespace zds{
    using zoneDeStckage = IntervalFloat::zoneDeStckage;
}

class Fichier{
public:
    void setNom(string nom){
        this->nom = nom;
    }

    void setExtension(string extension){
        this->extension = extension;
    }

    void setTaille(float t){
        this->taille = t;
    }

    void setPeripheriqueDeStockage(EspacePS* eps){
        this->prepheriqueDeStockage = eps;
    }

    string getExtension(){return this->extension;}

    friend bool operator<(const Fichier& f1, const Fichier& f2){
        return f1.taille < f2.taille;
    }

    friend std::ostream& operator<<(std::ostream& os, const Fichier& f) {
        os << f.nom << "." << f.extension << " : " << f.taille << "KO";
        return os;
    }

private:
    string nom;
    string extension;
    float taille;
    EspacePS* prepheriqueDeStockage;
};

class EspacePS{
public:
    EspacePS() = default;

    virtual ~EspacePS(){
        for (Fichier* f : fichiers) delete f;
        fichiers.clear();  
    };

    float getEspacePSStockage(){return this->capacite;}
    float getEspaceLibre(){return this->capacite - this->espaceOccupe;}

    void supprimerFichier(string extension, int nobmreASupprime){
        int nombreDeFichiersSupprimes = 0;
        for(int i = 0; i < this->fichiers.size(); ++i){
            if(this->fichiers[i]->getExtension() == extension){
                this->fichiers.erase(this->fichiers.begin() + i);
                nombreDeFichiersSupprimes++;
                if(nombreDeFichiersSupprimes == nobmreASupprime) break;
            }
        }
    }

    void explorerExtension(string ext) {
        for (int i = 0; i < this->fichiers.size(); ++i) {
            if (this->fichiers[i]->getExtension() == ext) {
                cout << *this->fichiers[i] << endl;
            }
        }
    }

    void ajouterFichier(string nomLogique, string extension, float capacite){
        Fichier* f = new Fichier();
        f->setNom(nomLogique);
        f->setExtension(extension);
        f->setTaille(capacite);
        f->setPeripheriqueDeStockage(this);

        this->espaceOccupe+=capacite;

        this->fichiers.push_back(f); 
    }

    Fichier* operator[](size_t index) {
        if (index >= this->fichiers.size()) {
            cerr<<"Index invalide"<<endl;
            exit(EXIT_FAILURE);
        }
        return this->fichiers[index];
    }

    vector<Fichier*> getFichiers(){return this->fichiers;}

protected:
    float capacite;
    float espaceOccupe;
    string nomLogique;
    vector<Fichier*> fichiers;
};

class CleUSB : public EspacePS{
public:
    CleUSB(float c, float stockage): EspacePS(), zoneDeStockageUSB(stockage) {
        this->capacite = c;
        this->espaceOccupe = 0.0;
    } 

private:
    zds::zoneDeStckage zoneDeStockageUSB;
};

class DisqueDur: public EspacePS{
public:
    DisqueDur(string nom, float t, int ns, int np, int nsur): EspacePS(), 
                                                              tailleSecteur(t),
                                                              nombreDeSecteurParPiste(ns),
                                                              nombrePistesParSurface(np),
                                                              nombreDeSurfaces(nsur) 
    {
        this->nomLogique = nom;
        this->capacite = this->tailleSecteur*this->nombreDeSecteurParPiste*this->nombrePistesParSurface*this->nombreDeSurfaces;
    }

private:
    float tailleSecteur;
    int nombreDeSecteurParPiste;
    int nombrePistesParSurface;
    int nombreDeSurfaces;
};

class DataCenter{
public:

    DataCenter(){}
    ~DataCenter(){
        this->espacesPS.clear(); 
    }

    void ajouterEspacePS(EspacePS* eps){
        this->espacesPS.push_back(eps);
    }

    float espaceTotal(){
        float sommeCapacite;
        for(int i = 0; i < this->espacesPS.size(); ++i){
            sommeCapacite+=this->espacesPS[i]->getEspaceLibre();
        }
        return sommeCapacite;
    }
private:
    vector<EspacePS*> espacesPS;
};

int main(){

    DisqueDur D("C:", 512.0, 520, 1024, 16);
    D.ajouterFichier("Fichier1", "doc", 2);
    D.ajouterFichier("Fichier2", "xls", 1);
    D.ajouterFichier("Fichier3", "gif", 25);

    D.explorerExtension("doc");

    Fichier* min = D[0];
    for (int i = 0; i < D.getFichiers().size(); ++i) {
        if (*D[i] < *min) {
            min = D[i];
        }
    }
    cout << *min << endl;

    DataCenter DC;
    DC.ajouterEspacePS(&D);
    cout<<DC.espaceTotal()<<endl;
    cout<<D.getEspaceLibre()<<endl;

    return 0;
}