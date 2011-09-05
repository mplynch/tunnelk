void gplot(const char*              filename, 
           vector< vector<double> > node, 
           vector< vector<int> >    tri);

void tplot(const char*              filename, 
           vector<double>           x, 
           vector<double>           y, 
           vector< vector<double> > Q,
           vector< vector<int> >    tri);

vector<double> pplot(const char*              filename, 
                     double                   alpha,
                     vector<int>              surf_node,
                     vector< vector<double> > &node,
                     vector< vector<double> > &node_B,
                     vector< vector<double> > &Q,
                     vector< vector<double> > &Q_B,
                     vector<double>           &Qfree,
                     vector<double>           &liftdrag_B);
