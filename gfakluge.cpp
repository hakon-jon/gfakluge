#include "gfakluge.hpp"

using namespace std;
namespace gfak{
GFAKluge::GFAKluge(){
  map<std::string, std::string> header;
  map<std::string, vector<contained_elem> > seq_to_contained;
  map<std::string, vector<link_elem> > seq_to_link;
  //Since we can't compare sequence elements for hashing,
  // we cheat and use their names (which are sort of guaranteed to be
  // unique.
  map<string, sequence_elem> name_to_seq;
}

GFAKluge::~GFAKluge(){

}

// Borrow from
//http://stackoverflow.com/questions/236129/split-a-string-in-c
// Thanks StackOverflow!
vector<string> GFAKluge::split(string s, char delim){
    vector<string> ret;
    stringstream sstream(s);
    string temp;
    while(getline(sstream, temp, delim)){
        ret.push_back(temp);
    }
    return ret;

}

bool GFAKluge::parse_gfa_file(string filename){
    ifstream gfi;
    gfi.open(filename.c_str(), std::ifstream::in);
    if (!gfi.good()){
        cerr << "Invalid input stream. Exiting." << endl;
        return false;
    }
    cerr << "Reading " << filename << "..." << endl;

    string line;
    vector<string> line_tokens;
    while (getline(gfi, line)){
        vector<string> tokens = split(line, '\t');
        if (tokens[0] == "H"){
            header_elem h;
            line_tokens = split(tokens[1], ':');
            //TODO this is not well implemented
            // GFA places no guarantees on header format
            h.key = line_tokens[0];
            h.val = line_tokens[1];
            header[h.key] = h.val;
        }
        else if (tokens[0] ==  "S"){
            //TODO: we've got some tokens at the end of the line
            //that have not been handled yet.
            sequence_elem s;
            s.name = tokens[1];
            s.seq = tokens[2];
            //s.id = atol(s.name.c_str());
            name_to_seq[s.name] = s;
        }
        else if (tokens[0] ==  "L"){
            // TODO: we need to deal with links where the link is given before
            // its corresponding sequence in the file. TODO this is probably
            // now fixed by using the string: sequence map.
            link_elem l;
            l.source_name = tokens[1];
            l.sink_name = tokens[3];
            //TODO: search the input strings for "-" and "+" and set using ternary operator
            l.source_orientation_forward = tokens[2] == "+" ? true : false;
            l.sink_orientation_forward = tokens[4] == "+" ? true : false;
            //l.pos = tokens[0];
            l.cigar = tokens[5];
            add_link(l.source_name, l);
        }
        else if (tokens[0] == "C"){
            contained_elem c;
            //TODO fix token indices here
            c.source_name = tokens[1];
            c.sink_name = tokens[3];
            c.source_orientation_forward = tokens[2] == "+" ? true : false;
            c.sink_orientation_forward = tokens[4] == "+" ? true : false;
            c.pos = atoi(tokens[5].c_str());
            c.cigar = tokens[6];
            add_contained(c.sink_name, c);
        }
        else if (tokens[0] == "x"){
            annotation_elem x;
            x.key = tokens[1];
            x.info = tokens[2];
        }
        else if (tokens[0] == "a"){
            annotation_elem a;
            a.key = tokens[1];
            a.info = tokens[2];
        }
        else{
            cerr << "Unknown line identifier  encountered: " << tokens[0] <<  " . Exiting." << endl;
        }

    }

    return true;

    }

    bool GFAKluge::parse_gfa_file(fstream fs){
        cerr << "Not implemented: parse_gfa_file(fstream)" << endl; exit(1);
        return true;
    }

    void GFAKluge::add_link(sequence_elem seq, link_elem link){
        seq_to_link[seq.name].push_back(link);
    }

    void GFAKluge::add_contained(sequence_elem seq, contained_elem con){
        seq_to_contained[seq.name].push_back(con);
    }

    void GFAKluge::add_link(string seq_name, link_elem link){
        seq_to_link[seq_name].push_back(link);
    }

    void GFAKluge::add_contained(string seq_name, contained_elem con){
        seq_to_contained[seq_name].push_back(con);
    }

    vector<link_elem> GFAKluge::get_links(string seq_name){
        return seq_to_link[seq_name];
    }

    vector<link_elem> GFAKluge::get_links(sequence_elem seq){
        string seq_name = seq.name;
        return seq_to_link[seq_name];
    }

    vector<contained_elem> GFAKluge::get_contained(string seq_name){
        return seq_to_contained[seq_name];
    }

    vector<contained_elem> GFAKluge::get_contained(sequence_elem seq){
        string seq_name = seq.name;
        return seq_to_contained[seq_name];
    }

    std::string GFAKluge::to_string(){
        string ret = "";
        int i;
        //First print header lines.
        if (header.size() > 0){
          map<std::string, std::string>::iterator it;
          for (it = header.begin(); it != header.end(); it++){
              ret += "H\t" + it->first + "\t" + it->second + "\n";
            }
        }
        if (name_to_seq.size() > 0){
          map<std::string, sequence_elem>::iterator st;
          for (st = name_to_seq.begin(); st != name_to_seq.end(); st++){
              ret += "S\t" + (st->second).name + "\t" + (st->second).seq + "\n";
              //TODO iterate over links
              //L    segName1,segOri1,segName2,segOri2,CIGAR      Link
           /**   if (seq_to_link[st->first].size() > 0){
                for (i = 0; i < seq_to_link[st->first].size(); i++){
                    ret += "L\t" + seq_to_link[st->first][i].source_name + "\t" + \
                            seq_to_link[st->first][i].source_orientation_forward + "\t" + \
                            seq_to_link[st->first][i].sink_name + "\t" + \
                            seq_to_link[st->first][i].sink_orientation_forward + "\t" + \
                            seq_to_link[st->first][i].cigar + \
                            "\n";
                }

              }
              
              //TODO iterate over contained segments
              if (seq_to_contained[st->first].size() > 0){
                for (i = 0; i < seq_to_contained[st->first].size(); i++){
                    ret += "C\t" + seq_to_contained[st->first][i] + "\t" + "\n";
                }
              } **/
          }
          // This is a magic comment


        }
        //TODO iterate over annotation lines.


        //Print sequences and links in order, then annotation lines.

        return ret;
    }

    // std::ostream& operator<<(std::ostream& os, const gfak::GFAKluge& g){
    //     os << g.to_string();
    //     return os;
    // }
}
