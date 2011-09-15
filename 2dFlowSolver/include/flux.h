vector<double> fluxp(vector<double> Q, double nx, double ny, double len);
vector<double> fluxm(vector<double> Q,double nx, double ny, double len);

vector< vector<double> > dfluxp(vector<double> Q,double nx, double ny, double len);
vector< vector<double> > dfluxm(vector<double> Q,double nx, double ny, double len);

vector< vector<double> > dfluxWall(vector<double> Q, double nx, double ny, double len);

vector<double> fluxV(int n1, int n2, int n3,
                     vector< vector<double> > &node,
                     vector< vector<double> > &Q,
                     vector<double> &Qfree,
                     double mu_inf,
                     double M_inf,
                     double nx_face, double ny_face, 
                     double len);

void dfluxV(int n1, int n2, int n3,
            vector< vector<double> > &node,
            vector< vector<double> > &Q,
            vector<double> &Qfree,
            double mu_inf,
            double M_inf,
            double nx_face, double ny_face, 
            double len,
            vector< vector<double> > &dfvisc1,
            vector< vector<double> > &dfvisc2,
            vector< vector<double> > &dfvisc3);

vector<double> fluxp_B(vector<double> Q, 
                       double nx, double ny, double len,
                       double nx_B, double ny_B, double len_B);

vector<double> fluxm_B(vector<double> Q,
                       double nx, double ny, double len,
                       double nx_B, double ny_B, double len_B);
