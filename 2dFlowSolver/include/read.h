void readmesh(char*                           filename,
              vector< vector<double> >        &p_node,
              vector< vector<int> >           &p_trivec,
              vector< vector<int> >           &p_quadvec,
              vector< vector< vector<int> > > &p_bound);
 
void readCmplxmesh(char*                           filename,
                   vector< vector<double> >        &node,
                   vector< vector<double> >        &node_ImPrt,
                   vector< vector<int> >           &trivec,
                   vector< vector<int> >           &quadvec,
                   vector< vector< vector<int> > > &bound);
 
void readinput(vector<int>      &int_input, 
               vector<double>   &dub_input);
