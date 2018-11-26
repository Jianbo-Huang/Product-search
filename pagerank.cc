#include "pagerank.h"
#include <omp.h>
using namespace mlearn;

std::string mlearn::dateNow() {
      time_t  t=std::time(0);
     std::tm *now=std::localtime(&t);
     int yy=now->tm_year+1900;
     int mm=now->tm_mon+1;
     int dd=now->tm_mday;
    std::string s=to_string(yy)+"-"+to_string(mm)+"-"+to_string(dd);
    return s;
}

double  Item::total_title_score=100;
double  Item::total_desc_score=20;
double Item::total_type_score=20;

std::string mlearn::current_time() {
   /*
  int tm_sec;   // seconds of minutes from 0 to 61
   int tm_min;   // minutes of hour from 0 to 59
   int tm_hour;  // hours of day from 0 to 24
   int tm_mday;  // day of month from 1 to 31
   int tm_mon;   // month of year from 0 to 11
   int tm_year;  // year since 1900
   int tm_wday;  // days since sunday
   int tm_yday;  // days since January 1st
   int tm_isdst; // hours of daylight savings ti
*/
   
      time_t  t=std::time(0);
     std::tm *now=std::localtime(&t);
     int yy=now->tm_year+1900;
     int mm=now->tm_mon+1;
     int dd=now->tm_mday;
     int thour=now->tm_hour;
     int tmin=now->tm_min;
     int sec=now->tm_sec;
    std::string s=to_string(yy)+"-"+to_string(mm)+"-"+to_string(dd)+"-"+to_string(thour)+"-"+
    to_string(tmin)+"-"+to_string(sec);
         return s;
}




vector<string> Query::stopwords;
string  Query::stop_words="\
a,able,about,above,abst,accordance,according,accordingly,across,act,actually,added,\
adj,affected,affecting,affects,after,afterwards,again,against,ah,all,almost,\
alone,along,already,also,although,always,am,among,amongst,an,and,\
announce,another,any,anybody,anyhow,anymore,anyone,anything,anyway,anyways,anywhere,apparently,approximately,\
are,aren,arent,arise,around,as,aside,ask,asking,at,auth,available,away,awfully,\
b,back,be,beautiful,became,because,become,becomes,becoming,been,before,beforehand,begin,beginning,beginnings,begins,\
behind,being,believe,below,beside,besides,between,beyond,biol,both,brief,briefly,but,by,\
c,ca,came,can,cannot,can\'t,cause,causes,certain,certainly,co,com,come,\
comes,contain,containing,contains,could,couldnt,d,date,did,didn\'t,different,do,does,\
doesn\'t,doing,done,don\'t,down,downwards,due,during,e,each,ed,edu,effect,eg,eight,eighty,\
either,else,elsewhere,end,ending,enough,especially,et,et-al,etc,even,\
ever,every,everybody,everyone,everything,everywhere,ex,except,f,far,few,\
ff,fifth,first,five,fix,followed,following,follows,for,former,formerly,forth,found,\
four,from,further,furthermore,g,gave,get,gets,getting,give,given,gives,giving,go,\
goes,gone,got,gotten,h,had,happens,hardly,has,hasn't,have,haven't,having,he,\
hed,hence,her,here,hereafter,hereby,herein,heres,hereupon,hers,herself,hes,\
hi,hid,him,himself,his,hither,home,how,howbeit,however,hundred,i,id,ie,if,i\'ll\
im,immediate,immediately,importance,important,in,inc,indeed,index,information,\
instead,into,invention,inward,is,isn\'t,it,itd,it\'ll,its,itself,i\'ve,\
j,just,k,keep keeps,kept,kg,km,know,known,knows,l,largely,last,\
lately,later,latter,latterly,least,less,lest,let,lets,like,\
liked,likely,line,little,\'ll,look,looking,looks,ltd,m,\
made,mainly,make,makes,many,may,maybe,me,mean,means,meantime,meanwhile,merely,mg,might,million,\
miss,ml,more,moreover,most,mostly,mr,mrs,much,mug,must,my,myself,n,\
na,name,namely,nay,nd,near,nearly,\
necessarily,necessary,need,needs,neither,never,nevertheless,new,next,nine,ninety,no,\
nobody,non,none,nonetheless,noone,nor,normally,nos,not,noted,nothing,now,\
nowhere,o,obtain,obtained,obviously,of,off,often,oh,ok,\
okay,old,omitted,on,once,one,ones,only,onto,or,ord,other,others,otherwise,\
ought,our,ours,ourselves,out,outside,over,overall,owing,own,\
p,page,pages,part,particular,particularly,past,per,perhaps,placed,please,\
plus,poorly,possible,possibly,potentially,pp,predominantly,present,pretty,\
previously,primarily,probably,promptly,proud,provides,put,q,que,quickly,\
quite,qv,r,ran,rather,rd,re,readily,really,recent,recently,ref,refs,regarding,regardless,regards,\
related,relatively,research,respectively,resulted,resulting,results,right,run,s,said,same,\
saw,say,saying,says,sec,section,see,seeing,seem,seemed,seeming,seems,seen,self,\
selves,sent,seven,several,shall,she,shed,\
she\'ll,shes,should,shouldn\'t,show,showed,shown,showns,shows,\
significant,significantly,similar,similarly,since,six,\
slightly,so,some,somebody,somehow,someone,somethan,something,\
sometime,sometimes,somewhat,somewhere,soon,sorry,specifically,specified,specify,specifying,still,stop,strongly,sub,\
substantially,successfully,such,sufficiently,suggest,sup,sure t,take,taken,\
taking,tell,tends,th,than,thank,thanks,thanx,that,that\'ll,thats,\
that\'ve,the,their,theirs,them,themselves,then,thence,there,thereafter,\
thereby,thered,therefore,therein,there\'ll,thereof,therere,theres,thereto,thereupon,\
there\'ve,these,they,theyd,they\'ll,theyre,they\'ve,\
think,this,those,thou,though,thoughh,thousand,throug,through,throughout,thru,thus,\
til,tip,to,together,too,took,toward,towards,tried,tries,truly,try,trying,ts,\
twice,two,u,un,under,unfortunately,unless,unlike,unlikely,until,unto,up,upon,ups,us,use,\
used,useful,usefully,usefulness,uses,using,usually,v,\
value,various,\'ve,very,via,viz,vol,vols,vs,w,want,wants,\
was,wasnt,way,we,wed,welcome,we\'ll,went,were,werent,\
we\'ve,what,whatever,what'll,whats,when,whence,whenever,where,whereafter,\
whereas,whereby,wherein,wheres,whereupon,wherever,whether,which,\
while,whim,whither,who,whod,whoever,whole,who\'ll,\
whom,whomever,whos,whose,why,widely,\
willing,wish,with,within,without,wont,words,world,\
would,wouldnt,www,x,y,yes,yet,you,youd,you\'ll,your,youre,yours,yourself,yourselves,you\'ve,z,zero ";


  void Catlog::setitems(vector<string> &contents,int istart, int iend,int  ithread) {
   vector<string> vtmp;
    for (unsigned i=istart;i<iend;i++) {
      string delim="\"";
      string s1=contents[i];
       split(vtmp,s1,delim);
       if (vtmp.size()!=7) continue;
       float d=atof(vtmp[6].c_str());
       Item item(s1,vtmp[2],vtmp[4],d,vtmp[0]);
        items[i]=item;
        //cout<<"setting items from :"<<istart<<" to "<<iend<<" for thread "<<ithread<<endl;
            
       }
       return;
}

vector<string> Item::descwords={"unit","units","case","set","sets","cover","box","boxes","sheet","bag"};
 Query::Query(const  vector<string > &vque) {
       remove_stopwords(vque,words);
    }

void Catlog::get_effective_items(const Query &que) {
   
   effective_items.resize(0);
    for (unsigned i=0; i<items.size();++i) {
        if (items[i].contains(que.words)) effective_items.push_back(i);
    }
    cout<<"number of  effective items found:"<<effective_items.size()<<endl;
}
       
void Query::set_stopword_vector() {
       if (stopwords.size()>0) {
            //already set
            return;
        }
        split(stopwords,stop_words,',');
        sort(stopwords.begin(),stopwords.end());
   }
   
void  Query::remove_stopwords(const vector<string> &vec,vector<string> &newlist) {
    newlist.resize(0);
   if (vec.size()<4) {
      for (unsigned i=0;i<vec.size();i++) {
           newlist.push_back(vec[i]);
      }
      return;
   }
     if (stopwords.empty()) {
           set_stopword_vector();
      }
     
      for (unsigned i=0;i<vec.size();i++) {
         bool isstopword=binary_search(stopwords.begin(),stopwords.end(),vec[i]);
         if (!isstopword) {
            newlist.push_back(vec[i]);
         }
     }
     return;
   }

int Item::findfreq(const std::string &wd) {
   //vdesc is a sorted array, loop to the first unmatch string
    auto it0=find(vdesc.begin(),vdesc.end(),wd);
    int nf=nfreq[wd];
     if (it0==vdesc.end()) return nf;
     for (auto it=it0+1;it!=vdesc.end();++it) {
           if (*it!=wd) break;
           nf++;
     }
     return nf;
}

 int Query::findtfreq(Item &item) {
       int tfreq=0;
       for (unsigned i=0;i<words.size();++i) {
          int ntf=item.findfreq(words[i]);
          tfreq+=ntf;
      }

       return tfreq;
    }

  void Item::desc_score (const vector<std::string> &qwords) {
      
        vector<int> tmatch(qwords.size(),0);
     
     for (unsigned i=0;i<qwords.size();i++) {
           for (unsigned k=0;k<vdesc.size();k++) {
                if (vdesc[k]==qwords[i]) {
                    tmatch[i]=1;
               }
          }//for k
     }//for i
     int tsum=0;
     for (unsigned s=0;s<tmatch.size();s++) {
          tsum+=tmatch[s];
     }
     descscore=total_desc_score*float(tsum)/float(qwords.size());
   }
       
 void Catlog::gettf(Query &que) {
    if (score_scheme=="tf-idf") {
        for (unsigned i=0;i<effective_items.size();++i) {
           int id=effective_items[i];
           int sumfr=que.findtfreq(items[id]);
           tfreq[id]=sumfr;
         //cout<<"term frequency:"<<id<<" "<<tfreq[id]<<endl;
       }
    } else {
          //by key match
          for (unsigned i=0;i<effective_items.size();++i) {
              int id=effective_items[i];
              items[id].desc_score (que.words);
              tscore[id]=items[id].typescore+items[id].titlescore+items[id].descscore;
         //cout<<"term frequency:"<<id<<" "<<tfreq[id]<<endl;
       }
   }
  return;
}

Query::Query(const std::string &que) {
       word=que;
       vector<string> vtmp;
       split(vtmp,que," ");
      remove_stopwords(vtmp,words);
   
   }
   
Catlog::Catlog(const string &tfile,int m) {
       topm=m;
        score_scheme="keymatch"; //keymatch 
         Query::set_stopword_vector();
         vector<string> contents;
         if (tfile!="none") {
            readfile(tfile,contents);
         }
       setitems(contents);
    }

void Catlog::show_importance(ostream &os) {
       os<<"Ranking results for query:\" "<<query->word<<"\""<<endl;
        os<<"Score scheme used: "<<score_scheme<<endl;
        os<<"id;item_id;title;score;price"<<endl;
      for (unsigned i=0;i<importance.size();i++) {
              int id=importance[i].first;
                os<<i<<";"<<id<<";" <<items[id].title()<<";"<<importance[i].second<<";"<<items[id].price<<endl;
       }

   }

void Item::write(ostream &os) {
   vector<string> vitem;
   string delim="\",";
   split(vitem,sitem,delim);
   if (vitem.size()<3) {
        //something wrong, skip
        return;
   }
   os<<"Category: "<<vitem[0]<<endl;
   os<<"Title: "<<vitem[1]<<endl;
   os<<"Description: \n   "<<vitem[2]<<endl;
   int last=vitem.size()-1;
   if (last>3) {
         string ss;
         for (unsigned i=3;i<last;i++) {
             ss+=vitem[i]+" ";
             if (ss.size()>=60) {
                os<<"  "<<ss<<endl;
                ss="";
             }
        }
        if (ss.size()>=1) {
           os<<ss<<endl;
        }
    }
    os<<"Price: "<<vitem[last]<<endl<<endl;
      return;
   }
   
void Catlog::readfile(const string &tfile,vector<string> &contents)  {
     ifstream is(tfile.c_str());
     if (!is) {
        cout<<"error read input file,"<<tfile<<" cannot open for reading."<<endl;
        return;
     }
     string s1;

     getline(is,s1,'\n');//get header line, ignore
     
   
     
     while (!is.eof()) {
        getline(is,s1,'\n');
        if (s1.size()<3) continue;
        contents.push_back(s1);
    }
cout<<"contents:"<<contents.size()<<endl;

       return;
   }

void Catlog::setitems(vector<string> &contents) {
    items.resize(contents.size());
      vector<string> vtmp;
    for (unsigned i=0;i<contents.size();i++) {
      string delim="\"";
      string s1=contents[i];
       split(vtmp,s1,delim);
       if (vtmp.size()!=7) continue;
       float d=atof(vtmp[6].c_str());
      Item item(s1,vtmp[2],vtmp[4],d,vtmp[0]);
      //cout<<"type:"<<vtmp[0]<<"   title:"<<vtmp[2]<<endl;
      items[i]=item;
   }
 cout<<"number of items read:"<<items.size()<<endl;
}

 void Catlog::show_search(ostream &os,Query &que) {
       int m=topm;
       if (m>effective_items.size() ||m==-1) {
          m=effective_items.size();
      }
       if (m==0) {
          cout<<"no search results for query :\""<<que.word<<" \" are found."<<endl;
         return;
      }

      if (m<effective_items.size()) {
          os<<"showing first  "<<m<< "  results for query \""<<que.word<<"\":\n";
      } else {
         os<<"showing  all  "<<m<< "  results for query \""<<que.word<<"\":\n";
      }
      os<<"Score scheme used for ranking: "<<score_scheme<<endl;
      for (int  i=0;i<m;i++) {
         int id=importance[i].first;
           items[id].write(os);
       }
       return;
    }
 void Catlog::show_all_search(ostream &os,Query &que) {
         int m=effective_items.size();
       if (m==0) {
          cout<<"no search results for query :\""<<que.word<<" \" are found."<<endl;
         return;
      }
      os<<"showing  all  "<<m<< "  results for query \""<<que.word<<"\":\n";
      for (int  i=0;i<m;i++) {
         int id=importance[i].first;
           items[id].write(os);
       }
       return;
    }
 void Catlog::get_rating(Query &que) {
         query=&que;
         get_effective_items(que);
         cout<<"number of effective items:"<<effective_items.size()<<endl;
         gettf(que);
         
         int N=items.size(); //total number of documents
         int  n=effective_items.size(); //number of documents contains one of more of the query words
          importance.resize(n);
       
         for (int  i=0; i<n;i++) {
             int id=effective_items[i];
             int nfr=0;
             double w=0;
             if (score_scheme=="tf-idf") {
                nfr= tfreq[id];
               w=float(nfr)*log10(double(N)/double(n)) ; //TF-IDF importance rating
            } else {
                 w=tscore[id];
            }
             pair<int,double> pval(id,w);
              importance[i]=pval;
         }
         auto sortfun=[] (const pair<int,double> &p1,const pair<int,double> &p2) {
                return p1.second>p2.second;
         };
         sort(importance.begin(),importance.end(),sortfun); //sort in decending prder
         
         return;
    }

