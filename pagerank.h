#ifndef __PAGE_RANK_H
#define __PAGE_RANK_H
#include <sstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <map>
#include <set>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <random>
#include <algorithm>
#include <stdlib.h>
#include <ctype.h>
#include <ctime>
//#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
//#include <boost/algorithm/string/split.hpp> // Include for boost::split
using namespace std;

namespace  mlearn {
 std::string dateNow();
std::string  current_time();

inline string word_join(const std::string &wd, int mxlen=20) {
   string nword;
   int len=wd.size();
   if (len>mxlen) len=mxlen;
   for (unsigned i=0;i<wd.size();++i) {
      char c=wd[i];
      if (c==' ' || c=='\n' ) c='-';
      nword.push_back(c);
   }
   return nword;
}

inline void  split
  (
   std::vector<std::string> &container, 
   const std::string  &in,
   const std::string &del
  ) {
    int len = in.length();
    int  i = 0;
    if (container.size()>=1) 
      {
        container.resize(0);
      }
    while ( i < len ) {
        // eat leading whitespace
        i = in.find_first_not_of (del, i);
        if (i <0) {
            return;   // nothing left but white space
        }
        // find the end of the token
        int  j = in.find_first_of (del, i);
        // push token
        if (j <0) {
            std::string ss=in.substr(i);
            container.push_back (ss);
            break;
        } else {
            std::string ss1=in.substr(i, j-i);
            container.push_back (ss1);
        }
        i = j + 1;
    }
  return;
}

inline void split(std::vector<string>&vec, const std::string &str, char sep=' ') {
   vec.resize(0);
    istringstream f(str);
    string s;    
    while (getline(f, s, sep)) {
       if (s!=" " &&s!="\t" &&s!="\n") {
              vec.push_back(s);
       }
    }
    return;
 }

inline int inlist(const vector<string> &list, const string &key) {
    for (unsigned i=0; i<list.size();i++) {
         if (list[i]==key) {
            return i;
        }
   }
   return -1;
}

inline void remove_punct(string &str) {
   //remove punctuation 
         char c=str[str.size()-1];
         if (c==',' || c=='.' || c==':' || c=='?' || c=='!'|| c==';') {
            str=str.substr(0,str.size()-1);
         }
   }
   
inline void tlower(std::string &str) {
   for (unsigned i=0;i<str.size();i++) {
      char c=str[i];
      str[i]=tolower(c);
   }
}

inline void replace_char(std::string &str) {
    for (unsigned i=0;i<str.size();i++) {
       if (str[i]=='&' || str[i]=='/'||str[i]=='-') {
          str[i]=' ';
      }
   }
}


 class Item  {
   public:
        string sitem;
        static vector<string> descwords;
        static double total_title_score;
        static double total_desc_score;
        static double total_type_score;
        vector<string> vtype;
        vector<string> vtitle;
        vector<string> vdesc;
        map<string,int> nfreq;
        double titlescore;//title score for the current query
        double descscore; //description score for the current query
        double typescore;
       float price;
       inline bool has_descwords(const vector<string> &qwords);
       string title() {
           vector<string> vtmp;
           split(vtmp,sitem,"\"");
           //cout<<"title:"<<vtmp.size()<<" "<<vtmp[1]<<" "<<vtmp[2]<<" "<<vtmp[3]<<endl;
           return vtmp[2];
       }
       Item(const std::string &s1,const std::string &tit, const std::string &desc, float price0,const std::string &type="none")  {
          sitem=s1;
          price=price0;
          str2words(vtitle,tit) ;
         str2words(vdesc,desc) ;
         if (type!="none") {
             str2words(vtype,type) ;
         }
        sort(vdesc.begin(),vdesc.end());
      }
   
     void desc_score (const vector<std::string> &words);
      
      int findfreq(const std::string &wd);
      void write(ostream &os=cout);
      inline void str2words(vector<string> &words,const std::string &str) ;
      inline void str2words(set<string> &words,const std::string &str) ;
       Item(){;}
       inline bool contains(const vector<string> &qwords);//check whether title contains one or more of the words in qwords
       ~Item (){;}
};


class Query {
   public:
      static string stop_words;
      static vector<string> stopwords;
      static void set_stopword_vector();
      static inline bool is_stopword(const std::string &wd);
      static void remove_stopwords(const vector<string> &vec,vector<string> &newlist);
      string word;
      int findtfreq(Item &item);//find total frequency of the query words in an item description
      
      Query(const std::string &que);
      Query(const  vector<string > &vque);
      vector<string> words;
    
      Query() {;}
      ~Query() {;}

};
 bool Query::is_stopword(const std::string &wd) {
  bool is_sword=binary_search(stopwords.begin(),stopwords.end(),wd);
    return is_sword;
 }
 
class Catlog {
   public:
      int topm;
      Query *query;
      string score_scheme; //keymatch or tf-idf
      vector<Item> items;
      vector<int> effective_items;
      vector<pair<int,double>> importance;//inportance rating
      void get_effective_items(const Query &que);  //get effective items relevant to a query
      map<int,int> tfreq; //term frequency map, get total frequency of all words in a search query words, for each of the effective items
      map<int,int> tscore; //total score of key match
      void gettf(Query &que);   //get term frequency for the query
      void get_rating(Query &que);  //get importance rating
      void readfile(const string &tfile,vector<string> &contents) ;
      
      void show_importance(ostream &os=std::cout);
      void show_search(ostream &os,Query &que); //show  most relevant results
      void show_all_search(ostream &os,Query &que);
      Catlog(const string &tfile,int m=6) ;
      void setitems(vector<string> &contents);
       void setitems(vector<string> &contents,int istart, int iend,int  ithread) ;
     
      Catlog(int m=6) {
           topm=m;
           score_scheme="keymatch";
         }
      
  ~Catlog() {;}
};

 bool Item::has_descwords(const vector<string> &qwords) {
      bool hasit=false;
      for (unsigned i=0; i<vtitle.size();i++) {
           string wd=vtitle[i];
           if (inlist(qwords,wd)>=0) {
              continue;
           }
           if (inlist(descwords,wd)>=0) {
              hasit=true;            
              break;
          }
     }
    return hasit;
 }
 
 void Item::str2words(vector<string> &words,const std::string &str) {
     //split string to words by space
   vector<string> vtmp1;
     string del=" / - & { } # ( ) , > < =";
     split(vtmp1,str,del);
      //boost::split(vtmp1, str, boost::is_any_of("/ - & { } # ( ) , > < = "), boost::token_compress_on);
     for (unsigned i=0;i<vtmp1.size();i++) {
           string s2=vtmp1[i];
           tlower(s2);
           remove_punct(s2) ;
           bool stopwd=Query::is_stopword(s2);
          if (stopwd) continue ;
          words.push_back(s2);
     }
   return;
}

void Item::str2words(set<string> &words,const std::string &str) {
     //split string to words by space
   vector<string> vtmp1;
     string del=" / - & { } # ( ) , > < =";
     split(vtmp1,str,del);
     for (unsigned i=0;i<vtmp1.size();i++) {
           string s2=vtmp1[i];
           tlower(s2);
           remove_punct(s2) ;
           bool stopwd=Query::is_stopword(s2);
          if (stopwd) continue ;
          words.insert(s2);
     }
   return;
}

 bool Item::contains(const vector<string> &qwords) {
     bool cnts=false;
     vector<int> matched(qwords.size(),0);
     vector<int> tmatch(qwords.size(),0);
     
     for (unsigned i=0;i<qwords.size();i++) {
           nfreq[qwords[i]]=0;
          for (unsigned k=0;k<vtitle.size();k++) {
              // if (inlist(vtitle,qwords[i])>=0) return true;
              if (vtitle[k]==qwords[i]) {
                    cnts=true;
                     matched[i]=1;
                    nfreq[qwords[i]]++;
              }
       }
      
       if (vtype.size()>=1) {
         for (unsigned k=0;k<vtype.size();k++) {
              if (vtype[k]==qwords[i]) {
                     tmatch[i]=1;
              }
         }
       }//if vtype
   }//for qwords
       int totmatch=0;
       for (unsigned s=0;s<matched.size();s++) {
           totmatch+=matched[s];
       }
       int typesum=0;
      for (unsigned s=0;s<tmatch.size();s++) {
           typesum+=tmatch[s];
       }
       titlescore=total_title_score*float(totmatch) /float(qwords.size());   
       typescore=total_type_score*float(typesum) /float(qwords.size());   
   // }
     if (titlescore/total_title_score>0.9) {
             //total match, reduce it if descriptive words are included
            if (has_descwords(qwords)) {
               titlescore*=0.85;
            }
     }
     return cnts;
 }


//
   }//mlearn

#endif
