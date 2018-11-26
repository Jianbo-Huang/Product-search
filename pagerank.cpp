#include <thread>
#include <future>
#include "pagerank.h"

using namespace mlearn;
//#include "test.H"

void printUsage(int argc, char **argv) {
      cout<<"Usage: "<<argv[0]<<"inputfile  options....\n\
       where: inputfile is the path to the file contains the corpus text.\n\
       options are optional parameters for running the search model"<<endl;
        cout<<"Available options are:\n";
        cout<<"       m=value, where m is the number of items to show on screen of the top search results, default is m=6."<<endl;
        cout<<"       score_scheme=value, where score_scheme  is the scheme used to rank the relevance \n";
        cout<<"       category items against the search query, default is keymatch."<<endl;
        cout<<"       threads=value,where thread is the number of threads to use for build the search system.\n";
        cout<<"       default is to use all the processors available on the machine.\n";
   }
   
 
 void call_from_thread(int id,int istart,int iend,Catlog &catlog,vector<string> &contents) {
        catlog.setitems(contents,istart,  iend,id) ;
    }

int main(int argc, char **argv) {
   if (argc<2) {
       printUsage(argc,argv);
       return -1;
   }
   string s0=argv[1];
  if (s0=="-h" || s0=="--help" || s0=="-help") {
        printUsage(argc,argv);
        return -1;
     }
     cout<<"argv1:"<<argv[1]<<endl;
    int m=6;
    unsigned int nthread0= std::thread::hardware_concurrency();
    unsigned int nthreads=1;
    
    cout<<"threads available:"<<nthread0<<endl;
    string score_scheme="keymatch"; //score the item based on keyword match, another option is tf-idf
   string ifile=argv[1];
   vector<string> keys;
   vector<string> vals;
   if (argc>2) {
          for (int i=2;i<argc;i++) {
             string s=argv[i];
             vector<string> vtmp;
             split(vtmp,s,"=");
             if (vtmp.size() !=2) continue;
             string key=vtmp[0];
             string val=vtmp[1];
             keys.push_back(key);
             vals.push_back(val);
         }
  }

  for (unsigned i=0; i<keys.size();i++) {
      if (keys[i]=="m") m=atoi(vals[i].c_str());
      if (keys[i]=="score_scheme") score_scheme=vals[i];
       if (keys[i]=="threads") nthreads=atoi(vals[i].c_str());
       if (nthreads<1) nthreads=1;
       if (nthreads>nthread0) nthreads=nthread0;
 }
 cout<<"m:"<<m<<endl;
cout<<"Building search system, please wait....."<<endl;
cout<<"Number of threads used: "<<nthreads<<endl;

clock_t  time_start = clock();
  Catlog catlog(m);
  catlog.score_scheme=score_scheme;
  vector<string> contents;
  catlog.readfile(ifile,contents);
   int nn=contents.size()/nthreads ;
   catlog.items.resize(contents.size());

   if (nthreads>1) {
       std::thread t[nthreads];
      for (int i=0; i<nthreads ;i++) {
          int istart=i*nn;
          int iend=(i+1)*nn;
          if (i==nthreads-1) iend=contents.size();      
          t[i]=std::thread (call_from_thread,i,istart,iend,std::ref(catlog),std::ref(contents));
      }//for i
 
    //Join the threads with the main thread
    for (int i = 0; i < nthreads; ++i) {
         t[i].join();
    }
} else {
   catlog.setitems(contents);
}
clock_t  time_end = clock();
cout<<"elapse time:"<<(time_end-time_start)/CLOCKS_PER_SEC<<" seconds."<<endl;
 
  /*
  ipad
ipad mini
red kettle
bosch dishwasher
lg tv 32 inch
cushions
oven safe bowl
*/
int run=1;
string s1;
while (run>0) {
    cout<<"Please input query string,words are separated by space, Press return key to end input,  input -1 to end session."<<endl;
    getline(cin,s1,'\n');
    if (s1=="-1") {
       run=-1;
       break;
    }
    Query que(s1);
    cout<<"get rating...."<<endl;
    catlog.get_rating(que);
    
    string resfile=word_join(s1)+".txt";
    ofstream os(resfile.c_str());
    ofstream os1("scorefile.tex");
    catlog.show_importance(os1);
     cout<<"top "<<m<<" search results are as below:\n";
      catlog.show_search(cout,que);
      catlog.show_all_search(os,que);
      cout<<"full search results are written to file :"<<resfile<<endl;
   
}

    return 0;
   }
